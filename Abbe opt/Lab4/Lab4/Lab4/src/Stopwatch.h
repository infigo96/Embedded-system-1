#ifndef Stopwatch_H
#define Stopwatch_H
	# include "board.h"
	# include "compiler.h"
	#include "tc.h"
	#include "pwm.h"
	#include "avr32\uc3a0512.h"
	#include "display_init.h"
	#define ADRESS AVR32_PWM_ADDRESS
	#define PORT 1
	#define PWM_PIN AVR32_PIN_PB19
	#define FUN AVR32_PWM_0_FUNCTION
	#define itp32 (unsigned int*) //Used to convert to addresses
	
	unsigned int time; //Counts 10ths of seconds. Is incremented during watch_interrupt
	void SW_init(volatile avr32_tc_t *tc);
	void SW_start(volatile avr32_tc_t *tc, unsigned int channel);
	void SW_stop(volatile avr32_tc_t *tc, unsigned int channel);
	void PWM_init();
	void PWM_start(int channel);
	int readPIN(void);
#endif