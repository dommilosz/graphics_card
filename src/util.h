u8 GetU8()
{
	return getchar_timeout_us(100000);
}

u16 GetU16()
{
	u8 u81 = GetU8();
	u8 u82 = GetU8();
	return u81 << 8 | u82;
}

u32 GetU32()
{
	u8 u81 = GetU8();
	u8 u82 = GetU8();
	u8 u83 = GetU8();
	u8 u84 = GetU8();
	return u81 << 24 | u82 << 16 | u83 << 8 | u84;
}

void ReadBuffer(u8 *buf, u32 len)
{
	for (u32 i = 0; i < len; i++)
	{
		buf[i] = GetU8();
	}
}

char GetChar()
{
	char c = getchar_timeout_us(0);
	if (c == (char)PICO_ERROR_TIMEOUT)
		c = 0;
	if ((c >= 'a') && (c <= 'z'))
		c -= 32;
	return c;
}

bool GetBit(u8 byte, u8 bit)
{
	return byte >> bit & 1;
}

bool GetBit(u16 byte, u8 bit)
{
	return byte >> bit & 1;
}

bool GetBit(u32 byte, u8 bit)
{
	return byte >> bit & 1;
}

char *GetLine(char *c){
	int i = 0;
	while(c[i]!=10){
		if(c[i] == 0)return 0;
		i++;
	}
	c[i] = 0;
	return c+i+1;
}

#define mergeu16(x,y) ((x << 8) | y)

