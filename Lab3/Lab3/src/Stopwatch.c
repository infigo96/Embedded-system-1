#include "Stopwatch.h"

__attribute__((__interrupt__)) static void watch_interrupt(void)
{
	time++;
	tc_read_sr(&AVR32_TC, 0);
}


void SW_init(volatile avr32_tc_t *tc)
{
	//--------------------
	//Configure timer
	static tc_waveform_opt_t opt;
	unsigned int channel = 0; //Compiler complains if 0 is written directly
	opt.channel = channel;
	opt.wavsel = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER;
	opt.tcclks = TC_CLOCK_SOURCE_TC5; //CLK / 128
	tc_init_waveform(tc, &opt);
	tc_write_rc(tc,channel,9375); //12MHz / (128 * 9375) = 0.1 second between interrupts
	//Add timer to interrupts
	Disable_global_interrupt();
	INTC_init_interrupts();

	tc_interrupt_t bitfield;
	bitfield.cpcs = 1;
	INTC_register_interrupt(&watch_interrupt,AVR32_TC_IRQ0 ,AVR32_INTC_INT0); //Set time/-counter interrupts

	tc_configure_interrupts(tc,0,&bitfield);

	Enable_global_interrupt();
	//--------------------------------------
}
//Starts the stopwatch/clock interrupts
void SW_start(volatile avr32_tc_t *tc, unsigned int channel){
	tc_start(tc,channel);
}
//Stops the stopwatch/clock interrupts
void SW_stop(volatile avr32_tc_t *tc, unsigned int channel){
	tc_stop(tc,channel);
}