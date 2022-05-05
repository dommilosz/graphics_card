#include "consts.h"
#include "I2CCom.h"
I2CCom_Slave I2CCom(30, &Wire1);
#include "util.h"
#include "vga.cpp"
VGA vga;
cRandom rnd;
#include "assets.h"
#include "objects.h"
#include "objects.cpp"
#include "sound.h"
#include "commands.h"
#include "PicoEasyReflash.h"

void draw_no_signal()
{
	vga.AutoInit(1);
	vga.initialized = false;
	DrawRect(&Canvas, 80, 70, 140, 30, 224);
	DrawText(&Canvas, "Not initialised", 90, 80, 255, &Font_Copy);
}

void receiveEvent(int len)
{
	if (!I2CCom.receiveEvent())
	{
		uint8_t action_id = I2CCom.data_req;
		if (action_id == 1)
		{
			CommandRoutine(data_source_wire_input_focused);
			//Wire1.write(0);
		}
		if (action_id == 2)
		{
			cmd_buff_iw = 0;
			cmd_buff_ir = 0;
			while (Wire1.available() > 0)
			{
				cmd_buff[cmd_buff_iw] = Wire1.read();
				cmd_buff_iw++;
				if (cmd_buff_iw >= 16)
					break;
			}
		}
	}
}

void requestEvent()
{
	if (!I2CCom.requestEvent())
	{
		uint8_t action_id = I2CCom.data_req;
		if (action_id == 1)
		{
			Wire1.write(last_result);
		}
		if (action_id == 2)
		{
			CommandRoutine(data_source_wire_output_focused);
		}
	}
}

int main()
{
	stdio_init_all();
	memcpy(Font_Copy, FontBold8x8, sizeof(FontBold8x8));
	Objects::Init();
	vga.Allocate();
	draw_no_signal();
	rnd.InitSeed();

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(26, INPUT_PULLUP);
	pinMode(27, INPUT_PULLUP);
	I2CCom.begin();
	I2CCom._wire->onReceive(receiveEvent);
	I2CCom._wire->onRequest(requestEvent);

	PWMSndInit();

	digitalWrite(LED_BUILTIN, HIGH);
	delay(100);
	digitalWrite(LED_BUILTIN, LOW);
	delay(100);
	digitalWrite(LED_BUILTIN, HIGH);
	delay(100);
	digitalWrite(LED_BUILTIN, LOW);
	delay(100);

	watchdog_enable(3000, 1);
	while (true)
	{
		long start = micros();
		watchdog_update();
		u8 ready = I2CCom.ready();
		u16 leddata = millis() % 1000;
		if (ready == 0)
		{
			digitalWrite(LED_BUILTIN, leddata > 100);
			digitalWrite(13, true);
		}
		else
		{
			digitalWrite(LED_BUILTIN, leddata > 700);
			digitalWrite(13, false);
		}
		//TickEasyReflash();
		CommandRoutine(data_source_serial);
		Objects::ExecCode();
		long end = micros();
		//printf("loop: %u",end-start);
	}
}