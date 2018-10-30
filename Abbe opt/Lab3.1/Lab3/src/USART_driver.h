#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include "avr32\usart_400.h"
#include "avr32\uc3a0512.h"
#include "avr32\pm_231.h"
#include "avr32\spi_1991.h"
#include "evk1100.h"
# include "board.h"
# include "compiler.h"

void mdelay(int ms);
void USART_init(volatile avr32_usart_t * usart);
char USART_getChar();
void USART_putChar(char c);
void USART_getString(char *message);
void USART_putString(char *message);

void USART_reset();
int base2converter(int exp);

#endif