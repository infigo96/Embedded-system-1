#include "PC_tasks.h"

void initLED()
{
	volatile avr32_gpio_port_t *led_port;
	led_port = & AVR32_GPIO.port[LED_PORT];
	
	led_port->gpers = 1 << LED0_PIN;
	led_port->gpers = 1 << LED1_PIN;
	led_port->gpers = 1 << LED2_PIN;

	led_port->ovrs = 1 << LED0_PIN;
	led_port->ovrs = 1 << LED1_PIN;
	led_port->ovrs = 1 << LED2_PIN;

	led_port->oders = 1 << LED0_PIN;
	led_port->oders = 1 << LED1_PIN;
	led_port->oders = 1 << LED2_PIN;
}


void initBUTTON(void)
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


//Calls writeUsart(const char * message) from a critical state to prvent message overlapping.
void writeUSART_CRT(const char * message)
{
	//Prevent task writing over each other by entering critical state during writing
	taskENTER_CRITICAL();
	writeUSART(message);
	taskEXIT_CRITICAL();
}

void Producer(void * pvParameters)
{
	int byteCount;
	char byte = '0';
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		if(xQueueSendToBack(Qhandle,&byte,0) == 1)
		{
			byte++;
		}
		else
		{
			writeUSART_CRT("Queue is full \r\n");
			vTaskSuspend(NULL);
			xLastWakeTime = xTaskGetTickCount();
		}
		vTaskDelayUntil(&xLastWakeTime, 1000);
	}
}

void Consumer(void * pvParameters)
{
	int byteCount;
	char byte[2];
	byte[2] = 0;
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		if(xQueueReceive(Qhandle,&(byte[0]),10) == 1)
		{
			writeUSART_CRT(&byte);
			vTaskDelayUntil(&xLastWakeTime, 1500);
		}
		
	}
}