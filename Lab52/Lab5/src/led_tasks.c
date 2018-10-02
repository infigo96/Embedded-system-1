#include "led_tasks.h"


void blinkdelay(int ms, int pin){
	long volatile cycles = 970*ms;
	AVR32_GPIO.port[LED_PORT].ovrc = (1 << pin);
	portTickType start = xTaskGetTickCount();
	while (cycles != 0)
	{
		
		cycles--;
	}
	portTickType end = xTaskGetTickCount();
	AVR32_GPIO.port[LED_PORT].ovrs = (1 << pin);
}
void configTaskTime(int period,int deley, task_struct * ts){
	ts->task_period = period;
	ts->next_period = period + deley;
	ts->last_donetime = 0;
	ts->start_delay = deley;
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
	task_struct * ts = (task_struct *)pvParameters;
	vTaskDelay(ts->start_delay);
	ts->last_waketime = xTaskGetTickCount();
	while (1)
	{
		writeUSART_CRT("Blink1 - Start\r\n");
		if( xSemaphoreTake( xSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE ){
			blinkdelay(2000,LED0_PIN);
			xSemaphoreGive( xSemaphore );
		}
		writeUSART_CRT("Blink1 - End\r\n");
		ts->last_donetime = xTaskGetTickCount();
		vTaskSuspend(NULL);
		vTaskDelayUntil(&(ts->last_waketime),ts->task_period);
	} 
		
}
void vBlinkLED2( void * pvParameters )
{
	task_struct * ts = (task_struct *)pvParameters;
	vTaskDelay(ts->start_delay);
	((task_struct *)pvParameters)->last_waketime = xTaskGetTickCount();
	while (1)
	{
		writeUSART_CRT("Blink2 - Start\r\n");
		blinkdelay(2000,LED1_PIN);
		writeUSART_CRT("Blink2 - End\r\n");
		vTaskDelayUntil(&((task_struct *)pvParameters)->last_waketime,((task_struct *)pvParameters)->task_period);
		vTaskSuspend(NULL);
		//writeUSART_CRT("Blink2 - Toggle LED1\r\n");
		
	} 
		
}
void vBlinkLED3( void * pvParameters )
{
	task_struct * ts = (task_struct *)pvParameters;
	vTaskDelay(ts->start_delay);
	((task_struct *)pvParameters)->last_waketime = xTaskGetTickCount();
	while (1) 
	{ 
		writeUSART_CRT("Blink3 - Start\r\n");	
		if( xSemaphoreTake( xSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
        {
			blinkdelay(200,LED2_PIN);
            xSemaphoreGive( xSemaphore );
        }
		writeUSART_CRT("Blink3 - End\r\n");
		vTaskSuspend(NULL);
		vTaskDelayUntil(&((task_struct *)pvParameters)->last_waketime,((task_struct *)pvParameters)->task_period);
	}
}
void vReadButtons(void * pvParameters)
{
	const portTickType xFrequency = 100;
	char text[50];
	portTickType xLastWakeTime = xTaskGetTickCount();
	int i = 0;
	unsigned int btn_state[3];
	unsigned int prev_btn_state[3];
	for(;;)
	{
		//Read the state of the buttons
		btn_state[0] = AVR32_GPIO.port[BUTTON_PORT].pvr & BUTTON0_PIN;
		btn_state[1] = AVR32_GPIO.port[BUTTON_PORT].pvr & BUTTON1_PIN;
		btn_state[2] = AVR32_GPIO.port[BUTTON_PORT].pvr & BUTTON2_PIN;
		//Only goes into the state the first time of a button press 
		while(btn_state[0]==0)
		{
			 i++;
			 btn_state[0] = AVR32_GPIO.port[BUTTON_PORT].pvr & BUTTON0_PIN;
		}
		vTaskDelayUntil(&xLastWakeTime, 100);
	}
}	
void vOverseer(void * pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xFrequency = 100;
	char text[50];
	xLastWakeTime = xTaskGetTickCount();
	task_struct * ts;
	portTickType LWT, TASK_D,P,curr_t;
	
	for (;;)
	{
		for (int i=0;i<3;i++)
		{
			ts = &((task_struct *)pvParameters)[i];
			LWT = ts->last_waketime;
			P = ts->task_period;
			TASK_D = ts->last_donetime;
			curr_t = xTaskGetTickCount();
			//if((curr_t > LWT && TASK_D < LWT- P) || curr_t > LWT + 2*P)

			//Has the task performed the work within the period
			if(ts->last_donetime >= ts->next_period - P && ts->last_donetime < ts->next_period)
			{
				//The work was done within the period, move the nest deadline
				ts->next_period += ts->task_period;
			}
			//Has the deadline been passed
			else if(curr_t>ts->next_period)
			{
				sprintf(text,"Missed deadline on task %d\r\n",i+1);
				writeUSART_CRT(text);
				ts->next_period += ts->task_period;
				i--;//Run the check again, to see if 
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