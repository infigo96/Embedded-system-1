#include "tasks.h"
#include "dip204.h"

void writedisp(char *m){
	dip204_set_cursor_position(1,1);
	dip204_write_string("          ");
	dip204_set_cursor_position(1,2);
	dip204_write_string("          ");
	dip204_set_cursor_position(1,3);
	dip204_write_string("          ");
	dip204_set_cursor_position(1,1);
	dip204_write_string(m);
}
void writeNrOfChars(unsigned int i){
	char temp[60];
	sprintf(temp,"Nr of chars:%d",i);
	dip204_set_cursor_position(1,4);
	dip204_write_string("                    ");
	dip204_set_cursor_position(1,4);
	dip204_write_string(temp);
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


//Calls writeUsart(const char * message) from a critical state to prvent message overlapping.
void writeUSART_CRT(const char * message)
{
	//Prevent task writing over each other by entering critical state during writing
	taskENTER_CRITICAL();
	writeUSART(message);
	taskEXIT_CRITICAL();
}

void ReaderTask(void * pvParameters)
{
	int byteCount;
	char byte = '0';
	portTickType xLastWakeTime = xTaskGetTickCount();
	char message[64];
	int len;
	char clearMsg[60] = "                                                            ";
	for(;;)
	{
		//---------------display-------------Print every row separate
		//Double semaphore will take and then wait for the semaphore to be released. 
		//Leading to it sleeping until the USART interrupt releases the semaphore when a message is in the pipe
		if( xSemaphoreTake( xSemaphoreReader, ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			if( xSemaphoreTake( xSemaphoreReader, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{

				USART_getString(message,63);	//60 characters + \r + \n + \0.
				
				len =strlen(message)-2;	//Cut of so there is only 60 chars
				charSum += len;		//Update the global total number of char recieved 
				message[len] = 0;	//Moves the \0 to the end of the string
				
				strncat(message, clearMsg,60-len);	//Fill the remainder (if there is one that is) with empty space. 

				dip204_set_cursor_position(1,1);
				dip204_write_string(message);

				(&AVR32_USART1)->IMR.rxrdy;		//Reads the Interrupt Mask register to clear this interrupt.
				(&AVR32_USART1)->IER.rxrdy = 1;		//Enables the interrupt again.

				xSemaphoreGive(xSemaphoreReader);
				xSemaphoreGive(xSemaphoreStatus);
				
			}
		}
	}
}

void StatusTask(void * pvParameters)
{
	for(;;)
	{
		//---------------display-------------Print every row separate
		if( xSemaphoreTake( xSemaphoreStatus, ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			writeNrOfChars(charSum);	//Print the number of total recieved char on the LCD.
		}
	}
}

void SwitchTask(void * pvParameters)
{
	unsigned int btn_state;
	portTickType xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		btn_state = AVR32_GPIO.port[BUTTON_PORT].pvr & BUTTON0_PIN;
		if(btn_state==0)
		{
			writeNrOfChars(charSum);	//Write the first update of total number of char before StatusTasks switch on
			vTaskResume(sHandle);
			
			vTaskDelay(10000);	//Let StatusTask run for 10 sec 
			
			vTaskSuspend(sHandle);	//Then kill it again
			dip204_set_cursor_position(1,4);
			dip204_write_string("                    ");		//Empty the display afterwards 
		}
		vTaskDelayUntil(&xLastWakeTime, 100);	//Waits 100ms between checking the button
	}
}
