#include "PC_tasks.h"

//Initiates LED 0, 1 and 2.
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

//Initiates Button 0, 1 and 2.
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

//Producer task. Sends bytes to the queue.
void Producer(void * pvParameters)
{
	task_struct *TS = (task_struct *)pvParameters;
	int byteCount;
	char byte = '0';
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		//Check if there is space in the queue (1 if there is space, 0 else) and if there is, write to it..
		if(xQueueSendToBack(Qhandle,&byte,0) == 1)
		{
			//Byte was written, now update to next byte (0, 1, 2, 3....)
			byte++;
			//Global variable that keeps track of absolute space in queue.
			nQueue++;
			if(nQueue >= (sizeQ-1))
			{
				if( xSemaphoreTake( TS->xSemaphore, ( portTickType ) 10) == pdFALSE )
				{
					vTaskResume(TS->cHandle);

				}
				else
				{
					xSemaphoreGive(TS->xSemaphore);
				}
				//writeUSART("Wake cons\r\n");
			}
		}
		else
		{
			
			if( xSemaphoreTake( TS->xSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{
				writeUSART_CRT("Queue is full, producer goes to sleep\r\n");
				//vTaskResume(cHandle);
				vTaskSuspend(NULL);
				writeUSART_CRT("producer woken \r\n");
				xSemaphoreGive(TS->xSemaphore);
			}
			//Update last wake time. 
			xLastWakeTime = xTaskGetTickCount();
		}
		//vTaskDelayUntil(&xLastWakeTime, 300);
	}
}

//Reads bytes from a queue.
void Consumer(void * pvParameters)
{
	task_struct *TS = (task_struct *)pvParameters;
	int byteCount;
	char byte[4];
	byte[1] = '\r';
	byte[2] = '\n';
	byte[3] = 0;
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		//Read from queue if there is stuff there.
		if(xQueueReceive(Qhandle,&(byte[0]),0) == 1)
		{
			//Update global
			nQueue--;
			//Write what was in the queue.
			writeUSART_CRT(&byte);
			
			if(nQueue <= (1))
			{
				if( xSemaphoreTake( TS->xSemaphore, ( portTickType ) 10) == pdFALSE )
				{
					vTaskResume(TS->pHandle);

				}
				else
				{
					xSemaphoreGive(TS->xSemaphore);
				}
				
			}
		}
		else
		{
			if( xSemaphoreTake( TS->xSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{
				writeUSART_CRT("Queue is empty, consumer goes to sleep\r\n");
				//vTaskResume(pHandle);
				vTaskSuspend(NULL);
				writeUSART_CRT("Consumer woken \r\n");
				xSemaphoreGive(TS->xSemaphore);
			}

			
			xLastWakeTime = xTaskGetTickCount();
		}
		//vTaskDelayUntil(&xLastWakeTime, 320);

		
	}
}