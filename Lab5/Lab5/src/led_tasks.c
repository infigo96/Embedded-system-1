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
		writeUSART_CRT("Blink1 - Toggle LED0\r\n");
		vTaskDelay(1000);
	} 
		
}
void vBlinkLED2( void * pvParameters )
{
	for(;;)
	{ 
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED1_PIN); 
		writeUSART_CRT("Blink2 - Toggle LED1\r\n");
		vTaskDelay(2000);
	} 
		
}
void vBlinkLED3( void * pvParameters )
{
	while (1) 
	{ 
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED2_PIN);
		writeUSART_CRT("Blink3 - Toggle LED2\r\n");
		vTaskDelay(4000);
	}
}
void vReadButtons(void * pvParameters)
{
	unsigned int btn_state[3];
	unsigned int prev_btn_state[3];
	for(;;)
	{
		//Read the state of the buttons
		btn_state[0] = AVR32_GPIO.port[BUTTON_PORT].pvr & (1 << BUTTON0_PIN);
		btn_state[1] = AVR32_GPIO.port[BUTTON_PORT].pvr & (1 << BUTTON1_PIN);
		btn_state[2] = AVR32_GPIO.port[BUTTON_PORT].pvr & (1 << BUTTON2_PIN);
		//Only goes into the state the first time of a button press 
		if(btn_state[0]==0 && btn_state[0] != prev_btn_state[0])
		{
			writeUSART_CRT("Button - Resuming Light1\r\n");
			vTaskResume(((xTaskHandle*)pvParameters)[0]); //Resumes the task lightled1
		}
		if(btn_state[1]==0 && btn_state[1] != prev_btn_state[1])
		{
			writeUSART_CRT("Button - Resuming Light2\r\n");
			vTaskResume(((xTaskHandle*)pvParameters)[1]); //Resumes the task lightled2
		}
		if(btn_state[2]==0 && btn_state[2] != prev_btn_state[2])
		{
			writeUSART_CRT("Button - Resuming Light3\r\n");
			vTaskResume(((xTaskHandle*)pvParameters)[2]); //Resumes the task lightled3
		}
		//Save previous button state
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
		writeUSART_CRT("Light1 - Suspending Blink1\r\n");
		AVR32_GPIO.port[LED_PORT].ovrc = (1 << LED0_PIN); //Turn on the light
		vTaskDelay(10*1000); //Wait 10 sec
		writeUSART_CRT("Light1 - Resuming Blink1\r\n");
		vTaskResume(*((xTaskHandle*)pvParameters)); //Resume blinking
		vTaskSuspend(NULL); //Suspend this task
	}
}
void vLightLED2( void * pvParameters )
{
	for(;;)
	{
		vTaskSuspend(*((xTaskHandle*)pvParameters)); //Stop the blinking
		writeUSART_CRT("Light2 - Suspending Blink2\r\n");
		AVR32_GPIO.port[LED_PORT].ovrc = (1 << LED1_PIN); //Turn on the light
		vTaskDelay(10*1000); //Wait 10 sec
		writeUSART_CRT("Light2 - Resuming Blink2\r\n");
		vTaskResume(*((xTaskHandle*)pvParameters)); //Resume blinking
		vTaskSuspend(NULL); //Suspend this task
	}
}
void vLightLED3( void * pvParameters )
{
	for(;;)
	{
		vTaskSuspend(*((xTaskHandle*)pvParameters)); //Stop the blinking
		writeUSART_CRT("Light3 - Suspending Blink3\r\n");
		AVR32_GPIO.port[LED_PORT].ovrc = (1 << LED2_PIN); //Turn on the light
		vTaskDelay(10*1000); //Wait 10 sec
		writeUSART_CRT("Light3 - Resuming Blink3\r\n");
		vTaskResume(*((xTaskHandle*)pvParameters)); //Resume blinking
		vTaskSuspend(NULL); //Suspend this task
	}
}

void writeUSART_CRT(const char * message){
	//Prevent task writing over each other by entering critical state during writing
	taskENTER_CRITICAL();
	writeUSART(message);
	taskEXIT_CRITICAL();
}