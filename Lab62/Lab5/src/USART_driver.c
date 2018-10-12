/*
 * USART_drive.c
 *
 * Created: 2018-09-28 10:03:49
 *  Author: Albert
 */ 
#include "USART_driver.h"
void initUSART(){

	static const gpio_map_t USART_SERIAL_GPIO_MAP =
	{
		{ serialPORT_USART_RX_PIN , serialPORT_USART_RX_FUNCTION },
		{ serialPORT_USART_TX_PIN , serialPORT_USART_TX_FUNCTION }
	};
	static const gpio_map_t USART_DEBUG_GPIO_MAP =
	{
		{ configDBG_USART_RX_PIN , configDBG_USART_RX_FUNCTION },
		{ configDBG_USART_TX_PIN , configDBG_USART_TX_FUNCTION }
	};
	static const usart_options_t USART_OPTIONS =
	{
		. baudrate = 57600 ,
		. charlength = 8,
		. paritytype = USART_NO_PARITY ,
		. stopbits = USART_1_STOPBIT ,
		. channelmode = USART_NORMAL_CHMODE
	};
	gpio_enable_module ( USART_SERIAL_GPIO_MAP , 2);
	gpio_enable_module ( USART_DEBUG_GPIO_MAP , 2);
	usart_init_rs232 ( serialPORT_USART , & USART_OPTIONS , FOSC0 );
	usart_init_rs232 ( configDBG_USART , & USART_OPTIONS , FOSC0 );
	usart_write_line ( configDBG_USART , " Serial initialized\r\n");
}
void writeUSART(const char * message){
	usart_write_line ( serialPORT_USART , message);
}

// Polls the designated register (CSR) until pin rxrdy == 1 (is high).
// Then it takes the char that was placed in the RHR register on rxchar pins and returns it.
char USART_getChar()
{
	char toTRX ;
	//volatile avr32_gpio_port_t *a = &AVR32_GPIO.port[BUTTON0_PORT];
	volatile unsigned long btnstat;
	while(AVR32_USART1.CSR.rxrdy==0)
	{
		// Reset funktion bound to button_0
/*		btnstat = a->pvr & BUTTON0_PIN;
		if(btnstat == 0)
		{
			USART_reset();
		}*/
	}
	toTRX = (char)AVR32_USART1.RHR.rxchr;
	
	return toTRX;
}

// Get string method. Buffers to fixed size char array that is defined by the calling function.
// uses USART_getChar() and fills said buffer untill 'new line' read.
void USART_getString(char *message, int maxLength)
{
	int i=0;
	do
	{
		message[i] = USART_getChar();
		i++;
	}
	while(message[i-1]!='\n' && i < maxLength);
	if(i < maxLength)
	{
		message[i] = 0; //Sets last char as null char for USART_putString() purposes.
	}
}