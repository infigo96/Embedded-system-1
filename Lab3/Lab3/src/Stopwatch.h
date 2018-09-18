#ifndef Stopwatch_H
#define Stopwatch_H
	# include "board.h"
	# include "compiler.h"
	#include "tc.h"
	#include "avr32\uc3a0512.h"
	extern unsigned int time; //Counts 10ths of seconds. Is incremented during watch_interrupt
	void SW_init(volatile avr32_tc_t *tc);
	
	/* Define the ports and pins to be used */
	// The maximum number of pins in a port
	# define GPIO_MAX_PIN_NUMBER 32

	// This is the port which LED0_GPIO is located on
	# define LED0_PORT ( LED0_GPIO / GPIO_MAX_PIN_NUMBER )
	// This is the bit position of the GPIO pin
	# define LED0_PIN ( LED0_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
	// This is a 1 - bit written to the bit position of the GPIO pin
	# define LED0_BIT_VALUE (1 << LED0_PIN )

	// This is the port which LED1_GPIO is located on
	# define LED1_PORT ( LED1_GPIO / GPIO_MAX_PIN_NUMBER )
	// This is the bit position of the GPIO pin
	# define LED1_PIN ( LED1_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
	// This is a 1 - bit written to the bit position of the GPIO pin
	# define LED1_BIT_VALUE (1 << LED1_PIN )
	
	
#endif