/*
 * main.c
 *
 * Created: 2018-09-27 09:50:57
 *  Author: Albert
 */ 

#include "led_tasks.h"

int main(){
	xTaskHandle xHandle;
	xTaskCreate(vBlinkLED1,"Blink1",configMINIMAL_STACK_SIZE,void,tskIDLE_PRIORITY + 1,&xHandle);
	vTaskStartScheduler();
	while(1)
	{
		
	}
	
	
	
	
	return 0;
}