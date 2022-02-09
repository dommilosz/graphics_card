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
        if (!vga.double_buffer)
        {
            vga.Clear();
        }
        else
        {
            memset(vga.TmpCanvas.img, vga.bgcolor, vga.box_len);
        }

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
};

class Object
{
public:
    u8 type = 0;
    u16 x, y, w, h;
    u8 color;
    bool visibility = true;

    u16 cmd_delay_left = 0;
    u8 *cmd_pointer;
    u8 *cmd_pointer_instr;
    u16 cmd_reg_A = 0;
    u16 cmd_reg_B = 0;

    void Move(int16_t x, int16_t y, u8 relative)
    {
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
        this->visibility = v;
        Objects::OnChange();
    }

    u8 Rotate(u16 deg)
    {
        if (deg > 360)
        {
            return 1;
        }

        Objects::OnChange();
    }

    void ExecCommands();

    void SetCode(u8 *c,u8 length)
    {
        cmd_pointer = (u8 *)this + OBJECT_SIZE;
        memcpy(cmd_pointer, c, length);
        cmd_pointer_instr = cmd_pointer;
        cmd_pointer[length] = 0;
        Objects::OnChange();
    }

    size_t GetSize();
};

class ObjectRect : public Object
{
public:
    void Resize(int16_t w, int16_t h, u8 relative)
    {
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
    char *text = 0;
    u16 sX, sY, fH;

    void SetText(char *c)
    {
        int len = strlen(c);
        text = (char *)this + OBJECT_SIZE+OBJECT_CODE_SIZE;
        memcpy(text, c, len);
        text[len] = 0;
        Objects::OnChange();
    }

    void SetScale(u16 sX, u16 sY, u16 fH)
    {
        this->sX = sX;
        this->sY = sY;
        this->fH = fH;
        Objects::OnChange();
    }

    void Draw()
    {
        memcpy(vga.tmp_buff, text, TMP_BUFFER_SIZE);
        char *c = (char *)vga.tmp_buff;
        u16 dY = 0;
        while (c != 0)
        {
            char *c2 = GetLine(c);
            DrawText(&vga.TmpCanvas, (const char *)c, x, y + dY, color, Font_Copy, fH, sX, sY);
            c = c2;
            dY += sY * fH;
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
        this->mask = mask;
        Objects::OnChange();
    }

    void SetFill(bool fill)
    {
        this->full = fill;
        Objects::OnChange();
    }

    void Draw()
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

    ~ObjectLine() {}

    void Free() { this->~ObjectLine(); }

    size_t Size()
    {
        return sizeof(ObjectLine);
    }
};
