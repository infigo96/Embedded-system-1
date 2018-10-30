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
	char text1[] = "Phil(x): Started\n";
	char text2[] = "Phil(x): Grabbed left\n";
	char text3[] = "Phil(x): Grabbed right\r\n";
	char text4[] = "Phil(x): Started eating----------\n";
	char text5[] = "Phil(x): Done eating----------\n";
	char text6[] = "Phil(x): Put down left\n";
	char text7[] = "Phil(x): Put down right\n";
	text1[5] = 48 + id;
	text2[5] = 48 + id;
	text3[5] = 48 + id;
	text4[5] = 48 + id;
	text5[5] = 48 + id;
	text6[5] = 48 + id;
	text7[5] = 48 + id;
	writeUSART_CRT(text1);
	for(;;)
	{
		// Think for a while
		vTaskDelay(2000);
		
		// Think until the left chopstick is available; when it is, pick it up;
		if( xSemaphoreTake( xSemaphore[id], ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			writeUSART_CRT(text2);
		
			// Think until the right chopstick is available; when it is, pick it up;
			//If this is set to MAX_DELAY, the program will be stuck
			if( xSemaphoreTake( xSemaphore[(id+1) % NR_OF_PHIL], ( portTickType ) 500) == pdTRUE )
			{
				writeUSART_CRT(text3);
		
				// When both chopsticks are held, eat for a fixed amount of time;
				writeUSART_CRT(text4);
				vTaskDelay(3500);
				writeUSART_CRT(text5);
		
				// Then, put the right chopstick down;
				writeUSART_CRT(text6);
				xSemaphoreGive( xSemaphore[id]);
				
			}
			// Then, put the left chopstick down;
			writeUSART_CRT(text7);
			xSemaphoreGive( xSemaphore[(id+1)%NR_OF_PHIL] );
		}
		
		// Repeat from the beginning.
	}
}