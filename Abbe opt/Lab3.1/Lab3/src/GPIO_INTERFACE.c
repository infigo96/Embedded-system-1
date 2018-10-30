/*
 * GPIO_INTERFACE.c
 *
 * Created: 2018-09-20 17:34:56
 *  Author: Albert
 */ 
#include "GPIO_INTERFACE.h"
void GPIO_init(){
	avr32_gpio_port_t *button_port = &AVR32_GPIO.port[BUTTON_PORT];
	button_port->gpers = BUTTON_PIN;
	button_port->oderc = BUTTON_PIN;
	
	avr32_gpio_port_t *joystick_port = &AVR32_GPIO.port[JOYSTICK_PORT];
	joystick_port->gpers = JOYSTICK_PIN;
	joystick_port->oderc = JOYSTICK_PIN;
}