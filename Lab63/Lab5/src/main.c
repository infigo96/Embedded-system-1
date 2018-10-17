
#include "PC_tasks.h"

int main()
{
	//Set the main clock to 12MHz
	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); 
	//InitStuff
	initLED(); initBUTTON(); initUSART();
	
	// Configure the ADC module and enable the potentiometer channel
	adc_configure(&AVR32_ADC);
	adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_LIGHT_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
	display_init();
	
	//Create queues with space for one value (int)
	QLight = xQueueCreate(1,sizeof(volatile int)); //LightTask --> DisplayTask
	QTemp = xQueueCreate(1,sizeof(volatile int));  //TempTask --> DisplayTask
	QPotent = xQueueCreate(1,sizeof(volatile int));//PotenTask --> DisplayTask  
	
	//Create tasks.
	xTaskCreate(LightTask,"light",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,NULL);
	xTaskCreate(TempTask,"temp",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,NULL);
	xTaskCreate(PotenTask,"potent",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,NULL);
	xTaskCreate(DisplayTask,"display",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,NULL);

	vTaskStartScheduler();
	
	return 0;
}