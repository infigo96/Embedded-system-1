#include "tasks.h"
#include "dip204.h"
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

void Reader(void * pvParameters)
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
		if( xSemaphoreTake( xSemaphoreReader, ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			if( xSemaphoreTake( xSemaphoreReader, ( portTickType ) portMAX_DELAY) == pdTRUE )
			{

				USART_getString(message,62);
				len =strlen(message)-2;
				charSum += len;
				message[len] = 0;
				strncat(message, clearMsg,60-len);

				dip204_set_cursor_position(1,1);
				dip204_write_string(message);
				//dip204_printf_string(message); //FUCKTARD FUNCTION
				
				dip204_set_cursor_position(1,4);
				dip204_write_string("hej Jag klhlk");

				(&AVR32_USART1)->IMR.rxrdy;		//Reads the Interrupt Mask register to clear this interrupt.
				(&AVR32_USART1)->IER.rxrdy = 1;

				xSemaphoreGive(xSemaphoreReader);
				
			}
		}
	}
}

void Consumer(void * pvParameters)
{
	int byteCount;
	char byte[2];
	byte[2] = 0;

	//vTaskDelay(900);
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	if( xSemaphoreTake( xSemaphoreStatus, ( portTickType ) portMAX_DELAY) == pdTRUE )
	{
		if( xSemaphoreTake( xSemaphoreStatus, ( portTickType ) portMAX_DELAY) == pdTRUE )
		{
			//USART_getString(message,60);
			//len =strlen(message)-2;
			//charSum += len;
			//message[len] = 0;
//
			//dip204_set_cursor_position(1,1);
			//dip204_clear_display();
			//dip204_write_string(message);
			////dip204_printf_string(message); //FUCKTARD FUNCTION
			//(&AVR32_USART1)->IER.rxrdy = 1;
			//xSemaphoreGive(xSemaphoreStatus);
			
		}
	}
}