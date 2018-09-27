/*
 * led_tasks.c
 *
 * Created: 2018-09-27 11:19:40
 *  Author: Albert
 */ 
#include "led_tasks.h"
void initLED()
{
	volatile avr32_gpio_port_t *led_port;
	led_port = & AVR32_GPIO.port[LED_PORT];
	
	led_port->gpers = LED0_PIN;
	led_port->gpers = LED1_PIN;
	led_port->gpers = LED2_PIN;

	led_port->ovrs = LED0_PIN;
	led_port->ovrs = LED1_PIN;
	led_port->ovrs = LED2_PIN;

	led_port->oders = LED0_PIN;
	led_port->oders = LED1_PIN;
	led_port->oders = LED2_PIN;
}
void vBlinkLED1( void * pvParameters )
{
	
	while (1)
	{
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED0_PIN);
		vTaskDelay( 100 );
	}
		
}
