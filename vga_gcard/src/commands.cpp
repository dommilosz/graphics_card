u8 NextByte(Object *obj)
{
    u8 b = ReadAsset(obj->cmd_asset, obj->cmd_pointer);
    obj->cmd_pointer = obj->cmd_pointer + 1;
    return b;
}

u16 Next2Byte(Object *obj)
{
    u8 b1 = ReadAsset(obj->cmd_asset, obj->cmd_pointer);
    u8 b2 = ReadAsset(obj->cmd_asset, obj->cmd_pointer + 1);
    obj->cmd_pointer = obj->cmd_pointer + 2;
    return (b1 << 8) | b2;
}

CODE_ADDRESS_TYPE NextAddress(Object *obj)
{
#if CODE_ADDRESS_SIZE == 8
    return NextByte(obj);
#elif CODE_ADDRESS_SIZE == 16
    return Next2Byte(obj);
#endif
}

bool cmd_changed = false;

void SkipByte(Object *obj)
{
    obj->cmd_pointer = obj->cmd_pointer + 1;
}

u8 PeekByte(Object *obj)
{
    u8 b = ReadAsset(obj->cmd_asset, obj->cmd_pointer);
    return b;
}

u16 GetProperty(u8 prop, Object *obj)
{
    if (prop == 0x00)
        return obj->x;
    if (prop == 0x01)
        return obj->y;
    if (prop == 0x02)
        return obj->w;
    if (prop == 0x03)
        return obj->h;
    if (prop == 0x04)
        return obj->color;
    if (prop == 0x05)
        return obj->visibility;
    if (prop == 0x06)
        return obj->type;

    if (prop == 0xD0)
        return vga.Width;
    if (prop == 0xD1)
        return vga.Heigth;
    if (prop == 0xD2)
        return vga.bgcolor;

    if (prop == 0xF0)
        return obj->draw_reg_color;
    if (prop == 0xF1)
        return obj->draw_reg_x;
    if (prop == 0xF2)
        return obj->draw_reg_y;
    if (prop == 0xF3)
        return obj->draw_reg_x2;
    if (prop == 0xF4)
        return obj->draw_reg_y2;

    return 0;
}
void SetProperty(u8 prop, u16 value, Object *obj)
{
    if (prop == 0xF0)
    {
        obj->draw_reg_color = value;
        return;
    }
    if (prop == 0xF1)
    {
        obj->draw_reg_x = value;
        return;
    }
    if (prop == 0xF2)
    {
        obj->draw_reg_y = value;
        return;
    }
    if (prop == 0xF3)
    {
        obj->draw_reg_x2 = value;
        return;
    }
    if (prop == 0xF4)
    {
        obj->draw_reg_y2 = value;
        return;
    }

    ObjectActionWR(clear, obj);
    if (prop == 0x00)
        obj->x = value;
    if (prop == 0x01)
        obj->y = value;
    if (prop == 0x02)
        obj->w = value;
    if (prop == 0x03)
        obj->h = value;
    if (prop == 0x04)
        obj->color = value;
    if (prop == 0x05)
        obj->visibility = value;
    if (prop == 0x06)
        obj->type = value;
    cmd_changed = true;
}

