u8 NextByte(Object *obj)
{
    return obj->code_core.read();
}

u16 Next2Byte(Object *obj)
{
    u8 b1 = obj->code_core.read();
    u8 b2 = obj->code_core.read();
    return (b1 << 8) | b2;
}

void NextBuff(u16 length, u8 *buff, Object *obj)
{
    for (u16 i = 0; i < length; i++)
    {
        buff[i] = NextByte(obj);
    }
}

u8 WriteAssetFromAssetDataSource(u8 asset, u16 length, Object *obj, u16 offset = 0xFFFF)
{
    if (!PrepareAsset(asset, length, &offset))
        return 0;
    for (int i = 0; i < length; i++)
    {
        alloc_table[asset].data[offset + i] = NextByte(obj);
    }
    return 1;
}

CODE_ADDRESS_TYPE NextAddress(Object *obj)
{
#if CODE_ADDRESS_SIZE == 8
    return NextByte(obj);
#elif CODE_ADDRESS_SIZE == 16
    return Next2Byte(obj);
#endif
}

u8 cmd_changed = 0;

void SkipByte(Object *obj)
{
    obj->code_core.PC(obj->code_core.PC() + 1);
}

u8 PeekByte(Object *obj)
{
    return obj->code_core.read(true);
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
    if (prop == 0x07)
        return obj->code_core.asset;
    if (prop == 0x08)
    {
        if (obj->type == ObjType_TEXT)
        {
            return ((ObjectText *)obj)->text_asset;
        }
        else
        {
            return 0;
        }
    }

    if (prop == 0xA0)
        return vga.Width;
    if (prop == 0xA1)
        return vga.Heigth;
    if (prop == 0xA2)
        return vga.bgcolor;
    if (prop == 0xA3)
        return millis();
    if (prop == 0xA4)
        return micros();

    if (prop >= 0xD0 && prop <= 0xEF)
    {
        return obj->code_core.readMem(prop - 0xD0);
    }

    if (prop == 0xFE)
    {
        return Next2Byte(obj);
    }
    if (prop == 0xFF)
    {
        return NextByte(obj);
    }

    return 0;
}
void SetProperty(u8 prop, u16 value, Object *obj)
{
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
    if (prop == 0x07)
    {
        obj->code_core.asset = value;
        obj->code_core.jmp(0);
        obj->code_core.sleep(0);
    }
    if (prop == 0x08)
    {
        if (obj->type == ObjType_TEXT)
        {
            ((ObjectText *)obj)->text_asset = value;
        }
    }
    if (prop >= 0xD0 && prop <= 0xEF)
    {
        obj->code_core.setMem(prop - 0xD0, value);
    }
}

u16 NextProperty(Object *obj)
{
    return GetProperty(NextByte(obj), obj);
}

