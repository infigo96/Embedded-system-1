
#include "led_tasks.h"
#include "tc.h"
#include "pm.h"
#include "power_clocks_lib.h"
#define nrTasks 3
#define nrPositions nrTasks*50
unsigned int Alltimes[nrPositions];
int pos;

void context( void * pvParameters )
{
	unsigned int times[50];
	volatile avr32_tc_t *tc = &AVR32_TC;
	
	times[0] = tc->channel[0].CV.cv;
	times[1] = tc->channel[0].CV.cv;
	times[2] = tc->channel[0].CV.cv;
	times[3] = tc->channel[0].CV.cv;
	times[4] = tc->channel[0].CV.cv;
	times[5] = tc->channel[0].CV.cv;
	times[6] = tc->channel[0].CV.cv;
	times[7] = tc->channel[0].CV.cv;
	times[8] = tc->channel[0].CV.cv;
	times[9] = tc->channel[0].CV.cv;
	times[10] = tc->channel[0].CV.cv;
	times[11] = tc->channel[0].CV.cv;
	times[12] = tc->channel[0].CV.cv;
	times[13] = tc->channel[0].CV.cv;
	times[14] = tc->channel[0].CV.cv;
	times[15] = tc->channel[0].CV.cv;
	times[16] = tc->channel[0].CV.cv;
	times[17] = tc->channel[0].CV.cv;
	times[18] = tc->channel[0].CV.cv;
	times[19] = tc->channel[0].CV.cv;
	times[20] = tc->channel[0].CV.cv;
	times[21] = tc->channel[0].CV.cv;
	times[22] = tc->channel[0].CV.cv;
	times[23] = tc->channel[0].CV.cv;
	times[24] = tc->channel[0].CV.cv;
	times[25] = tc->channel[0].CV.cv;
	times[26] = tc->channel[0].CV.cv;
	times[27] = tc->channel[0].CV.cv;
	times[28] = tc->channel[0].CV.cv;
	times[29] = tc->channel[0].CV.cv;
	times[30] = tc->channel[0].CV.cv;
	times[31] = tc->channel[0].CV.cv;
	times[32] = tc->channel[0].CV.cv;
	times[33] = tc->channel[0].CV.cv;
	times[34] = tc->channel[0].CV.cv;
	times[35] = tc->channel[0].CV.cv;
	times[36] = tc->channel[0].CV.cv;
	times[37] = tc->channel[0].CV.cv;
	times[38] = tc->channel[0].CV.cv;
	times[39] = tc->channel[0].CV.cv;
	times[40] = tc->channel[0].CV.cv;
	times[41] = tc->channel[0].CV.cv;
	times[42] = tc->channel[0].CV.cv;
	times[43] = tc->channel[0].CV.cv;
	times[44] = tc->channel[0].CV.cv;
	times[45] = tc->channel[0].CV.cv;
	times[46] = tc->channel[0].CV.cv;
	times[47] = tc->channel[0].CV.cv;
	times[48] = tc->channel[0].CV.cv;
	times[49] = tc->channel[0].CV.cv;
	
	vTaskDelay(1000);
	taskENTER_CRITICAL();
	for(int i = 0; i<50; i++)
	{
		Alltimes[i + pos] = times[i];
	}
	pos += 50;
	taskEXIT_CRITICAL();
	vTaskDelay(1000);
	while(1);
}
int main()
{
	//Init the GPIO units
	initLED(); initBUTTON();initUSART();
	volatile avr32_tc_t *tc = &AVR32_TC;
	static tc_waveform_opt_t opt;
	unsigned int channel = 0; //Compiler complains if 0 is written directly
	opt.channel = channel;
	opt.wavsel = TC_WAVEFORM_SEL_UP_MODE;
	opt.tcclks = TC_CLOCK_SOURCE_TC2; //CLK / 2
	tc_init_waveform(tc, &opt);
	
	pcl_switch_to_osc(PCL_OSC0,FOSC0,OSC0_STARTUP); //Set the main clock to 12MHz
	tc_start(tc,channel);

	xTaskHandle *xTaskHandle_blink = malloc(sizeof(xTaskHandle)*3);
	xTaskHandle *xTaskHandle_light = malloc(sizeof(xTaskHandle)*3);
	pos = 0;
	//The tasks for handling blinking of the three LEDs
	/*xTaskCreate(vBlinkLED1,"Blink1",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&(xTaskHandle_blink[0]));
	xTaskCreate(vBlinkLED2,"Blink2",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&(xTaskHandle_blink[1]));
	xTaskCreate(vBlinkLED3,"Blink3",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&(xTaskHandle_blink[2]));
	
	//The tasks that sets the three LEDs
	xTaskCreate(vLightLED1,"Light1",configMINIMAL_STACK_SIZE,&(xTaskHandle_blink[0]),tskIDLE_PRIORITY + 2,&(xTaskHandle_light[0]));
	xTaskCreate(vLightLED2,"Light2",configMINIMAL_STACK_SIZE,&(xTaskHandle_blink[1]),tskIDLE_PRIORITY + 2,&(xTaskHandle_light[1]));
	xTaskCreate(vLightLED3,"Light3",configMINIMAL_STACK_SIZE,&(xTaskHandle_blink[2]),tskIDLE_PRIORITY + 2,&(xTaskHandle_light[2]));
	
	xTaskCreate(vReadButtons,"Buttons",configMINIMAL_STACK_SIZE,xTaskHandle_light,tskIDLE_PRIORITY + 2,NULL);
	//Suspend the task so the lights starts blinking
	vTaskSuspend(xTaskHandle_light[0]); 
	vTaskSuspend(xTaskHandle_light[1]);
	vTaskSuspend(xTaskHandle_light[2]); */
	for(int i = 0; i < nrTasks ; i++)
	{
		xTaskCreate(context,"context",4*configMINIMAL_STACK_SIZE,xTaskHandle_light,tskIDLE_PRIORITY + 1,NULL);
	}
	vTaskStartScheduler();
	while(1);
	return 0;
}