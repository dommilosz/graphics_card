#include "consts.h"

#define ObjectAction(x, index)           \
    {                                    \
        Object *obj = objects[index];    \
        u8 type = obj->type;             \
        if (type == ObjType_RECTANGLE)   \
        {                                \
            ((ObjectRect *)obj)->x();    \
        }                                \
        else if (type == ObjType_TEXT)   \
        {                                \
            ((ObjectText *)obj)->x();    \
        }                                \
        else if (type == ObjType_CIRCLE) \
        {                                \
            ((ObjectCircle *)obj)->x();  \
        }                                \
        else if (type == ObjType_LINE)   \
        {                                \
            ((ObjectLine *)obj)->x();    \
        }                                \
        else if (type == ObjType_BLOB)   \
        {                                \
            ((ObjectBlob *)obj)->x();    \
        }                                \
    }

#define ObjectActionWR(x, obj)           \
    {                                    \
        u8 type = obj->type;             \
        if (type == ObjType_RECTANGLE)   \
        {                                \
            ((ObjectRect *)obj)->x();    \
        }                                \
        else if (type == ObjType_TEXT)   \
        {                                \
            ((ObjectText *)obj)->x();    \
        }                                \
        else if (type == ObjType_CIRCLE) \
        {                                \
            ((ObjectCircle *)obj)->x();  \
        }                                \
        else if (type == ObjType_LINE)   \
        {                                \
            ((ObjectLine *)obj)->x();    \
        }                                \
        else if (type == ObjType_BLOB)   \
        {                                \
            ((ObjectBlob *)obj)->x();    \
        }                                \
    }

#define ObjAssert(x)               \
    Object *_obj = objects[index]; \
    if (_obj->type != x)           \
    {                              \
        Status(STATUS_WRONG_TYPE); \
        return;                    \
    }

class Object;
class ObjectRect;
class ObjectCircle;
class ObjectLine;

Object *objects[OBJECTS_COUNT];
bool ONC = true;

class Objects
{
public:
    static void Init()
    {
        ONC = true;
        memset(objects, 0, sizeof(objects));
    }

    static void DrawAll()
    {
        vga.ClearDouble();
        for (u8 i = 0; i < OBJECTS_COUNT; i++)
        {
            if (objects[i] != 0)
            {
                Draw(i);
            }
        }
        vga.MergeCanvas();
    }

    static void ReDrawAll()
    {
        vga.Clear();
        for (u8 i = 0; i < OBJECTS_COUNT; i++)
        {
            if (objects[i] != 0)
            {
                Draw(i);
            }
        }
        vga.MergeCanvas();
    }

    static void Draw(int index);

    static void Free(int index);

    static void OnChange()
    {
        if (!ONC)
            return;
        Objects::DrawAll();
    }

    static void Push(u8 type, u8 index);

    static void ExecCode();

    static void OnChangingAsset(u8 asset);

    static void OnChangedAsset(u8 asset);
};

class CodeCore
{
public:
    u16 mem[32];
    /*
        0-25  - for user
        25    - own_time
        26    - PC
        27    - SP
        28-31 - Call Stack
    */
    absolute_time_t delay_to;
    u8 asset = 0;
    u16 PC() { return mem[26]; }
    void PC(u16 pc) { mem[26] = pc; }

    u16 SP() { return mem[27]; }
    void SP(u16 sp) { mem[27] = sp; }

    u16 own_time() { return (mem[25] ); }
    void own_time(u16 t) { mem[25] = t; }

    void setMem(u8 index, u16 data)
    {
        //printf("set: %u = %u\n", index, data);
        mem[index] = data;
    }

    u16 readMem(u8 index)
    {
        //printf("get: %u = %u\n", index, mem[index]);
        return mem[index];
    }

    void push(u16 addr)
    {
        u16 call_stack_ptr = SP();
        if (call_stack_ptr >= 4)
            return;
        call_stack_ptr++;
        SP(call_stack_ptr);
        setMem(27 + call_stack_ptr, addr);
    }

    u16 pop()
    {
        u16 call_stack_ptr = SP();
        if (call_stack_ptr <= 0)
            return 0;
        call_stack_ptr--;
        SP(call_stack_ptr);
        return readMem(28 + call_stack_ptr);
    }

    void jmp(u16 address)
    {
        //printf("jmp: %u\n", address);
        mem[26] = address;
    }

    void sleep(u16 delay)
    {
        //printf("sleep: %u\n", delay);
        delay_to = make_timeout_time_ms(delay);
    }

    u8 read(bool peek = false)
    {
        u8 b = ReadAsset(asset, PC());
        if (!peek)
            PC(PC() + 1);
        return b;
    }
    
};

class Object
{
public:
    u8 type = 0;
    u16 x, y, w, h;
    u8 color;
    bool visibility = true;
    CodeCore code_core;