bool HandleJumps(u8 instr, Object *obj)
{
    if (instr == 0xF0)
    { //jmp
        CODE_ADDRESS_TYPE addr = NextProperty(obj);
        obj->code_core.jmp(addr);
        return true;
    }
    if (instr == 0xF1)
    { //jmp if A == B
        CODE_ADDRESS_TYPE addr = NextProperty(obj);
        u16 pval1 = NextProperty(obj);
        u16 pval2 = NextProperty(obj);
        if (pval1 == pval2)
        {
            obj->code_core.jmp(addr);
        }
        return true;
    }
    if (instr == 0xF2)
    { //jmp if A != B
        CODE_ADDRESS_TYPE addr = NextProperty(obj);
        u16 pval1 = NextProperty(obj);
        u16 pval2 = NextProperty(obj);
        if (pval1 != pval2)
        {
            obj->code_core.jmp(addr);
        }
        return true;
    }
    if (instr == 0xF3)
    { //jmp if A > B
        CODE_ADDRESS_TYPE addr = NextProperty(obj);
        u16 pval1 = NextProperty(obj);
        u16 pval2 = NextProperty(obj);
        if (pval1 > pval2)
        {
            obj->code_core.jmp(addr);
        }
        return true;
    }
    if (instr == 0xF4)
    { //jmp if A >= B
        CODE_ADDRESS_TYPE addr = NextProperty(obj);
        u16 pval1 = NextProperty(obj);
        u16 pval2 = NextProperty(obj);
        if (pval1 >= pval2)
        {
            obj->code_core.jmp(addr);
        }
        return true;
    }
    if (instr == 0xF5)
    { //jmp if A < B
        CODE_ADDRESS_TYPE addr = NextProperty(obj);
        u16 pval1 = NextProperty(obj);
        u16 pval2 = NextProperty(obj);
        if (pval1 < pval2)
        {
            obj->code_core.jmp(addr);
        }
        return true;
    }
    if (instr == 0xF6)
    { //jmp if A <= B
        CODE_ADDRESS_TYPE addr = NextProperty(obj);
        u16 pval1 = NextProperty(obj);
        u16 pval2 = NextProperty(obj);
        if (pval1 <= pval2)
        {
            obj->code_core.jmp(addr);
        }
        return true;
    }
    if (instr == 0xF7) //push_jmp
    {
        CODE_ADDRESS_TYPE addr = NextProperty(obj);
        obj->code_core.push(obj->code_core.PC());
        obj->code_core.jmp(addr);
        return true;
    }
    if (instr == 0xF8) //pop
    {
        obj->code_core.jmp(obj->code_core.pop());
        return true;
    }
    return false;
}
bool HandleLoads(u8 instr, Object *obj)
{
    if (instr == 0xE0)
    { //A => property
        SetProperty(NextByte(obj), NextProperty(obj), obj);
        return true;
    }
    if (instr == 0xE2)
    { // swp - A = B, B = A
        u8 prop1 = NextByte(obj);
        u8 prop2 = NextByte(obj);
        u16 tmp = GetProperty(prop1, obj);
        SetProperty(prop1, GetProperty(prop2, obj), obj);
        SetProperty(prop2, tmp, obj);
        return true;
    }
    if (instr == 0xE3) //push
    {
        obj->code_core.push(NextProperty(obj));
        return true;
    }
    if (instr == 0xE4) //pop
    {
        SetProperty(NextByte(obj), obj->code_core.pop(), obj);
        return true;
    }

    return false;
}
bool HandleALU(u8 instr, Object *obj)
{
    if (instr == 0xD0)
    { //prop = prop+prop
        SetProperty(NextByte(obj), NextProperty(obj) + NextProperty(obj), obj);
        return true;
    }
    if (instr == 0xD1)
    { //prop = prop-prop
        SetProperty(NextByte(obj), NextProperty(obj) - NextProperty(obj), obj);
        return true;
    }
    if (instr == 0xD2)
    { //prop = prop*prop
        SetProperty(NextByte(obj), NextProperty(obj) * NextProperty(obj), obj);
        return true;
    }
    if (instr == 0xD3)
    { //prop = prop/prop
        SetProperty(NextByte(obj), NextProperty(obj) / NextProperty(obj), obj);
        return true;
    }
    if (instr == 0xD4)
    { //prop = prop%prop
        SetProperty(NextByte(obj), NextProperty(obj) % NextProperty(obj), obj);
        return true;
    }
    if (instr == 0xD5)
    { //prop = prop + $1
        u8 prop = NextByte(obj);
        SetProperty(prop, GetProperty(prop, obj) + NextProperty(obj), obj);
        return true;
    }
    if (instr == 0xD6)
    { //prop = prop - $1
        u8 prop = NextByte(obj);
        SetProperty(prop, GetProperty(prop, obj) - NextProperty(obj), obj);
        return true;
    }

    return false;
}
bool HandleSleeps(u8 instr, Object *obj)
{
    if (instr == 0xFF)
    { //sleep
        u16 val = NextProperty(obj);
        obj->code_core.sleep(val);
        return true;
    }
    return false;
}
bool HandleMisc(u8 _instr, Object *obj)
{
    if (_instr == 0xC0) //Random
    {
        u8 instr = NextByte(obj);
        if (instr == 0x00)
        { //A = 16 bit random
            u8 prop = NextByte(obj);
            SetProperty(prop, rnd.U16(), obj);
            return true;
        }
        if (instr == 0x01)
        { //A = 16 bit random (range)
            u8 prop = NextByte(obj);
            SetProperty(prop, rnd.U16Max(NextProperty(obj)), obj);
            return true;
        }
        if (instr == 0x02)
        { //A = 16 bit random (range)
            u8 prop = NextByte(obj);
            SetProperty(prop, rnd.U16MinMax(NextProperty(obj), NextProperty(obj)), obj);
            return true;
        }
    }
    if (_instr == 0xC1) //Other
    {
        u8 instr = NextByte(obj);
        if (instr == 0x00) //clear
        {
            obj->clear();
            return true;
        }
        if (instr == 0x01) //draw
        {
            cmd_changed = 1;
            return true;
        }
        if (instr == 0x02) //debug print
        {
            printf("code @ %u: %u\n", obj->code_core.PC(), NextProperty(obj));
            return true;
        }
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
    if (instr == 0xB5)
    { //int = get point regs
        //u16 x = obj->draw_reg_x;
        //u16 y = obj->draw_reg_y;
        //u8 *d = Canvas.img + x / 8 + y * Canvas.wb;
        //obj->draw_reg_color = GetPoint(&Canvas, x, y);
        //return true;
    }
    if (instr == 0xB7)
    { //draw line parm
        u16 x = Next2Byte(obj);
        u16 y = Next2Byte(obj);
        u16 x2 = Next2Byte(obj);
        u16 y2 = Next2Byte(obj);
        u8 col = NextByte(obj);
        DrawLine(&Canvas, x, y, x2, y2, col);
        return true;
    }
    if (instr == 0xB9)
    { //draw rect parm
        u16 x = Next2Byte(obj);
        u16 y = Next2Byte(obj);
        u16 w = Next2Byte(obj);
        u16 h = Next2Byte(obj);
        u8 col = NextByte(obj);
        DrawRect(&Canvas, x, y, w, h, col);
        return true;
    }
    return false;
}
bool HandleAssets(u8 instr, Object *obj)
{
    if (instr == 0xA0)
    { //set asset - asset, offset, length, data
        u8 asset = NextProperty(obj);
        u16 offset = NextProperty(obj);
        u8 refresh = NextByte(obj);
        u16 length = NextProperty(obj);
        if (refresh)
            Objects::OnChangingAsset(asset);
        WriteAssetFromAssetDataSource(asset, length, obj, offset);
        if (refresh)
        {
            Objects::OnChangedAsset(asset);
            cmd_changed = 1;
        }
        return true;
    }
    if (instr == 0xA1)
    { //read asset8- asset, offset, property
        u8 asset = NextProperty(obj);
        u16 offset = NextProperty(obj);
        u8 prop = NextByte(obj);
        u8 val = ReadAsset(asset, offset);
        SetProperty(prop, val, obj);
        return true;
    }
    if (instr == 0xA2)
    { //read asset16- asset, offset, property
        u8 asset = NextProperty(obj);
        u16 offset = NextProperty(obj);
        u8 prop = NextByte(obj);
        u16 val = ReadAsset(asset, offset) << 8;
        val = val | ReadAsset(asset, offset + 1);
        SetProperty(prop, val, obj);
        return true;
    }
    if (instr == 0xA3)
    { //set asset byte - asset, offset, property
        u8 asset = NextProperty(obj);
        u16 offset = NextProperty(obj);
        u8 refresh = NextByte(obj);
        u8 data = NextProperty(obj) & 0xFF;

        u8 buff[1];
        buff[0] = data;
        if (refresh)
            Objects::OnChangingAsset(asset);
        WriteAsset(asset, buff, 1, offset);
        if (refresh)
        {
            Objects::OnChangedAsset(asset);
            cmd_changed = 1;
        }
        return true;
    }
    if (instr == 0xA4)
    { //set asset u16 - asset, offset, property
        u8 asset = NextProperty(obj);
        u16 offset = NextProperty(obj);
        u8 refresh = NextByte(obj);
        u16 data = NextProperty(obj);

        u8 buff[2];
        buff[0] = data >> 8;
        buff[1] = data & 0xFF;

        if (refresh)
            Objects::OnChangingAsset(asset);
        WriteAsset(asset, buff, 2, offset);
        if (refresh)
        {
            Objects::OnChangedAsset(asset);
            cmd_changed = 1;
        }
        return true;
    }
    if (instr == 0xA5)
    { //int_to_str
        u8 asset = NextProperty(obj);
        u16 offset = NextProperty(obj);
        u8 refresh = NextByte(obj);
        u16 number = NextProperty(obj);
        char num_char[8];
        sprintf(num_char, "%d", number);
        if (refresh)
            Objects::OnChangingAsset(asset);
        WriteAsset(asset, (u8 *)num_char, 8, offset);
        if (refresh)
        {
            Objects::OnChangedAsset(asset);
            cmd_changed = 1;
        }
        return true;
    }
    if (instr == 0xA6) //append
    {
        u8 asset = NextProperty(obj);
        u8 refresh = NextByte(obj);
        u16 length = NextProperty(obj);

        u16 offset = StrlenAsset(asset);

        if (refresh)
            Objects::OnChangingAsset(asset);
        WriteAssetFromAssetDataSource(asset, length, obj, offset);
        if (refresh)
        {
            Objects::OnChangedAsset(asset);
            cmd_changed = 1;
        }
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

        return true;
    }
    if (instr == 0x11)
    {
        obj->code_core.delay_to = make_timeout_time_ms(99999999);
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
    if (HandleMisc(instr, obj))
        return true;
    if (HandleDraw(instr, obj))
        return true;
    if (HandleAssets(instr, obj))
        return true;

    return false;
}