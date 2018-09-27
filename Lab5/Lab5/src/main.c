
#include "led_tasks.h"
#include "tc.h"

int main()
{
	initLED();
	AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED0_PIN);
	static unsigned char ucParameterToPass;
	
	//Configure timer
	static tc_waveform_opt_t opt;
	volatile avr32_tc_t *tc = &AVR32_TC;
	unsigned int channel = 0; //Compiler complains if 0 is written directly
	opt.channel = channel;
	opt.wavsel = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER;
	opt.tcclks = TC_CLOCK_SOURCE_TC2; //CLK / 128
	tc_init_waveform(tc, &opt);
	tc_write_rc(tc,channel,115); //12MHz / (128 * 9375) = 0.1 second between interrupts

	AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED0_PIN);

	xTaskHandle xHandle;
	xTaskCreate(vBlinkLED1,"Blink1",configMINIMAL_STACK_SIZE,&ucParameterToPass,tskIDLE_PRIORITY + 1,&xHandle);

	AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED0_PIN);
	
	vTaskStartScheduler();
	
	return 0;
}