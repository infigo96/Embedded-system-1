#include "PC_tasks.h"

//Initiates LED 0, 1 and 2.
void PrintUsart(unsigned int iPot, unsigned int iTemp, unsigned int iLight)
{
	char *temp = malloc(128);
	sprintf(temp,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPotentiometer Value:%d\nTemp Value:%d\nLight Value:%d\n", iPot, iTemp, iLight);
	writeUSART(temp);
	free(temp);
}

void PrintLight(unsigned int i)
{
	char *temp = malloc(64);
	sprintf(temp,"Light value:%d",i);
	dip204_set_cursor_position(1,3);
	dip204_write_string("                    ");
	dip204_set_cursor_position(1,3);
	dip204_write_string(temp);
	free(temp);
}
void PrintPot(unsigned int i)
{
	char *temp = malloc(64);
	sprintf(temp,"Pot Value:%d",i);
	dip204_set_cursor_position(1,1);
	dip204_write_string("                    ");
	dip204_set_cursor_position(1,1);
	dip204_write_string(temp);
	free(temp);
}
void PrintTemp(unsigned int i)
{
	char *temp = malloc(64);
	sprintf(temp,"Temp Value:%d",i);
	dip204_set_cursor_position(1,2);
	dip204_write_string("                    ");
	dip204_set_cursor_position(1,2);
	dip204_write_string(temp);
	free(temp);
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
	unsigned int Value = TI->task_nr;
	
	for(;;)
	{
		vTaskDelay(50);
		adc_start(&AVR32_ADC); // Start a ADC sampling of all active channels
		Value = adc_get_value(&AVR32_ADC, ADC_LIGHT_CHANNEL);	//Only 10 bits used out of 32
		Value |= (1 << 14);	//Adds a bit in the unused space in the uint for identifying this specific type of data
		
		if(xQueueSendToFront(Qhandle,&Value,50) == 1)
		{
			xSemaphoreTake( GloQueueSemaphore,  ( portTickType ) portMAX_DELAY);
			//Global variable that keeps track of absolute space in queue.
			nQueue++;
			xSemaphoreGive( GloQueueSemaphore );			
		}
		
	}
}
void TempProducer(void * pvParameters)
{
	Task_Info *TI = (Task_Info *) pvParameters;
	unsigned int Value = TI->task_nr;
	
	for(;;)
	{
		vTaskDelay(50);
		adc_start(&AVR32_ADC); // Start a ADC sampling of all active channels
		Value = adc_get_value(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
		Value |= (1 << 13);	//Adds a bit in the unused space in the uint for identifying this specific type of data
		
		//Check if there is space in the queue (1 if there is space, 0 else) and if there is, write to it..
		if(xQueueSendToFront(Qhandle,&Value,50) == 1)
		{
			xSemaphoreTake( GloQueueSemaphore,  ( portTickType ) portMAX_DELAY);
			//Global variable that keeps track of absolute space in queue.
			nQueue++;
			xSemaphoreGive( GloQueueSemaphore );			
		}
		
	}
}
void PotProducer(void * pvParameters)
{
	Task_Info *TI = (Task_Info *) pvParameters;
	unsigned int Value = TI->task_nr;
	
	for(;;)
	{
		vTaskDelay(50);
		adc_start(&AVR32_ADC); // Start a ADC sampling of all active channels
		Value = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
		Value |= (1 << 12);	//Adds a bit in the unused space in the uint for identifying this specific type of data
		
		if(xQueueSendToFront(Qhandle,&Value,50) == 1)
		{
			xSemaphoreTake( GloQueueSemaphore,  ( portTickType ) portMAX_DELAY);
			//Global variable that keeps track of absolute space in queue.
			nQueue++;
			xSemaphoreGive( GloQueueSemaphore );			
		}
		
	}
}
//Reads bytes from a queue.
void Consumer(void * pvParameters)
{
	Task_Info *TI = (Task_Info *) pvParameters;
	unsigned int Value = TI->task_nr;
	int iPot = 0, iTemp = 0, iLight = 0; 
	
	for(;;)
	{
		//Read from queue if there is stuff there.
		if(xQueueReceive(Qhandle,&Value,0) == 1)	//Some yeld issue internally here so a tick wait >0 gives issus, I just wait in the else instead. 
		{
			xSemaphoreTake( GloQueueSemaphore,  ( portTickType ) portMAX_DELAY);
			//Update global
			nQueue--;
			xSemaphoreGive( GloQueueSemaphore );
			
			if((Value & (1 << 12)) != 0)	//Check which type of sensor the data is and print to correct LCD row
			{
				Value &= ~(1<<12);
				iPot = Value;
				PrintPot(iPot);
			}
			else if((Value & (1 << 13)) != 0)
			{
				Value &= ~(1<<13);
				iTemp = Value;
				PrintTemp(iTemp);
			}
			else if((Value & (1 << 14)) != 0)
			{
				Value &= ~(1<<14);
				iLight = Value; 
				PrintLight(iLight);
			}
			PrintUsart(iPot,iTemp,iLight);	//Print to USART always after new data is recieved 
			
		}
		else
		{
			vTaskDelay(100);
		}
	}
}