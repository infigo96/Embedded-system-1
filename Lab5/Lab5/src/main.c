/*
 * main.c
 *
 * Created: 2018-09-27 09:50:57
 *  Author: Albert
 */ 

#include "led_tasks.h"

int main()
{
	static unsigned char ucParameterToPass;
	int i = 0;
	xTaskHandle xHandle;
	xTaskCreate(vBlinkLED1,"Blink1",configMINIMAL_STACK_SIZE,&ucParameterToPass,tskIDLE_PRIORITY + 1,&xHandle);
	vTaskStartScheduler();
	while(1)
	{
		i++;
	}
	
	
	
	
	return 0;
}