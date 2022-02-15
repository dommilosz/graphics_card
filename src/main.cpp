#include "consts.h"
#include "I2CCom.h"
I2CCom_Slave I2CCom(25,&Wire1);
#include "util.h"
#include "vga.cpp"
VGA vga;
cRandom rnd;
#include "assets.h"
#include "objects.h"
#include "objects.cpp"

#include "commands.h"

void draw_no_signal()
{
	vga.Init(RES_EGA, FORM_8BIT, 255);
}

void receiveEvent(int action_id)
{
  if (!I2CCom.receiveEvent()) {

  }
}

void requestEvent()
{
  if (!I2CCom.requestEvent()) {

  }
}


int main()
{
	stdio_init_all();
	memcpy(Font_Copy, FontBold8x8, sizeof(FontBold8x8));
	Objects::Init();
	vga.Allocate();
	PWMSndInit();
	//draw_no_signal();

	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	rnd.InitSeed();

	pinMode(26,INPUT_PULLUP);
	pinMode(27,INPUT_PULLUP);
	I2CCom.begin();
	I2CCom._wire->onReceive(receiveEvent);
	I2CCom._wire->onRequest(requestEvent);

	while (true)
	{
		u8 ready = I2CCom.ready();
		digitalWrite(LED_PIN, ready == 0);
		CommandRoutine(data_source_serial);
		Objects::ExecCode();
	}
}