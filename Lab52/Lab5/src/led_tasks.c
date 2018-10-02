#include "led_tasks.h"

void setPeriod(int period,task_struct * ts){
	ts->task_period = period;
	//ts->next_period = period;
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
void initBUTTON(void);
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

void vBlinkLED1( void * pvParameters )
{
	((task_struct *)pvParameters)->last_waketime = xTaskGetTickCount();
	while (1)
	{
		vTaskDelayUntil(&((task_struct *)pvParameters)->last_waketime,((task_struct *)pvParameters)->task_period);
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED0_PIN);
		writeUSART_CRT("Blink1 - Toggle LED0\r\n");
	} 
		
}
void vBlinkLED2( void * pvParameters )
{
	((task_struct *)pvParameters)->last_waketime = xTaskGetTickCount();
	while (1)
	{
		vTaskDelayUntil(&((task_struct *)pvParameters)->last_waketime,((task_struct *)pvParameters)->task_period);
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED1_PIN); 
		writeUSART_CRT("Blink2 - Toggle LED1\r\n");
		
	} 
		
}
void vBlinkLED3( void * pvParameters )
{
	((task_struct *)pvParameters)->last_waketime = xTaskGetTickCount();
	while (1) 
	{ 
		vTaskDelayUntil(&((task_struct *)pvParameters)->last_waketime,((task_struct *)pvParameters)->task_period);
		AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED2_PIN);
		writeUSART_CRT("Blink3 - Toggle LED2\r\n");
	}
}
void vReadButtons(void * pvParameters)
{
	unsigned int btn_state[3];
	unsigned int prev_btn_state[3];
	for(;;)
	{
		//Read the state of the buttons
		btn_state[0] = AVR32_GPIO.port[BUTTON_PORT].pvr & BUTTON0_PIN;
		btn_state[1] = AVR32_GPIO.port[BUTTON_PORT].pvr & BUTTON1_PIN;
		btn_state[2] = AVR32_GPIO.port[BUTTON_PORT].pvr & BUTTON2_PIN;
		//Only goes into the state the first time of a button press 
		if(btn_state[0]==0 && btn_state[0] != prev_btn_state[0])
		{
			writeUSART_CRT("Button - Resuming Light1\r\n");
			vTaskResume(((xTaskHandle*)pvParameters)[0]); //Resumes the task lightled1
		}
		if(btn_state[1]==0 && btn_state[1] != prev_btn_state[1])
		{
			writeUSART_CRT("Button - Resuming Light2\r\n");
			vTaskResume(((xTaskHandle*)pvParameters)[1]); //Resumes the task lightled2
		}
		if(btn_state[2]==0 && btn_state[2] != prev_btn_state[2])
		{
			writeUSART_CRT("Button - Resuming Light3\r\n");
			vTaskResume(((xTaskHandle*)pvParameters)[2]); //Resumes the task lightled3
		}
		//Save previous button state
		for (int i=0;i<3;i++)
		{
			prev_btn_state[i] = btn_state[i];
		}
		vTaskDelay(100);
	}
}	
void vOverseer(void * pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xFrequency = 500;
	char text[50];
	xLastWakeTime = xTaskGetTickCount();
	portTickType taskLastWake, taskPeriod,overseer;
	for (;;)
	{
		for (int i=0;i<1;i++)
		{
			taskLastWake = ((task_struct *)pvParameters)[i].last_waketime;
			taskPeriod = ((task_struct *)pvParameters)[i].task_period;
			overseer = xTaskGetTickCount();
			if(taskLastWake + taskPeriod < overseer)
			{
				sprintf(text,"Missed deadline on task %d\r\n",i+1);
				writeUSART_CRT(text);
			}
		}
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}
}

void writeUSART_CRT(const char * message)
{
	//Prevent task writing over each other by entering critical state during writing
	taskENTER_CRITICAL();
	writeUSART(message);
	taskEXIT_CRITICAL();
}