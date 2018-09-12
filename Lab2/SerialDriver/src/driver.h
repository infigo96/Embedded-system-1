#ifndef DRIVER_H
#define DRIVER_H

#include "avr32\usart_400.h"
#include "avr32\uc3a0512.h"
#include "avr32\pm_231.h"
#include "avr32\spi_1991.h"

void USART_init(volatile avr32_usart_t * usart);
char USART_getChar();
void USART_putChar(char c);
void USART_reset();
int base2converter(int exp);

#endif