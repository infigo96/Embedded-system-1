
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

	nQueue = 0;
	nrProd = 3;
	nrCons = 1;
	Task_Info *TI = malloc((nrProd+nrCons)*(sizeof(Task_Info)));
	task_struct *TS = malloc(sizeof(task_struct));
	TS->cHandle = malloc(nrCons*(sizeof(xTaskHandle)));
	TS->pHandle = malloc(nrProd*(sizeof(xTaskHandle)));
	
	//Size of Queue buffer
	//Size of queue, size of a slot
	Qhandle = xQueueCreate(sizeQ,sizeof(int));
	vSemaphoreCreateBinary(GloReadSemaphore);
	vSemaphoreCreateBinary(GloTranSemaphore);
	vSemaphoreCreateBinary(GloQueueSemaphore);
	//vSemaphoreCreateBinary(xSuspSemaphore);
	xSuspSemaphore = xSemaphoreCreateMutex();
	
	TI[0].task_nr = 0;
	TI[0].Ts = TS;
	xTaskCreate(LightProducer,"producer",configMINIMAL_STACK_SIZE,&(TI[0]),tskIDLE_PRIORITY + 1,&(TS->pHandle[0]));
	TI[1].task_nr = 1;
	TI[1].Ts = TS;
	xTaskCreate(TempProducer,"producer",configMINIMAL_STACK_SIZE,&(TI[1]),tskIDLE_PRIORITY + 1,&(TS->pHandle[1]));
	TI[2].task_nr = 2;
	TI[2].Ts = TS;
	xTaskCreate(PotProducer,"producer",configMINIMAL_STACK_SIZE,&(TI[2]),tskIDLE_PRIORITY + 1,&(TS->pHandle[2]));
	
	
	TI[3].task_nr = 3;
	TI[3].Ts = TS;
	xTaskCreate(Consumer,"consumer",configMINIMAL_STACK_SIZE,&(TI[3]),tskIDLE_PRIORITY + 1,&(TS->cHandle[0]));
	
	vTaskStartScheduler();
	
	return 0;
}