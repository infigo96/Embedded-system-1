
#include "PC_tasks.h"
#include "board.h"
#include "display_init.h"
#include <stdio.h>
#include "USART_driver.h"

int main()
{
	//Set the main clock to 12MHz
	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); 
	//InitStuff
	display_init(); initLED(); initBUTTON(); initUSART(); 

	// Configure the ADC module and enable the potentiometer channel
	adc_configure(&AVR32_ADC);
	adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_LIGHT_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
	delay_init();

	//Globals
	nQueue = 0;
	task_struct *TS;

	//Size of Queue buffer
	sizeQ = 12;
	//Size of queue, size of a slot
	Qhandle = xQueueCreate(sizeQ,sizeof(value_type_struct));
	
	vSemaphoreCreateBinary((TS->consumerSuspendSemaphore));
	//if( TS->xSemaphore != NULL )
	//{
		//writeUSART("Semaphore created\r\n");
	//}
	//Create tasks.
	xTaskCreate(potentSensorTask,"potentSensorTask",configMINIMAL_STACK_SIZE*4,TS,tskIDLE_PRIORITY + 1,&(TS->producerHandles.pHandle));
	xTaskCreate(tempSensorTask,"tempSensorTask",configMINIMAL_STACK_SIZE*4,TS,tskIDLE_PRIORITY + 1,&(TS->producerHandles.tHandle));
	xTaskCreate(lightSensorTask,"lightSensorTask",configMINIMAL_STACK_SIZE*4,TS,tskIDLE_PRIORITY + 1,&(TS->producerHandles.lHandle));
	xTaskCreate(sensorReaderTask,"sensorReaderTask",configMINIMAL_STACK_SIZE*4,TS,tskIDLE_PRIORITY + 1,&(TS->cHandle));

	vTaskStartScheduler();
	
	return 0;
}