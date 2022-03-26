enum data_source{
	data_source_serial=0,
	data_source_wire_input_focused = 1,
	data_source_wire_output_focused = 2,
};

data_source UTIL_DATA_SOURCE = data_source_serial;

u8 last_result = STATUS_UNDEFINED;
u8 cmd_buff[16];
u8 cmd_buff_ir = 0;
u8 cmd_buff_iw = 0;

int GetU8()
{
	if(UTIL_DATA_SOURCE == data_source_serial){
		return getchar_timeout_us(0);
	}else if(UTIL_DATA_SOURCE == data_source_wire_input_focused){
		if(I2CCom._wire->available()<1)return -2;
		return I2CCom._wire->read();
	}else if(UTIL_DATA_SOURCE == data_source_wire_output_focused){
		if(cmd_buff_ir>=cmd_buff_iw)return -2;
		u8 v = cmd_buff[cmd_buff_ir];
		cmd_buff_ir += 1;
		return v;
	}
	return 0;
}

void putu(u8 byte){
	if(UTIL_DATA_SOURCE == data_source_serial){
		printf("%c",byte);
	}else if(UTIL_DATA_SOURCE == data_source_wire_input_focused){
		last_result = byte;
	}else if(UTIL_DATA_SOURCE == data_source_wire_output_focused){
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
	for(byte i=0;i<100;i++){
		int b = GetU8();
		if(b>=0)return b;
		sleep_ms(10);
	}
	return 0;
}

u16 WaitForByte16(){
	u8 u81 = WaitForByte();
	u8 u82 = WaitForByte();
	return u81 << 8 | u82;
}

u32 WaitForByte32()
{
	u8 u81 = WaitForByte();
	u8 u82 = WaitForByte();
	u8 u83 = WaitForByte();
	u8 u84 = WaitForByte();
	return u81 << 24 | u82 << 16 | u83 << 8 | u84;
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
#define putu32(x) putu(x >> 24);putu(x >> 16 & 0xFF);putu(x >> 8 & 0xFF);putu(x & 0xFF)

#define set_sys_clock_mhz(x) set_sys_clock_khz(x*1000,true)

uint32_t crc32(const char *s,size_t n) {
	uint32_t crc=0xFFFFFFFF;
	
	for(size_t i=0;i<n;i++) {
		char ch=s[i];
		for(size_t j=0;j<8;j++) {
			uint32_t b=(ch^crc)&1;
			crc>>=1;
			if(b) crc=crc^0xEDB88320;
			ch>>=1;
		}
	}
	
	return ~crc;
}
