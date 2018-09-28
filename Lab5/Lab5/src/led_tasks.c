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
void initBUTTON()
{
	volatile avr32_gpio_port_t *button_port;
	button_port = & AVR32_GPIO.port[BUTTON_PORT];
	button_port->gpers = 1 << BUTTON0_PIN;
	button_port->oderc = 1 << BUTTON0_PIN;
	button_port->gpers = 1 << BUTTON1_PIN;
	button_port->oderc = 1 << BUTTON1_PIN;
	button_port->gpers = 1 << BUTTON2_PIN;
	button_port->oderc = 1 << BUTTON2_PIN;
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
void vReadButtons(void * pvParameters)
{
	
	for(;;)
	{
		if((AVR32_GPIO.port[BUTTON_PORT].pvr & (1 << BUTTON0_PIN))==0)
		{
			vTaskResume(xTaskHandle_light1);
		}
		if((AVR32_GPIO.port[BUTTON_PORT].pvr & (1 << BUTTON1_PIN))==0)
		{
			vTaskResume(xTaskHandle_light2);
		}
		if((AVR32_GPIO.port[BUTTON_PORT].pvr & (1 << BUTTON2_PIN))==0)
		{
			vTaskResume(xTaskHandle_light3);
		}
		vTaskDelay(100);
	}
}	
void vLightLED1( void * pvParameters )
{
	for(;;)
	{
		vTaskSuspend(xTaskHandle_blink1); //Stop the blinking
		AVR32_GPIO.port[LED_PORT].ovrc = (1 << LED0_PIN); //Turn on the light
		vTaskDelay(10*1000); //Wait 10 sec
		vTaskResume(xTaskHandle_blink1); //Resume blinking
		vTaskSuspend(NULL); //Suspend this task
	}
}
void vLightLED2( void * pvParameters )
{
	for(;;)
	{
		vTaskSuspend(xTaskHandle_blink2); //Stop the blinking
		AVR32_GPIO.port[LED_PORT].ovrc = (1 << LED1_PIN); //Turn on the light
		vTaskDelay(10*1000); //Wait 10 sec
		vTaskResume(xTaskHandle_blink2); //Resume blinking
		vTaskSuspend(NULL); //Suspend this task
	}
}
void vLightLED3( void * pvParameters )
{
	for(;;)
	{
		vTaskSuspend(xTaskHandle_blink3); //Stop the blinking
		AVR32_GPIO.port[LED_PORT].ovrc = (1 << LED2_PIN); //Turn on the light
		vTaskDelay(10*1000); //Wait 10 sec
		vTaskResume(xTaskHandle_blink3); //Resume blinking
		vTaskSuspend(NULL); //Suspend this task
	}
}