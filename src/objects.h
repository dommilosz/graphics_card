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
    }

#define ObjAssert(x)               \
    Object *_obj = objects[index]; \
    if (_obj->type != x)           \
    {                              \
        Status(STATUS_WRONG_TYPE); \
        return;                    \
    }

u8 obj_mem[OBJECTS_COUNT * OBJECT_ALLOC];
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
};

class Object
{
public:
    u8 type = 0;
    u16 x, y, w, h;
    u8 color;
    bool visibility = true;

    absolute_time_t cmd_delay_to;
    u8 cmd_asset = 0;
    CODE_ADDRESS_TYPE cmd_pointer = 0;
    u16 cmd_reg_A = 0;
    u16 cmd_reg_B = 0;

    u16 draw_reg_x = 0;
    u16 draw_reg_y = 0;
    u16 draw_reg_x2 = 0;
    u16 draw_reg_y2 = 0;
    u16 draw_reg_color = 0;

    void clear();

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
        cmd_asset = asset;
        cmd_pointer = 0;
        cmd_delay_to = make_timeout_time_us(0);
        Objects::OnChange();
    }

    size_t GetSize();
};

class ObjectRect : public Object
{
public:
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

    void Draw(u8 color){
        u16 dY = 0;
        int i = 0;
        while (true)
        {
            char data[2];
            data[0] = ReadAsset(text_asset, i);
            data[1] = 0;

            DrawText(&vga.TmpCanvas, (const char *)data, x, y + dY, color, Font_Copy, fH, sX, sY);

            if (data[0] == 0)
            {
                return;
            }
            if (data[0] == 10)
            {
                dY += sY * fH;
            }
            i++;
        }
    }

    void clear()
    {
        Draw(vga.bgcolor);
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
