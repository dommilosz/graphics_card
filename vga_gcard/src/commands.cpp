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
    return 0;
}
void SetProperty(u8 prop,u16 value, Object *obj)
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
    Objects::OnChange();
}

u8 *HandleJumps(u8 *ptr, Object *obj)
{
    if (ptr[0] == 0xF0)
    { //jmp
        return (obj->cmd_pointer + ptr[1]);
    }
    if (ptr[0] == 0xF1)
    { //jmp if A == B
        if (obj->cmd_reg_A == obj->cmd_reg_B)
        {
            return (obj->cmd_pointer + ptr[1]);
        }
        return ptr + 2;
    }
    if (ptr[0] == 0xF2)
    { //jmp if A != B
        if (obj->cmd_reg_A != obj->cmd_reg_B)
        {
            return (obj->cmd_pointer + ptr[1]);
        }
        return ptr + 2;
    }
    if (ptr[0] == 0xF3)
    { //jmp if A > B
        if (obj->cmd_reg_A > obj->cmd_reg_B)
        {
            return (obj->cmd_pointer + ptr[1]);
        }
        return ptr + 2;
    }
    if (ptr[0] == 0xF4)
    { //jmp if A >= B
        if (obj->cmd_reg_A >= obj->cmd_reg_B)
        {
            return (obj->cmd_pointer + ptr[1]);
        }
        return ptr + 2;
    }
    if (ptr[0] == 0xF5)
    { //jmp if A < B
        if (obj->cmd_reg_A < obj->cmd_reg_B)
        {
            return (obj->cmd_pointer + ptr[1]);
        }
        return ptr + 2;
    }
    if (ptr[0] == 0xF6)
    { //jmp if A <= B
        if (obj->cmd_reg_A <= obj->cmd_reg_B)
        {
            return (obj->cmd_pointer + ptr[1]);
        }
        return ptr + 2;
    }
    return 0;
}
u8 *HandleLoads(u8 *ptr, Object *obj)
{
    if (ptr[0] == 0xE0)
    { // number => A
        obj->cmd_reg_A = mergeu16(ptr[1], ptr[2]);
        return ptr + 3;
    }
    if (ptr[0] == 0xE1)
    { // number => A
        obj->cmd_reg_B = mergeu16(ptr[1], ptr[2]);
        return ptr + 3;
    }
    if (ptr[0] == 0xE2)
    { // property => A
        obj->cmd_reg_A = GetProperty(ptr[1], obj);
        return ptr + 2;
    }
    if (ptr[0] == 0xE3)
    { // property => B
        obj->cmd_reg_B = GetProperty(ptr[1], obj);
        return ptr + 2;
    }

    if(ptr[0] == 0xE4){ //A => property
        SetProperty(ptr[1],obj->cmd_reg_A,obj);
        return ptr+2;
    }
    if(ptr[0] == 0xE5){ //B => property
        SetProperty(ptr[1],obj->cmd_reg_B,obj);
        return ptr+2;
    }

    return 0;
}
u8 *HandleALU(u8 *ptr, Object *obj){
    if(ptr[0] == 0xD0){ //A = A+B
        u16 res = obj->cmd_reg_A + obj->cmd_reg_B;
        obj->cmd_reg_A = res;
        return ptr+1;
    }
    if(ptr[0] == 0xD1){ //A = A-B
        obj->cmd_reg_A = obj->cmd_reg_A - obj->cmd_reg_B; 
        return ptr+1;
    }
    if(ptr[0] == 0xD2){ //A = A*B
        obj->cmd_reg_A = obj->cmd_reg_A * obj->cmd_reg_B; 
        return ptr+1;
    }
    if(ptr[0] == 0xD3){ //A = A/B
        obj->cmd_reg_A = obj->cmd_reg_A / obj->cmd_reg_B; 
        return ptr+1;
    }
    if(ptr[0] == 0xD4){ //A = A%B
        obj->cmd_reg_A = obj->cmd_reg_A % obj->cmd_reg_B; 
        return ptr+1;
    }
    return 0;
}


u8 *ExecuteCommand(u8 *ptr, Object *obj)
{
    //printf(" %x %u %u\n",ptr[0],obj->cmd_reg_A,obj->cmd_reg_B);

    u8 *ret_ptr = 0;
    if (ptr[0] == 0x01)
    { //move abs
        obj->Move(mergeu16(ptr[1], ptr[2]), mergeu16(ptr[3], ptr[4]), false);
        return ptr + 5;
    }
    if (ptr[0] == 0x02)
    { //move rel
        obj->Move(mergeu16(ptr[1], ptr[2]), mergeu16(ptr[3], ptr[4]), true);
        return ptr + 5;
    }
    if(ptr[0] == 0xFF){//sleep
        obj->cmd_delay_left = mergeu16(ptr[1],ptr[2]);
        return ptr + 3;
    }

    ret_ptr = HandleJumps(ptr, obj);
    if (ret_ptr > 0)
        return ret_ptr;

    ret_ptr = HandleLoads(ptr, obj);
    if (ret_ptr > 0)
        return ret_ptr;

    ret_ptr = HandleALU(ptr, obj);
    if (ret_ptr > 0)
        return ret_ptr;

    return ptr + 1;
}