#include "Stopwatch.h"

__attribute__((__interrupt__)) static void watch_interrupt(void)
{
	//buttin[0] = AVR32_GPIO.port[BUTTON0_PORT].pvr;
	buttin[0] = hej->pvr & BUTTON0_PIN;
	buttin[1] = hej->pvr & BUTTON0_PIN;
	buttin[2] = hej->pvr & BUTTON0_PIN;
	buttin[3] = hej->pvr & BUTTON0_PIN;
	buttin[4] = hej->pvr & BUTTON0_PIN;
	buttin[5] = hej->pvr & BUTTON0_PIN;
	buttin[6] = hej->pvr & BUTTON0_PIN;
	if(buttin[6] == 0)
	{
		tc_stop(&AVR32_TC,0);
		lockdown = 1;
	}
	tc_read_sr(&AVR32_TC, 0);
}


void SW_init(volatile avr32_tc_t *tc)
{
	//--------------------
	static tc_waveform_opt_t opt;
	unsigned int channel = 0; //Compiler complains if 0 is written directly
	opt.channel = channel;
	opt.wavsel = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER;
	opt.tcclks = TC_CLOCK_SOURCE_TC2;
	tc_init_waveform(tc, &opt);
	tc_write_rc(tc,channel,10);

	Disable_global_interrupt();
	INTC_init_interrupts();

	tc_interrupt_t bitfield;
	bitfield.cpcs = 1;
	INTC_register_interrupt(&watch_interrupt,AVR32_TC_IRQ0 ,AVR32_INTC_INT0);

	tc_configure_interrupts(tc,0,&bitfield);

	Enable_global_interrupt();
	//--------------------------------------
}