bool HandleJumps(u8 instr, Object *obj)
{
    if (instr == 0xF0)
    { //jmp
        CODE_ADDRESS_TYPE addr = NextAddress(obj);
        obj->cmd_pointer = addr;
        return true;
    }
    if (instr == 0xF1)
    { //jmp if A == B
        CODE_ADDRESS_TYPE addr = NextAddress(obj);
        if (obj->cmd_reg_A == obj->cmd_reg_B)
        {
            obj->cmd_pointer = addr;
            return true;
        }
        return true;
    }
    if (instr == 0xF2)
    { //jmp if A != B
        CODE_ADDRESS_TYPE addr = NextAddress(obj);
        if (obj->cmd_reg_A != obj->cmd_reg_B)
        {
            obj->cmd_pointer = addr;
        }
        return true;
    }
    if (instr == 0xF3)
    { //jmp if A > B
        CODE_ADDRESS_TYPE addr = NextAddress(obj);
        if (obj->cmd_reg_A > obj->cmd_reg_B)
        {
            obj->cmd_pointer = addr;
        }
        return true;
    }
    if (instr == 0xF4)
    { //jmp if A >= B
        CODE_ADDRESS_TYPE addr = NextAddress(obj);
        if (obj->cmd_reg_A >= obj->cmd_reg_B)
        {
            obj->cmd_pointer = addr;
        }
        return true;
    }
    if (instr == 0xF5)
    { //jmp if A < B
        CODE_ADDRESS_TYPE addr = NextAddress(obj);
        if (obj->cmd_reg_A < obj->cmd_reg_B)
        {
            obj->cmd_pointer = addr;
        }
        return true;
    }
    if (instr == 0xF6)
    { //jmp if A <= B
        CODE_ADDRESS_TYPE addr = NextAddress(obj);
        if (obj->cmd_reg_A <= obj->cmd_reg_B)
        {
            obj->cmd_pointer = addr;
        }
        return true;
    }
    return false;
}
bool HandleLoads(u8 instr, Object *obj)
{
    if (instr == 0xE0)
    { // number => A
        obj->cmd_reg_A = mergeu16(NextByte(obj), NextByte(obj));
        return true;
    }
    if (instr == 0xE1)
    { // number => A
        obj->cmd_reg_B = mergeu16(NextByte(obj), NextByte(obj));
        return true;
    }
    if (instr == 0xE2)
    { // property => A
        obj->cmd_reg_A = GetProperty(NextByte(obj), obj);
        return true;
    }
    if (instr == 0xE3)
    { // property => B
        obj->cmd_reg_B = GetProperty(NextByte(obj), obj);
        return true;
    }

    if (instr == 0xE4)
    { //A => property
        SetProperty(NextByte(obj), obj->cmd_reg_A, obj);
        return true;
    }
    if (instr == 0xE5)
    { //B => property
        SetProperty(NextByte(obj), obj->cmd_reg_B, obj);
        return true;
    }

    if (instr == 0xE6)
    { // number => A (8b)
        obj->cmd_reg_A = NextByte(obj);
        return true;
    }
    if (instr == 0xE7)
    { // number => A (8b)
        obj->cmd_reg_B = NextByte(obj);
        return true;
    }
    if (instr == 0xE8)
    { // load A, B
        obj->cmd_reg_A = obj->cmd_reg_B;
        return true;
    }
    if (instr == 0xE9)
    { // load B, A
        obj->cmd_reg_B = obj->cmd_reg_A;
        return true;
    }
    if (instr == 0xEA)
    { // swp - A = B, B = A
        u16 tmp = obj->cmd_reg_A;
        obj->cmd_reg_A = obj->cmd_reg_B;
        obj->cmd_reg_B = tmp;
        return true;
    }

    return false;
}
bool HandleALU(u8 instr, Object *obj)
{
    if (instr == 0xD0)
    { //A = A+B
        u16 res = obj->cmd_reg_A + obj->cmd_reg_B;
        obj->cmd_reg_A = res;
        return true;
    }
    if (instr == 0xD1)
    { //A = A-B
        obj->cmd_reg_A = obj->cmd_reg_A - obj->cmd_reg_B;
        return true;
    }
    if (instr == 0xD2)
    { //A = A*B
        obj->cmd_reg_A = obj->cmd_reg_A * obj->cmd_reg_B;
        return true;
    }
    if (instr == 0xD3)
    { //A = A/B
        obj->cmd_reg_A = obj->cmd_reg_A / obj->cmd_reg_B;
        return true;
    }
    if (instr == 0xD4)
    { //A = A%B
        obj->cmd_reg_A = obj->cmd_reg_A % obj->cmd_reg_B;
        return true;
    }
    if (instr == 0xD5)
    { //A = A + $1
        obj->cmd_reg_A = obj->cmd_reg_A + Next2Byte(obj);
        return true;
    }
    if (instr == 0xD6)
    { //A = A - $1
        obj->cmd_reg_A = obj->cmd_reg_A - Next2Byte(obj);
        return true;
    }
    if (instr == 0xD7)
    { //prop = prop + $1
        u8 prop = NextByte(obj);
        SetProperty(prop, GetProperty(prop, obj) + Next2Byte(obj), obj);
        return true;
    }
    if (instr == 0xD8)
    { //prop = prop - $1
        u8 prop = NextByte(obj);
        SetProperty(prop, GetProperty(prop, obj) - Next2Byte(obj), obj);
        return true;
    }

    if (instr == 0xD9)
    { //A = A + $1
        obj->cmd_reg_A = obj->cmd_reg_A + NextByte(obj);
        return true;
    }
    if (instr == 0xDA)
    { //A = A - $1
        obj->cmd_reg_A = obj->cmd_reg_A - NextByte(obj);
        return true;
    }
    if (instr == 0xDB)
    { //prop = prop + $1
        u8 prop = NextByte(obj);
        SetProperty(prop, GetProperty(prop, obj) + NextByte(obj), obj);
        return true;
    }
    if (instr == 0xDC)
    { //prop = prop - $1
        u8 prop = NextByte(obj);
        SetProperty(prop, GetProperty(prop, obj) - NextByte(obj), obj);
        return true;
    }

    return false;
}
bool HandleSleeps(u8 instr, Object *obj)
{
    if (instr == 0xFD)
    { //sleep from A
        obj->cmd_delay_to = make_timeout_time_ms(obj->cmd_reg_A);
        return true;
    }
    if (instr == 0xFE)
    { //sleep from B
        obj->cmd_delay_to = make_timeout_time_ms(obj->cmd_reg_B);
        return true;
    }
    if (instr == 0xFF)
    { //sleep
        obj->cmd_delay_to = make_timeout_time_ms(Next2Byte(obj));
        return true;
    }
    if (instr == 0xFC)
    { //sleep
        obj->cmd_delay_to = make_timeout_time_ms(NextByte(obj));
        return true;
    }
    return false;
}
bool HandleRandom(u8 instr, Object *obj)
{
    if (instr == 0xC0)
    { //A = 16 bit random
        obj->cmd_reg_A = rnd.U16();
    }
    if (instr == 0xC1)
    { //A = 16 bit random (range)
        obj->cmd_reg_A = rnd.U16Max(Next2Byte(obj));
    }
    if (instr == 0xC2)
    { //A = 8 bit random (range)
        obj->cmd_reg_A = rnd.U16Max(NextByte(obj));
    }
    if (instr == 0xC3)
    { //A = 16 bit random (range)
        obj->cmd_reg_A = rnd.U16MinMax(Next2Byte(obj), Next2Byte(obj));
    }
    if (instr == 0xC4)
    { //A = 8 bit random (range)
        obj->cmd_reg_A = rnd.U16MinMax(NextByte(obj), NextByte(obj));
    }
    if (instr == 0xC5)
    { //A = bit random (range) 0 - B
        obj->cmd_reg_A = rnd.U16Max(obj->cmd_reg_B);
    }
    if (instr == 0xC6)
    { //A = bit random (range) A - B
        obj->cmd_reg_A = rnd.U16MinMax(obj->cmd_reg_A, obj->cmd_reg_B);
    }
    return false;
}
bool HandleDraw(u8 instr, Object *obj)
{
    if (instr == 0xB0)
    { //draw point
        u16 x = Next2Byte(obj);
        u16 y = Next2Byte(obj);
        u8 col = NextByte(obj);
        DrawPoint(&Canvas, x, y, col);
        return true;
    }
    if (instr == 0xB1)
    { //draw point AB
        u16 x = obj->cmd_reg_A;
        u16 y = obj->cmd_reg_B;
        u8 col = obj->draw_reg_color;
        DrawPoint(&Canvas, x, y, col);
        return true;
    }
    if (instr == 0xB2)
    { //int = get point
        u16 x = Next2Byte(obj);
        u16 y = Next2Byte(obj);
        u8 *d = Canvas.img + x / 8 + y * Canvas.wb;
        obj->draw_reg_color = GetPoint(&Canvas, x, y);
        return true;
    }
    if (instr == 0xB3)
    { //int = get point AB
        u16 x = obj->cmd_reg_A;
        u16 y = obj->cmd_reg_B;
        u8 *d = Canvas.img + x / 8 + y * Canvas.wb;
        obj->draw_reg_color = GetPoint(&Canvas, x, y);
        return true;
    }
    if (instr == 0xB4)
    { //draw point regs
        u16 x = obj->draw_reg_x;
        u16 y = obj->draw_reg_y;
        u8 col = obj->draw_reg_color;
        DrawPoint(&Canvas, x, y, col);
        return true;
    }
    if (instr == 0xB5)
    { //int = get point regs
        u16 x = obj->draw_reg_x;
        u16 y = obj->draw_reg_y;
        u8 *d = Canvas.img + x / 8 + y * Canvas.wb;
        obj->draw_reg_color = GetPoint(&Canvas, x, y);
        return true;
    }
    if (instr == 0xB6)
    { //draw line regs
        u16 x = obj->draw_reg_x;
        u16 y = obj->draw_reg_y;
        u16 x2 = obj->draw_reg_x2;
        u16 y2 = obj->draw_reg_y2;
        u8 col = obj->draw_reg_color;
        DrawLine(&Canvas, x, y,x2,y2, col);
        return true;
    }
    if (instr == 0xB7)
    { //draw line parm
        u16 x = Next2Byte(obj);
        u16 y = Next2Byte(obj);
        u16 x2 = Next2Byte(obj);
        u16 y2 = Next2Byte(obj);
        u8 col = NextByte(obj);
        DrawLine(&Canvas, x, y,x2,y2, col);
        return true;
    }
    return false;
}

bool ExecuteCommand(Object *obj)
{
    //printf("exec: %x\n",PeekByte(obj));
    u8 instr = NextByte(obj);

    if (instr == 0x10)
    {
        printf("A: %u\n", obj->cmd_reg_A);
        printf("B: %u\n", obj->cmd_reg_B);
        return true;
    }

    if (HandleJumps(instr, obj))
        return true;
    if (HandleLoads(instr, obj))
        return true;
    if (HandleALU(instr, obj))
        return true;
    if (HandleSleeps(instr, obj))
        return true;
    if (HandleRandom(instr, obj))
        return true;
    if (HandleDraw(instr, obj))
        return true;

    return false;
}