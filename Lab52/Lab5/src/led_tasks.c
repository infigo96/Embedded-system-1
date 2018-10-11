#include "led_tasks.h"

//Lights a LED at PIN for MS.
void holdLED(int ms, int pin){
	long volatile cycles = 970*ms;
	AVR32_GPIO.port[LED_PORT].ovrc = (1 << pin);
	while (cycles != 0)
	{
		cycles--;
	}
	AVR32_GPIO.port[LED_PORT].ovrs = (1 << pin);
}

//Initiate timing information for a task.
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
		//Takes xSemaphore if available. If not available go to sleep until xSemaphore is available.	
		if( xSemaphoreTake( xSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			//Block for 2000 ms then return xSemaphore.
			writeUSART_CRT("Blink1 - Take Semaphore\r\n");
			holdLED(2000,LED0_PIN);
			writeUSART_CRT("Blink1 - Release Semaphore\r\n");
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
		//Block for 2000 ms.
		writeUSART_CRT("Blink2 - Start\r\n");
		holdLED(2000,LED1_PIN);
		writeUSART_CRT("Blink2 - End\r\n");
		//Sleep for remaining time (about 4000 ms if not interrupted.)
		vTaskDelayUntil(&((task_struct *)pvParameters)->last_waketime,((task_struct *)pvParameters)->task_period);
		vTaskSuspend(NULL);
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
		
		//Takes xSemaphore if available. If not available go to sleep until xSemaphore is available.	
		if( xSemaphoreTake( xSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
        {
			//Block for 200 ms then return xSemaphore.
			writeUSART_CRT("Blink3 - Take Semaphore\r\n");
			holdLED(200,LED2_PIN);
            xSemaphoreGive( xSemaphore );
			writeUSART_CRT("Blink3 - Release Semaphore\r\n");
        }
		writeUSART_CRT("Blink3 - End\r\n");
		vTaskSuspend(NULL);
		vTaskDelayUntil(&((task_struct *)pvParameters)->last_waketime,((task_struct *)pvParameters)->task_period);
	}
}

// Used for debug purposes.
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
		//Read the state of the button 0
		btn_state[0] = AVR32_GPIO.port[BUTTON_PORT].pvr & BUTTON0_PIN;
		
		// Blocking State as long as button is pressed.
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
	// Buffer for messaging.
	char text[50];
	
	//Timing information for Overseer.
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const portTickType xFrequency = 100;

	//Task information.
	task_struct * ts;
	portTickType P,CURR_TIME;
	
	for (;;)
	{
		for (int i=0;i<3;i++)
		{
			ts = &((task_struct *)pvParameters)[i];
			P = ts->task_period;

			//Current system time when overseer starts operating on said task.
			CURR_TIME = xTaskGetTickCount();

			//Has the task performed the work within the required time interval T->T+p
			if(ts->last_donetime >= ts->next_period - P && ts->last_donetime < ts->next_period)
			{
				//The work was done within the period, move the nest deadline
				ts->next_period += ts->task_period;
			}
			//No work finished in this time period. Has the deadline passed?
			else if(CURR_TIME>ts->next_period)
			{
				// Let user know deadline has been passed.
				sprintf(text,"Missed deadline on task %d\r\n",i+1);
				writeUSART_CRT(text);

				// Move to next deadline.
				ts->next_period += P;

				//Rerun this i in case this task missed several deadlines (overseer must have been blocked for this 2 happen).
				i--;
			}
		}

		//Sleep Overseer for xFrequency.
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}
}


//Calls writeUsart(const char * message) from a critical state to prvent message overlapping.
void writeUSART_CRT(const char * message)
{
	//Prevent task writing over each other by entering critical state during writing
	taskENTER_CRITICAL();
	writeUSART(message);
	taskEXIT_CRITICAL();
}