    void clear();
    void Draw();

    void Move(int16_t x, int16_t y, u8 relative)
    {
        clear();
        if (relative == 0)
        {
            this->x = x;
            this->y = y;
        }
        else
        {
            this->x += x;
            this->y += y;
        }
        Objects::OnChange();
    }

    void Resize(int16_t w, int16_t h, u8 relative)
    {
        clear();
        if (relative == 0)
        {
            this->w = w;
            this->h = h;
        }
        else
        {
            this->w += w;
            this->h += h;
        }
        Objects::OnChange();
    }

    void ChangeColor(u8 c)
    {
        this->color = c;
        Objects::OnChange();
    }

    void ChangeVisibility(u8 v)
    {
        clear();
        this->visibility = v;
        Objects::OnChange();
    }

    u8 Rotate(u16 deg)
    {
        clear();
        if (deg > 360)
        {
            return 1;
        }

        Objects::OnChange();
    }

    void ExecCommands();

    void SetCode(u8 asset)
    {
        code_core.asset = asset;
        code_core.jmp(0);
        code_core.sleep(0);
        Objects::OnChange();
    }

    size_t Size()
    {
        return sizeof(Object);
    }

    size_t GetSize();
};

class ObjectRect : public Object
{
public:
    void Draw()
    {
        DrawRect(&vga.TmpCanvas, x, y, w, h, color);
    }

    void clear()
    {
        DrawRect(&vga.TmpCanvas, x, y, w, h, vga.bgcolor);
    }

    ~ObjectRect() {}

    void Free() { this->~ObjectRect(); }

    size_t Size()
    {
        return sizeof(ObjectRect);
    }
};

class ObjectText : public Object
{
public:
    u8 text_asset = 0;
    u16 sX, sY, fH;

    void SetText(u8 asset)
    {
        clear();
        text_asset = asset;
        Objects::OnChange();
    }

    void SetScale(u16 sX, u16 sY, u16 fH)
    {
        clear();
        this->sX = sX;
        this->sY = sY;
        this->fH = fH;
        Objects::OnChange();
    }

    void Draw()
    {
        Draw(color);
    }

    void Draw(u8 color)
    {
        u16 dY = 0;
        u16 dX = 0;
        int i = 0;
        while (true)
        {
            char data[2];
            data[0] = ReadAsset(text_asset, i);
            data[1] = 0;

            if (data[0] == 0)
            {
                return;
            }
            if (data[0] == 10)
            {
                dY += sY * fH;
                dX = 0;
                i++;
                continue;
            }
            if (data[0] == 13)
            {
                i++;
                continue;
            }

            DrawText(&vga.TmpCanvas, (const char *)data, x + dX, y + dY, color, Font_Copy, fH, sX, sY);
            dX += sX * 8;
            i++;
        }
    }

    void clear()
    {
        u16 dY = 0;
        u16 dX = 0;
        int i = 0;
        while (true)
        {
            char data[2];
            data[0] = ReadAsset(text_asset, i);
            data[1] = 0;

            if (data[0] == 0)
            {
                return;
            }
            if (data[0] == 10)
            {
                dY += sY * fH;
                dX = 0;
                i++;
                continue;
            }
            if (data[0] == 13)
            {
                i++;
                continue;
            }

            DrawRect(&Canvas, x + dX, y + dY, 8 * sX, 8 * sY, vga.bgcolor);
            dX += sX * 8;
            i++;
        }
    }

    ~ObjectText()
    {
    }

    void Free() { this->~ObjectText(); }

    size_t Size()
    {
        return sizeof(ObjectText);
    }
};

class ObjectCircle : public Object
{
public:
    u8 mask;
    u16 r;
    bool full;

    void SetRadius(int16_t r, bool relative)
    {
        clear();
        if (!relative)
        {
            this->r = r;
        }
        else
        {
            this->r += r;
        }
        Objects::OnChange();
    }

    void SetMask(u8 mask)
    {
        clear();
        this->mask = mask;
        Objects::OnChange();
    }

    void SetFill(bool fill)
    {
        clear();
        this->full = fill;
        Objects::OnChange();
    }

    void Draw()
    {
        Draw(color);
    }

    void Draw(u8 color)
    {
        if (this->full)
        {
            DrawFillCircle(&vga.TmpCanvas, this->x, this->y, this->r, this->color, this->mask);
        }
        else
        {
            DrawCircle(&vga.TmpCanvas, this->x, this->y, this->r, this->color, this->mask);
        }
    }

    void clear()
    {
        Draw(vga.bgcolor);
    }

    ~ObjectCircle() {}

    void Free() { this->~ObjectCircle(); }

    size_t Size()
    {
        return sizeof(ObjectCircle);
    }
};

