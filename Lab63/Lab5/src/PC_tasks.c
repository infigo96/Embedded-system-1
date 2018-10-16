#include "PC_tasks.h"

//Initiates LED 0, 1 and 2.
void PrintLight(unsigned int i){
	char temp[60];
	sprintf(temp,"Light value:%d",i);
	dip204_set_cursor_position(1,1);
	dip204_write_string("                    ");
	dip204_set_cursor_position(1,1);
	dip204_write_string(temp);
	writeUSART_CRT(temp);
}
void PrintPot(unsigned int i){
	char temp[60];
	sprintf(temp,"PotValue:%d",i);
	dip204_set_cursor_position(1,2);
	dip204_write_string("                    ");
	dip204_set_cursor_position(1,2);
	dip204_write_string(temp);
	writeUSART_CRT(temp);

}
void PrintTemp(unsigned int i){
	char temp[60];
	sprintf(temp,"TempValue:%d",i);
	dip204_set_cursor_position(1,3);
	dip204_write_string("                    ");
	dip204_set_cursor_position(1,3);
	dip204_write_string(temp);
	writeUSART_CRT(temp);

}
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
void LightProducer(void * pvParameters)
{
	Task_Info *TI = (Task_Info *) pvParameters;
	task_struct *TS = TI->Ts;
	int byteCount = TI->task_nr;
	char byte = 48+(TI->task_nr);
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xSemaphoreTake( GloTranSemaphore,  ( portTickType ) portMAX_DELAY);
		//Check if there is space in the queue (1 if there is space, 0 else) and if there is, write to it..
		if(xQueueSendToBack(Qhandle,&byteCount,0) == 1)
		{
			xSemaphoreGive( GloTranSemaphore );			
			xSemaphoreTake( GloQueueSemaphore,  ( portTickType ) portMAX_DELAY);
			//Global variable that keeps track of absolute space in queue.
			nQueue++;
			xSemaphoreGive( GloQueueSemaphore );
			//Byte was written, now update to next byte (0, 1, 2, 3....)
			//byte++;
			vTaskDelay(10);

			if(nQueue >= (sizeQ-1))
			{
				if( xSemaphoreTake( xSuspSemaphore, ( portTickType ) 1) == pdFALSE )
				{
					for(int i = 0; i < nrCons; i++)
					{
						vTaskResume(TS->cHandle[i]);
					}

				}
				else
				{
					xSemaphoreGive(xSuspSemaphore);
				}
				//writeUSART("Wake cons\r\n");
			}
		}
		else
		{
			xSemaphoreGive( GloTranSemaphore );
			if( xSemaphoreTake( xSuspSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{
				//writeUSART_CRT("Queue is full, producer goes to sleep\r\n");
				//vTaskResume(cHandle);
				vTaskSuspend(NULL);
				//writeUSART_CRT("producer woken \r\n");
				xSemaphoreGive(xSuspSemaphore);
			}
			//Update last wake time. 
			xLastWakeTime = xTaskGetTickCount();
		}
		//vTaskDelayUntil(&xLastWakeTime, 300);
	}
}
void TempProducer(void * pvParameters)
{
	Task_Info *TI = (Task_Info *) pvParameters;
	task_struct *TS = TI->Ts;
	int byteCount = TI->task_nr;
	char byte = 48+(TI->task_nr);
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xSemaphoreTake( GloTranSemaphore,  ( portTickType ) portMAX_DELAY);
		//Check if there is space in the queue (1 if there is space, 0 else) and if there is, write to it..
		if(xQueueSendToBack(Qhandle,&byteCount,0) == 1)
		{
			xSemaphoreGive( GloTranSemaphore );
			xSemaphoreTake( GloQueueSemaphore,  ( portTickType ) portMAX_DELAY);
			//Global variable that keeps track of absolute space in queue.
			nQueue++;
			xSemaphoreGive( GloQueueSemaphore );
			//Byte was written, now update to next byte (0, 1, 2, 3....)
			//byte++;
			vTaskDelay(10);

			if(nQueue >= (sizeQ-1))
			{
				if( xSemaphoreTake( xSuspSemaphore, ( portTickType ) 1) == pdFALSE )
				{
					for(int i = 0; i < nrCons; i++)
					{
						vTaskResume(TS->cHandle[i]);
					}

				}
				else
				{
					xSemaphoreGive(xSuspSemaphore);
				}
				//writeUSART("Wake cons\r\n");
			}
		}
		else
		{
			xSemaphoreGive( GloTranSemaphore );
			if( xSemaphoreTake( xSuspSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{
				//writeUSART_CRT("Queue is full, producer goes to sleep\r\n");
				//vTaskResume(cHandle);
				vTaskSuspend(NULL);
				//writeUSART_CRT("producer woken \r\n");
				xSemaphoreGive(xSuspSemaphore);
			}
			//Update last wake time.
			xLastWakeTime = xTaskGetTickCount();
		}
		//vTaskDelayUntil(&xLastWakeTime, 300);
	}
}
void PotProducer(void * pvParameters)
{
	Task_Info *TI = (Task_Info *) pvParameters;
	task_struct *TS = TI->Ts;
	int byteCount = TI->task_nr;
	char byte = 48+(TI->task_nr);
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xSemaphoreTake( GloTranSemaphore,  ( portTickType ) portMAX_DELAY);
		//Check if there is space in the queue (1 if there is space, 0 else) and if there is, write to it..
		adc_start(&AVR32_ADC); // Start a ADC sampling of all active channels
		byteCount = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
		if(xQueueSendToBack(Qhandle,&byteCount,0) == 1)
		{
			xSemaphoreGive( GloTranSemaphore );
			xSemaphoreTake( GloQueueSemaphore,  ( portTickType ) portMAX_DELAY);
			//Global variable that keeps track of absolute space in queue.
			nQueue++;
			xSemaphoreGive( GloQueueSemaphore );
			//Byte was written, now update to next byte (0, 1, 2, 3....)
			//byte++;
			vTaskDelay(50);
			
			if(nQueue >= (sizeQ-1))
			{
				if( xSemaphoreTake( xSuspSemaphore, ( portTickType ) 1) == pdFALSE )
				{
					for(int i = 0; i < nrCons; i++)
					{
						vTaskResume(TS->cHandle[i]);
					}

				}
				else
				{
					xSemaphoreGive(xSuspSemaphore);
				}
				//writeUSART("Wake cons\r\n");
			}
		}
		else
		{
			xSemaphoreGive( GloTranSemaphore );
			if( xSemaphoreTake( xSuspSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{
				//writeUSART_CRT("Queue is full, producer goes to sleep\r\n");
				//vTaskResume(cHandle);
				vTaskSuspend(NULL);
				//writeUSART_CRT("producer woken \r\n");
				xSemaphoreGive(xSuspSemaphore);
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
	Task_Info *TI = (Task_Info *) pvParameters;
	task_struct *TS = TI->Ts;
	int byteCount;
	char byte[4];
	byte[0] = 'a';
	byte[1] = '\r';
	byte[2] = '\n';
	byte[3] = 0;
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xSemaphoreTake( GloReadSemaphore,  ( portTickType ) portMAX_DELAY);
		//Read from queue if there is stuff there.
		if(xQueueReceive(Qhandle,&byteCount,0) == 1)
		{
			xSemaphoreGive( GloReadSemaphore );
			xSemaphoreTake( GloQueueSemaphore,  ( portTickType ) portMAX_DELAY);
			//Update global
			nQueue--;
			xSemaphoreGive( GloQueueSemaphore );
			//Write what was in the queue.
			PrintPot(byteCount);
			
			
			if(nQueue <= (1))
			{
				if( xSemaphoreTake( xSuspSemaphore, ( portTickType ) 10) == pdFALSE )
				{
					for(int i = 0; i < nrProd; i++)
					{
						vTaskResume(TS->pHandle[i]);					
					}

				}
				else
				{
					xSemaphoreGive(xSuspSemaphore);
				}
				
			}
		}
		else
		{
			xSemaphoreGive( GloReadSemaphore );
			if( xSemaphoreTake( xSuspSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{
				//writeUSART_CRT("Queue is empty, consumer goes to sleep\r\n");
				//vTaskResume(pHandle);
				vTaskSuspend(NULL);
				//writeUSART_CRT("Consumer woken \r\n");
				xSemaphoreGive(xSuspSemaphore);
			}

			
			xLastWakeTime = xTaskGetTickCount();
		}
		//vTaskDelayUntil(&xLastWakeTime, 320);

		
	}
}