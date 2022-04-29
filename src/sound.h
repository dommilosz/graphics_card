#define PWMSND_GPIO 19						  // PWM output GPIO pin (0..29)
#define PWMSND_SLICE ((PWMSND_GPIO >> 1) & 7) // PWM slice index (=1)
#define PWMSND_CHAN (PWMSND_GPIO & 1)		  // PWM channel index (=1)

#define SOUNDRATE 22050								// sound rate [Hz]
#define PWMSND_TOP 255								// PRM top (period = PWMSND_TOP + 1 = 256)
#define PWMSND_CLOCK (SOUNDRATE * (PWMSND_TOP + 1)) // PWM clock (= 22050*256 = 5644800)

#define SNDFRAC 10			  // number of fraction bits
#define SNDINT (1 << SNDFRAC) // integer part of sound fraction

u16 CurFrequency = 0; // current playing sound
u8 CurAsset = 0;	  //current asset of sound.
u16 CurAssetIndex = 0;
int SoundEnd = 0;
u8 SoundType = 0;
int SoundIndex = 0;
bool repeatAsset = false;
u8 last_sample = 0;

u8 GetSoundSample(u16 position)
{
	u8 sample = ((128 * sin(2 * PI * CurFrequency / 22050 * position))+128);
	int diff = sample-last_sample;
	if(diff < 0)diff = -diff;

	if(diff > 5){
		if(last_sample > sample){
			sample = last_sample - 5;
		}else{
			sample = last_sample + 5;
		}
	}

	return sample;
}

bool FetchSoundFromAsset(u8 asset, u16 startPos = 0)
{
	u16 pos = startPos;
	CurFrequency = Read2Asset(CurAsset, pos);
	pos += 2;
	u16 duration = Read2Asset(CurAsset, pos);
	pos += 2;
	SoundType = ReadAsset(CurAsset, pos);
	pos++;
	SoundEnd = 0;
	if (duration < 1)
	{
		if(repeatAsset && startPos != 0){
			return FetchSoundFromAsset(asset,0);
		}
		CurAsset = 0;
		CurAssetIndex = 0;
		return false;
	}
	SoundEnd = millis() + duration;
	CurAssetIndex = pos;
	return true;
}

void PlaySoundAsset(u8 asset, bool repeat, u16 startPos = 0)
{
	repeatAsset = repeat;
	SoundEnd = 0;
	CurAsset = asset;
	CurAssetIndex = 0;
	FetchSoundFromAsset(asset,startPos);
}

// check if playing sound
Bool PlayingSound()
{
	return SoundEnd > millis();
}

// PWM sound interrupt service
void PWMSndIrq()
{
	// clear interrupt request
	pwm_clear_irq(PWMSND_SLICE);
	
	u8 smp = 0;
	// check if sound is playing
	if (PlayingSound())
	{
		smp = GetSoundSample(SoundIndex);
		SoundIndex++;
	}
	else
	{
		if (CurAsset > 0)
		{
			FetchSoundFromAsset(CurAsset, CurAssetIndex);
		}
	}

	last_sample = smp;
	((u16 *)&pwm_hw->slice[PWMSND_SLICE].cc)[PWMSND_CHAN] = smp;
}

// initialize PWM sound output
//  GP19 ... MOSI + sound output (PWM1 B)
void PWMSndInit()
{
	// set GPIO function to PWM
	gpio_set_function(PWMSND_GPIO, GPIO_FUNC_PWM);

	// set IRQ handler
	SoundIndex = 0;
	pwm_clear_irq(PWMSND_SLICE);
	pwm_set_irq_enabled(PWMSND_SLICE, true);
	irq_set_exclusive_handler(PWM_IRQ_WRAP, PWMSndIrq);
	irq_set_enabled(PWM_IRQ_WRAP, true);

	// get PWM default config
	pwm_config cfg = pwm_get_default_config();

	// set clock divider (INT = 0..255, FRAC = 1/16..15/16)
	//  125 MHz: 125000000/5644800 = 22.144, INT=22, FRAC=2,
	//     real sample rate = 125000000/(22+2/16)/256 = 22069Hz
	pwm_config_set_clkdiv(&cfg, (float)clock_get_hz(clk_sys) / PWMSND_CLOCK + 0.03f); // 0.03f = rounding 0.5/16

	// set period to 256 cycles
	pwm_config_set_wrap(&cfg, PWMSND_TOP);

	// start PWM
	pwm_init(PWMSND_SLICE, &cfg, True);
}

// output PWM sound (sound must be PCM 8-bit mono 22050Hz)
//  snd = pointer to sound
//  len = length of sound in number of samples
//  speed = relative speed (1=normal)
//  rep = True to repeat sample

// stop playing sound
void StopSound()
{
	__dmb();
	SoundIndex = 0;
	SoundEnd = 0;
	__dmb();
}

void PlaySoundFreq(u16 freq, u16 duration, u8 type)
{
	SoundType = type;
	SoundIndex = 0;
	SoundEnd = millis() + duration;
	CurFrequency = freq;
}