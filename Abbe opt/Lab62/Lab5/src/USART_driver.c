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
		. baudrate = 9600 ,
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