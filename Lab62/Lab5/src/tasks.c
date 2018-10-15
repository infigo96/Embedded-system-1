#include "tasks.h"
#include "dip204.h"
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
	char message[60];
	for(;;)
	{
			//---------------display-------------Print every row separate
		if( xSemaphoreTake( xSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			if( xSemaphoreTake( xSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{
				USART_getString(message,60);
				dip204_set_cursor_position(1,1);
				dip204_printf_string(message);
			}
		}
		
		/*if(xQueueSendToBack(Qhandle,&byte,0) == 1)
		{
			byte++;
			nQueue++;
			if(nQueue >= (sizeQ-1))
			{
				vTaskResume(cHandle);
				//writeUSART("Wake cons\r\n");
			}
		}
		else
		{
			writeUSART("Queue is full, producer goes to sleep\r\n");
			//vTaskResume(cHandle);
			vTaskSuspend(NULL);
			writeUSART("producer woken \r\n");
			xLastWakeTime = xTaskGetTickCount();
		}*/
		//vTaskDelayUntil(&xLastWakeTime, 300);
	}
}

void Consumer(void * pvParameters)
{
	int byteCount;
	char byte[2];
	byte[2] = 0;
	//vTaskDelay(900);
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		if(xQueueReceive(Qhandle,&(byte[0]),0) == 1)
		{
			writeUSART(&byte);
			nQueue--;
			if(nQueue <= (1))
			{
				vTaskResume(pHandle);
				//writeUSART("Wake prod\r\n");

			}
		}
		else
		{
			writeUSART("Queue is empty, consumer goes to sleep\r\n");
			//vTaskResume(pHandle);
			vTaskSuspend(NULL);
			writeUSART("Consumer woken \r\n");
			xLastWakeTime = xTaskGetTickCount();
		}
		vTaskDelayUntil(&xLastWakeTime, 320);

		
	}
}