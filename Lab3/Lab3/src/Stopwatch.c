#include "Stopwatch.h"

__attribute__((__interrupt__)) static void tc_irq_handler(void)
{
	AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE;
	time++;
	tc_read_sr(&AVR32_TC, 0);
}


void SW_init(volatile avr32_tc_t *tc)
{
	//--------------------
	static tc_waveform_opt_t opt;
	opt.channel = 0;
	opt.wavsel = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER;
	opt.tcclks = TC_CLOCK_SOURCE_TC2;
	int hej0 = tc_write_rc(tc,0,100);
	int hej1 = tc_init_waveform(tc, &opt);
	unsigned int hejsan = 0;
	int hej2 = tc_start(tc, hejsan);


	Disable_global_interrupt();
	INTC_init_interrupts();


	tc_interrupt_t bitfield;
	bitfield.cpcs = 1;
	INTC_register_interrupt(&tc_irq_handler,AVR32_TC_IRQ0 ,AVR32_INTC_INT0);

	int hej4 = tc_configure_interrupts(tc,0,&bitfield);

	Enable_global_interrupt();
	//--------------------------------------
}