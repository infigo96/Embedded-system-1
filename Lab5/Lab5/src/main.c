
#include "led_tasks.h"

int main()
{
	initLED();
	AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED0_PIN);
	static unsigned char ucParameterToPass;
	int i = 0;
	xTaskHandle xHandle;
	xTaskCreate(vBlinkLED1,"Blink1",configMINIMAL_STACK_SIZE,&ucParameterToPass,tskIDLE_PRIORITY + 1,&xHandle);
	
	vTaskStartScheduler();
	
	return 0;
}