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
    }

class Object
{
public:
    u8 type = 0;
    u16 x, y, w, h;
    u8 color;
    bool visibility = true;

    void Move(int16_t x, int16_t y, u8 relative);

    void ChangeColor(u8 c);

    void ChangeVisibility(u8 v);

    u8 Rotate(u16 deg);
};

class ObjectRect : public Object
{
public:
    void Resize(int16_t w, int16_t h, u8 relative);

    void Draw();

    ~ObjectRect() {}

    void Free() { this->~ObjectRect(); }
};

class ObjectText : public Object
{
public:
    char *text = 0;
    u16 sX, sY, fH;

    void SetText(char *c);

    void Draw();

    ~ObjectText();

    void Free() { this->~ObjectText(); }
};

class ObjectCircle : public Object
{
public:
    u8 mask;
    u16 r;
    bool full;

    void SetRadius(int16_t r, bool relative);

    void SetMask(u8 mask);

    void SetFill(bool full);

    void Draw();

    ~ObjectCircle() {}

    void Free() { this->~ObjectCircle(); }
};

Object *objects[128];

class Objects
{
public:
    static void Init()
    {
        for (u8 i = 0; i < 128; i++)
        {
            if (objects[i] != 0)
            {
                Free(i);
            }
        }
        memset(objects, 0, sizeof(objects));
    }

    static void PushObject(Object *obj, u8 index);

    static void DrawAll();

    static void Draw(int index);

    static void Free(int index);

    static void OnChange();

    static void Push(u8 type, u8 index);
};
