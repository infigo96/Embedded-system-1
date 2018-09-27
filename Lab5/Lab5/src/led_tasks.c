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
	
	led_port->gpers = 1 << LED0_PIN;
	led_port->gpers = 1 << LED1_PIN;
	led_port->gpers = 1 << LED2_PIN;

	led_port->ovrc = 1 << LED0_PIN;
	led_port->ovrc = 1 << LED1_PIN;
	led_port->ovrc = 1 << LED2_PIN;

	led_port->oders = 1 << LED0_PIN;
	led_port->oders = 1 << LED1_PIN;
	led_port->oders = 1 << LED2_PIN;
}

void vBlinkLED1( void * pvParameters )
{
	unsigned short * pxPreviousWakeTime1 = xTaskGetTickCount(); 
	portTickType xTimeIncrement1 = 1000;
	for(;;) 
	{ 
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED0_PIN); 
		//vTaskDelayUntil(pxPreviousWakeTime1, xTimeIncrement1); 
		vTaskDelay(1000);
	} 
		
}
void vBlinkLED2( void * pvParameters )
{
	unsigned short * pxPreviousWakeTime = xTaskGetTickCount(); 
	portTickType xTimeIncrement = 2000;
	for(;;)
	{ 
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED1_PIN); 
		//vTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement); 
		vTaskDelay(2000);
	} 
		
}
void vBlinkLED3( void * pvParameters )
{
	unsigned short *pxPreviousWakeTime; 
	const unsigned short xTimeIncrement = 3000;
	while (1) 
	{ 
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED2_PIN); 
		vTaskDelay(4000);
	} 
		
}