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
void PWM_init()
{
	volatile avr32_pm_t* pm = &AVR32_PM;
	// Switch clock to external 12 MHz crystal
	// 1) Configure OSC0 in crystal mode, external crystal with a FOSC0 Hz frequency.
	pm_enable_osc0_crystal(pm, FOSC0);

	// 2) Enable the OSC0
	pm_enable_clk0(pm, OSC0_STARTUP);

	// 3) Set the main clock source as being OSC0.
	pm_switch_to_clock(pm, AVR32_PM_MCSEL_OSC0);
	static pwm_opt_t opt;
	volatile avr32_pwm_t *pwm = &AVR32_PWM;
	opt.prea = 1;
	opt.diva = 1;
	pwm_init(&opt);
	
	

}
void PWM_start(int channel)
{
	avr32_pwm_channel_t pwm_c;
	pwm_c.cdty = 50;
	pwm_c.cprd = 100;
	int i;
	i = pwm_channel_init(0,&pwm_c);
	i = pwm_start_channels(1 << channel);
}

//Starts the stopwatch/clock interrupts
void SW_start(volatile avr32_tc_t *tc, unsigned int channel){
	tc_start(tc,channel);
}
//Stops the stopwatch/clock interrupts
void SW_stop(volatile avr32_tc_t *tc, unsigned int channel){
	tc_stop(tc,channel);
}
int readPIN(void)
{
	return *(itp32(ADRESS));
}