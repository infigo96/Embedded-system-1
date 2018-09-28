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
	for(;;) 
	{ 
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED0_PIN);
		writeUSART("Blink1 - Toggle LED0\r\n");
		vTaskDelay(1000);
	} 
		
}
void vBlinkLED2( void * pvParameters )
{
	for(;;)
	{ 
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED1_PIN); 
		writeUSART("Blink2 - Toggle LED1\r\n");
		vTaskDelay(2000);
	} 
		
}
void vBlinkLED3( void * pvParameters )
{
	while (1) 
	{ 
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED2_PIN);
		writeUSART("Blink3 - Toggle LED2\r\n");
		vTaskDelay(4000);
	}
}
void vReadButtons(void * pvParameters)
{
	unsigned int btn_state[3];
	unsigned int prev_btn_state[3];
	for(;;)
	{
		btn_state[0] = AVR32_GPIO.port[BUTTON_PORT].pvr & (1 << BUTTON0_PIN);
		btn_state[1] = AVR32_GPIO.port[BUTTON_PORT].pvr & (1 << BUTTON1_PIN);
		btn_state[2] = AVR32_GPIO.port[BUTTON_PORT].pvr & (1 << BUTTON2_PIN);
		if(btn_state[0]==0 && btn_state[0] != prev_btn_state[0])
		{
			writeUSART("Button - Resuming Light1\r\n");
			vTaskResume(((xTaskHandle*)pvParameters)[0]);
		}
		if(btn_state[1]==0 && btn_state[1] != prev_btn_state[1])
		{
			writeUSART("Button - Resuming Light2\r\n");
			vTaskResume(((xTaskHandle*)pvParameters)[1]);
		}
		if(btn_state[2]==0 && btn_state[2] != prev_btn_state[2])
		{
			writeUSART("Button - Resuming Light3\r\n");
			vTaskResume(((xTaskHandle*)pvParameters)[2]);
		}
		for (int i=0;i<3;i++)
		{
			prev_btn_state[i] = btn_state[i];
		}
		vTaskDelay(100);
	}
}	
void vLightLED1( void * pvParameters )
{
	for(;;)
	{
		vTaskSuspend(*((xTaskHandle*)pvParameters)); //Stop the blinking
		writeUSART("Light1 - Suspending Blink1\r\n");
		AVR32_GPIO.port[LED_PORT].ovrc = (1 << LED0_PIN); //Turn on the light
		vTaskDelay(10*1000); //Wait 10 sec
		writeUSART("Light1 - Resuming Blink1\r\n");
		vTaskResume(*((xTaskHandle*)pvParameters)); //Resume blinking
		vTaskSuspend(NULL); //Suspend this task
	}
}
void vLightLED2( void * pvParameters )
{
	for(;;)
	{
		vTaskSuspend(*((xTaskHandle*)pvParameters)); //Stop the blinking
		writeUSART("Light2 - Suspending Blink2\r\n");
		AVR32_GPIO.port[LED_PORT].ovrc = (1 << LED1_PIN); //Turn on the light
		vTaskDelay(10*1000); //Wait 10 sec
		writeUSART("Light2 - Resuming Blink2\r\n");
		vTaskResume(*((xTaskHandle*)pvParameters)); //Resume blinking
		vTaskSuspend(NULL); //Suspend this task
	}
}
void vLightLED3( void * pvParameters )
{
	for(;;)
	{
		vTaskSuspend(*((xTaskHandle*)pvParameters)); //Stop the blinking
		writeUSART("Light3 - Suspending Blink3\r\n");
		AVR32_GPIO.port[LED_PORT].ovrc = (1 << LED2_PIN); //Turn on the light
		vTaskDelay(10*1000); //Wait 10 sec
		writeUSART("Light3 - Resuming Blink3\r\n");
		vTaskResume(*((xTaskHandle*)pvParameters)); //Resume blinking
		vTaskSuspend(NULL); //Suspend this task
	}
}