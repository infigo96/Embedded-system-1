
#include "PC_tasks.h"

int main()
{
	int i = 0;
	//Set the main clock to 12MHz
	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); 
	//InitStuff
	initLED(); initBUTTON(); initUSART(); display_init();
	
	adc_configure(&AVR32_ADC);
	adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_LIGHT_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
	delay_init();

	nQueue = 0;	//Where in queue we are. 
	nrProd = 3;
	nrCons = 1;
	Task_Info *TI = malloc((nrProd+nrCons)*(sizeof(Task_Info)));
	
	Qhandle = xQueueCreate(sizeQ,sizeof(int));
	vSemaphoreCreateBinary(GloQueueSemaphore);
	
	TI[0].task_nr = 0;
	xTaskCreate(LightProducer,"producer",configMINIMAL_STACK_SIZE,&(TI[0]),tskIDLE_PRIORITY + 1,&lightHandle);
	TI[1].task_nr = 1;
	xTaskCreate(TempProducer,"producer",configMINIMAL_STACK_SIZE,&(TI[1]),tskIDLE_PRIORITY + 1,&tempHandle);
	TI[2].task_nr = 2;
	xTaskCreate(PotProducer,"producer",configMINIMAL_STACK_SIZE,&(TI[2]),tskIDLE_PRIORITY + 1,&potHandle);
	
	
	TI[3].task_nr = 3;
	xTaskCreate(Consumer,"consumer",configMINIMAL_STACK_SIZE,&(TI[3]),tskIDLE_PRIORITY + 1,&cHandle);
	
	vTaskStartScheduler();
	
	return 0;
}