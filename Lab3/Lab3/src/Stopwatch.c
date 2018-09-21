#include "Stopwatch.h"

__attribute__((__interrupt__)) static void watch_interrupt(void)
{
	/*unsigned int hej = AVR32_GPIO.port[BUTTON0_PORT].pvr & BUTTON0_PIN;
	if(hej == 0 && lockdown == 0)
	{
		firstPress++;
		presses[0]++;
	}
	else if(firstPress != 0)
	{
		presses[1]++;
	}*/
	time++;
	tc_read_sr(&AVR32_TC, 0);
}


void SW_init(volatile avr32_tc_t *tc)
{
	//--------------------
	static tc_waveform_opt_t opt;
	unsigned int channel = 0; //Compiler complains if 0 is written directly
	opt.channel = channel;
	opt.wavsel = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER;
	opt.tcclks = TC_CLOCK_SOURCE_TC5;
	tc_init_waveform(tc, &opt);
	tc_write_rc(tc,channel,56250);

	Disable_global_interrupt();
	INTC_init_interrupts();

	tc_interrupt_t bitfield;
	bitfield.cpcs = 1;
	INTC_register_interrupt(&watch_interrupt,AVR32_TC_IRQ0 ,AVR32_INTC_INT0);

	tc_configure_interrupts(tc,0,&bitfield);

	Enable_global_interrupt();
	//--------------------------------------
}