#include "PC_tasks.h"
/*
Problem:
1. think until the left chopstick is available; when it is, pick it up;
2. think until the right chopstick is available; when it is, pick it up;
3. when both chopsticks are held, eat for a fixed amount of time;
4. then, put the right chopstick down;
5. then, put the left chopstick down;
6. repeat from the beginning.
*/
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

//Initiates Button 0, 1 and 2.
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


//Calls writeUsart(const char * message) from a critical state to prvent message overlapping.
void writeUSART_CRT(const char * message)
{
	//Prevent task writing over each other by entering critical state during writing
	if( xSemaphoreTake( writeSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
	{
		writeUSART(message);
		xSemaphoreGive(writeSemaphore);
	}
}


void Philosopher(void * pvParameters)
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	int * ts = (int *)pvParameters;
	int id = *ts;
	char text[] = "Phil( ): Started\r\n";
	text[5] = 46 + id;
	writeUSART_CRT(text);
	for(;;)
	{
		// Think for a while
		vTaskDelay(100);
		
		// Think until the left chopstick is available; when it is, pick it up;
		if( xSemaphoreTake( xSemaphore[id], ( portTickType ) portMAX_DELAY) == pdTRUE ){}
		//sprintf(text,"Phil(%d): Grabbed left\n",id);
		writeUSART_CRT(text);
		
		// Think until the right chopstick is available; when it is, pick it up;
		if( xSemaphoreTake( xSemaphore[(id+1) % NR_OF_PHIL], ( portTickType ) portMAX_DELAY) == pdTRUE ){}
		//sprintf(text,"Phil(%d): Grabbed right\n",id);
		writeUSART_CRT(text);
		
		// When both chopsticks are held, eat for a fixed amount of time;
		//sprintf(text,"Phil(%d): Started eating\n",id);
		writeUSART_CRT(text);
		vTaskDelay(1500);
		//sprintf(text,"Phil(%d): Done eating\n",id);
		writeUSART_CRT(text);
		
		// Then, put the right chopstick down;
		xSemaphoreGive( xSemaphore[id]);
		// Then, put the left chopstick down;
		xSemaphoreGive( xSemaphore[(id+1)%NR_OF_PHIL] );
		
		//sprintf(text,"Phil(%d): Put down chopstick\n",id);
		writeUSART_CRT(text);
		
		// Repeat from the beginning.
	}
}