class ObjectLine : public Object
{
public:
    u16 x2, y2;
    void Move(int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool relative)
    {
        clear();
        if (relative == 0)
        {
            this->x = x1;
            this->y = y1;
            this->x2 = x2;
            this->y2 = y2;
        }
        else
        {
            this->x += x1;
            this->y += y1;
            this->x2 += x2;
            this->y2 += y2;
        }
        Objects::OnChange();
    }

    void Draw()
    {
        DrawLine(&Canvas, x, y, x2, y2, color);
    }

    void clear()
    {
        DrawLine(&Canvas, x, y, x2, y2, vga.bgcolor);
    }

    ~ObjectLine() {}

    void Free() { this->~ObjectLine(); }

    size_t Size()
    {
        return sizeof(ObjectLine);
    }
};

class ObjectBlob : public Object
{
public:
    u8 img_asset;

    void SetImage(u8 asset)
    {
        img_asset = asset;
        Objects::OnChange();
    }

    void Draw()
    {
        if (img_asset == 0)
            return;
        u8 compressed = ReadAsset(img_asset, 0);

        bool rotated = false;
        if (compressed >> 7 == 1)
        {
            compressed = compressed & 0x7F;
            rotated = true;
        }

        if (compressed == 0)
        {
            for (int i = 0; i < w * h; i++)
            {
                int b = ReadAsset(img_asset, i + 1);
                int _x = i / h;
                int _y = i % h;
                if (rotated)
                {
                    _y = h - (i / w) - 1;
                    _x = i % w;
                }
                DrawPoint(&Canvas, _x + x, _y + y, b);
            }
        }
        else if (compressed == 1)
        {
            int tmp_color;
            int tmp_next;
            int asset_index = 1;

            tmp_color = ReadAsset(img_asset, asset_index);
            asset_index += 1;
            tmp_next = Read3Asset(img_asset, asset_index);
            asset_index += 3;

            for (int i = 0; i < w * h; i++)
            {
                if (i > tmp_next)
                {
                    tmp_color = ReadAsset(img_asset, asset_index);
                    asset_index += 1;
                    tmp_next = Read3Asset(img_asset, asset_index);
                    asset_index += 3;
                }

                int _x = i / h;
                int _y = i % h;
                if (rotated)
                {
                    _y = h - (i / w) - 1;
                    _x = i % w;
                }
                DrawPoint(&Canvas, _x + x, _y + y, tmp_color);
            }
        }
        else if (compressed == 2)
        {
            int tmp_color;
            int tmp_next;
            int asset_index = 1;

            tmp_color = ReadAsset(img_asset, asset_index);
            asset_index += 1;
            tmp_next = Read2Asset(img_asset, asset_index);
            asset_index += 2;

            for (int i = 0; i < w * h; i++)
            {
                if (i > tmp_next)
                {
                    tmp_color = ReadAsset(img_asset, asset_index);
                    asset_index += 1;
                    tmp_next = Read2Asset(img_asset, asset_index);
                    asset_index += 2;
                }
                int _x = i / h;
                int _y = i % h;
                if (rotated)
                {
                    _y = h - (i / w) - 1;
                    _x = i % w;
                }
                DrawPoint(&Canvas, _x + x, _y + y, tmp_color);
            }
        }
        else if (compressed == 3)
        {
            u32 tmp_color;
            u32 tmp_next;
            u32 asset_index = 1;

            u32 colors_loc = Read3Asset(img_asset, asset_index);
            asset_index += 3;

            tmp_next = Read3Asset(img_asset, asset_index);
            tmp_color = tmp_next >> 18;
            tmp_next = tmp_next & 0x03FFFF;
            asset_index += 3;
            u32 length = w * h;

            u8 read_color = 0;
            read_color = ReadAsset(img_asset, colors_loc + tmp_color);
            for (int i = 0; i < length; i++)
            {
                if (i > tmp_next)
                {
                    tmp_next = Read3Asset(img_asset, asset_index);
                    tmp_color = tmp_next >> 18;
                    tmp_next = tmp_next & 0x03FFFF;
                    read_color = ReadAsset(img_asset, colors_loc + tmp_color);
                    asset_index += 3;
                }

                int _x = i / h;
                int _y = i % h;
                if (rotated)
                {
                    _y = h - (i / w) - 1;
                    _x = i % w;
                }
                DrawPoint(&Canvas, _x + x, _y + y, read_color);
            }
        }
    }

    void clear()
    {
        DrawRect(&Canvas, x, y, w, h, vga.bgcolor);
    }

    ~ObjectBlob() {}

    void Free() { this->~ObjectBlob(); }

    size_t Size()
    {
        return sizeof(ObjectBlob);
    }
};

u8 obj_mem[OBJECTS_COUNT * OBJECT_SIZE];