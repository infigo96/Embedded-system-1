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

//Producer task. Produces potentiometer values and adds them 2 queue.
void potentSensorTask(void * pvParameters)
{
	task_struct *TS = (task_struct *)pvParameters;
	value_type_struct VT; 
	VT.taskValue = 0;
	VT.taskType = 'p';
	
	for(;;)
	{
		// Get the potentiometer value
		adc_start(&AVR32_ADC);
		VT.taskValue = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
		if( xSemaphoreTake( TS->writeQueueSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			if(xQueueSendToBack(Qhandle,&VT,1) == 1)
			{
				//Update the number of items on the queue.
				nQueue++;
				if(nQueue >= sizeQ-1)
				{
					if( xSemaphoreTake( TS->consumerSuspendSemaphore, ( portTickType ) 10) == pdFALSE )
					{
						vTaskResume(TS->cHandle);
					}
					else
					{
						xSemaphoreGive(TS->consumerSuspendSemaphore);
						vTaskDelay(1);
					}
					xSemaphoreGive(TS->writeQueueSemaphore);
					vTaskDelay(1);
				}
			}
			else
			{
				//Queue full. Wake consumer, then go to sleep.
				xSemaphoreGive(TS->writeQueueSemaphore);
				vTaskDelay(1);
				vTaskResume(TS->cHandle);
				vTaskSuspend(NULL);
			}
		}
	}
}

//Producer task. Produces temperature values and adds them 2 queue.
void tempSensorTask(void * pvParameters)
{
	task_struct *TS = (task_struct *)pvParameters;
	value_type_struct VT;
	VT.taskValue = 0;
	VT.taskType = 't';
	
	for(;;)
	{
		for (int i=0;i<10;i++)
		{
			// Get the potentiometer value
			adc_start(&AVR32_ADC);
			VT.taskValue += adc_get_value(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
		}
		VT.taskValue/=10;
		//Check if there is space in the queue (1 if there is space, 0 else) and if there is, write to it..
		if( xSemaphoreTake( TS->writeQueueSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			if(xQueueSendToBack(Qhandle,&VT,1) == 1)
			{
				//Update the number of items on the queue.
				nQueue++;
				if(nQueue >= sizeQ-1)
				{
					if( xSemaphoreTake( TS->consumerSuspendSemaphore, ( portTickType ) 10) == pdFALSE )
					{
						vTaskResume(TS->cHandle);
					}
					else
					{
						xSemaphoreGive(TS->consumerSuspendSemaphore);
						vTaskDelay(1);
					}
					xSemaphoreGive(TS->writeQueueSemaphore);
					vTaskDelay(1);
				}
			}
			else
			{
				//Queue full. Wake consumer, then go to sleep.
				xSemaphoreGive(TS->writeQueueSemaphore);
				vTaskDelay(1);
				vTaskResume(TS->cHandle);
				vTaskSuspend(NULL);
			}
		}
	}
}

//Producer task. Produces light values and adds them 2 queue.
void lightSensorTask(void * pvParameters)
{
	task_struct *TS = (task_struct *)pvParameters;
	value_type_struct VT;
	VT.taskValue = 0;
	VT.taskType = 'l';
	
	for(;;)
	{
		for (int i=0;i<10;i++)
		{
			// Get the potentiometer value
			adc_start(&AVR32_ADC);
			VT.taskValue += adc_get_value(&AVR32_ADC, ADC_LIGHT_CHANNEL);
		}
		VT.taskValue/=10;
		//Check if there is space in the queue (1 if there is space, 0 else) and if there is, write to it..
		if( xSemaphoreTake( TS->writeQueueSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			if(xQueueSendToBack(Qhandle,&VT,1) == 1)
			{
				//Update the number of items on the queue.
				nQueue++;
				if(nQueue >= sizeQ-1)
				{
					if( xSemaphoreTake( TS->consumerSuspendSemaphore, ( portTickType ) 10) == pdFALSE )
					{
						vTaskResume(TS->cHandle);

					}
					else
					{
						xSemaphoreGive(TS->consumerSuspendSemaphore);
						vTaskDelay(1);
					}
				}
				xSemaphoreGive(TS->writeQueueSemaphore);
				vTaskDelay(1);
			}
			else
			{
				//Queue full. Wake consumer, then go to sleep.
				xSemaphoreGive(TS->writeQueueSemaphore);
				vTaskDelay(1);
				vTaskResume(TS->cHandle);
				vTaskSuspend(NULL);
			}
		}
	}
}


//Reads three sensor values from queue AND writes them on LCD and Usart.
void sensorReaderTask(void * pvParameters)
{
	task_struct *TS = (task_struct *)pvParameters;
	value_type_struct VT;
	int light_value, potent_value, temp_value = 0;
	char charBuff[60];
	int l, p , t = 0;

	for(;;)
	{
		//Read from queue if there is stuff there.
		if(xQueueReceive(Qhandle,&VT,1) == 1)
		{
			nQueue--;
			//We read from queue, find out what we read.
			/*
			if(VT.taskType == 'l')
			{
				//Light Sensor data in queue.
				light_value = VT.taskValue;
				l++;
			}
			if(VT.taskType == 't')
			{
				//Temp sensor data in queue.
				potent_value = VT.taskValue;
				p++;
			}
			*/
			if (VT.taskType == 'p')
			{
				//Potentiometer data in queue.
				temp_value = VT.taskValue;
				t++;
			}
			/*
			if (l-p >= 3|| l-t >=3)
			{
				vTaskSuspend(TS->producerHandles.lHandle);
				vTaskResume(TS->producerHandles.pHandle);
				vTaskResume(TS->producerHandles.tHandle);
			}
			if (p-l >= 3|| p-t >=3)
			{
				vTaskSuspend(TS->producerHandles.pHandle);
				vTaskResume(TS->producerHandles.lHandle);
				vTaskResume(TS->producerHandles.tHandle);
			}
			if (t-l >= 3|| t-p >=3)
			{
				vTaskSuspend(TS->producerHandles.tHandle);
				vTaskResume(TS->producerHandles.lHandle);
				vTaskResume(TS->producerHandles.pHandle);
			}
			*/
		}
		else
		{
			if( xSemaphoreTake( TS->consumerSuspendSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{
				//Queue is empty. Wake producers and Go to sleep
				//vTaskResume(TS->producerHandles.lHandle);
				vTaskResume(TS->producerHandles.pHandle);
				//vTaskResume(TS->producerHandles.tHandle);
				l, t, p = 0;
				vTaskSuspend(NULL);
				xSemaphoreGive(TS->consumerSuspendSemaphore);
			}
		}
		sprintf(charBuff, "LightValue: %4d , PotentiometerValue: %4d , Temperature Value %3d\n\n\n\n\n\n", light_value, potent_value, temp_value);
		writeUSART_CRT(charBuff);
	}
}

//Old producer task.
/*
void potentSensorTask(void * pvParameters)
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
*/

//Old consumer task.
/*
void sensorReader(void * pvParameters)
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
			//Write what was in the queue.
			writeUSART_CRT(&byte);
			//Update global
			nQueue--;
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
*/


// other stuff



/*
			//Update items on queue.
			if( xSemaphoreTake( TS->queueCountSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{


//Write first row
dip204_set_cursor_position(1,1);
sprintf(text,"%04d",pot_value);
dip204_printf_string(text);

//Write second row
dip204_set_cursor_position(1,2);
sprintf(text,"%04d",lig_value);
dip204_printf_string(text);


//Write third row
dip204_set_cursor_position(1,3);
sprintf(text,"%04d",tem_value);
dip204_printf_string(text);
delay_ms(1000);*/