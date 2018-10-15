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
	dip204_write_string("          ");
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
	int len;
	for(;;)
	{
		//---------------display-------------Print every row separate
		if( xSemaphoreTake( xSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			if( xSemaphoreTake( xSemaphore, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{
				USART_getString(message,60);
				len = strlen(message)-2;
				charSum += len;
				message[len] = 0;
				dip204_set_cursor_position(1,1);
				dip204_clear_display();
				dip204_write_string(message);
				(&AVR32_USART1)->IER.rxrdy = 1;
				xSemaphoreGive(xSemaphore);
				xSemaphoreGive(xSemaphore2);
			}
		}
	}
}

void StatusTask(void * pvParameters)
{
	for(;;)
	{
		//---------------display-------------Print every row separate
		if( xSemaphoreTake( xSemaphore2, ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			writeNrOfChars(charSum);
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
			writeNrOfChars(charSum);
			vTaskResume(sHandle);
			
			vTaskDelay(10000);
			
			vTaskSuspend(sHandle);
			dip204_set_cursor_position(1,4);
			dip204_write_string("          ");
		}
		vTaskDelayUntil(&xLastWakeTime, 100);
	}
}
