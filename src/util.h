enum data_source{
	data_source_serial=0,
	data_source_wire = 1,
};

data_source UTIL_DATA_SOURCE = data_source_serial;


int GetU8()
{
	if(UTIL_DATA_SOURCE == data_source_serial){
		return getchar_timeout_us(0);
	}else if(UTIL_DATA_SOURCE == data_source_wire){
		if(I2CCom._wire->available()<1)return -2;
		return I2CCom._wire->read();
	}
	return 0;
}

void putu(u8 byte){
	if(UTIL_DATA_SOURCE == data_source_serial){
		printf("%c",byte);
	}else if(UTIL_DATA_SOURCE == data_source_wire){
		I2CCom._wire->write(byte);
	}
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

u8 WaitForByte(){
	for(byte i=0;i<10;i++){
		int b = GetU8();
		if(b>=0)return b;
		sleep_ms(10);
	}
	return 0;
}

void ReadBuffer(u8 *buf, u32 len)
{
	for (u32 i = 0; i < len; i++)
	{
		buf[i] = WaitForByte();
	}
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

#define Status(x) putu(x);return
#define putu16(x) putu(x >> 8);putu(x & 0x00FF)

#define set_sys_clock_mhz(x) set_sys_clock_khz(x*1000,true)