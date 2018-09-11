#include "driver.h"


void USART_init(volatile avr32_usart_t *usart)
{
	usart->CR.rstrx = 1; // Reset Receiver
	volatile int a=0;
	usart->CR.rsttx = 1; // Reset transmitter
	usart->CR.rxen = 1;	// Enable transmitter
	usart->CR.rxdis = 0; // Don't disable receiver
	usart->CR.txen = 1; // Enable Transmitter
	usart->CR.txdis = 0; // Don't disable transmitter
	usart->CR.rststa = 0; // Resets Status bit.
	usart->CR.sttbrk = 0; // Don't start break
	usart->CR.stpbrk = 0; // Don't stop break
	usart->CR.sttto = 0; // Don't start timeout
	usart->CR.senda = 0; // No effect
	usart->CR.rstit = 0; // No effect
	usart->CR.rstnack = 0; // No effect
	usart->CR.retto = 0; // No effect
	usart->CR.rtsen = 0; // No effect
	usart->CR.rtsdis = 0; // No effect

	usart->MR.mode = 0; // Normal mode
	usart->MR.usclks = 0; // Clock usart
	usart->MR.chrl = 3; // 8-bits char length
	usart->MR.sync = 1; // Synchronous mode
	usart->MR.par = 4; // No parity bit
	usart->MR.nbstop = 0; // 1 bit stop bit.
	usart->MR.chmode = 0; // normal channel mode
	usart->MR.msbf = 0; // Least significant bit first
	usart->MR.mode9 = 0; // see usart->MR.chrl
	usart->MR.clko = 1; // CLK = Usart_clock
	usart->MR.over = 0; // 16 bit over-sampling (not used in synchronous mode)
	usart->MR.inack = 1; // no nacks generated
	usart->MR.dsnack = 0; // no parity bit used so this does nothing
	usart->MR.var_sync = 1; // Update when character is writen into THR register
	usart->MR.max_iteration = 0; // Not used
	usart->MR.filter = 0; // no filtering or receive line
	usart->MR.man = 0; // Manchester is disabled
	usart->MR.modsync = 0; // irrelevant (manchester)
	usart->MR.onebit = 1; // Start Frame delimiter is One Bit.

	AVR32_PM->MCCTRL.mcsel = 0;
	AVR32_PM->C

	usart->BRGR.fp = 0; // No fractions needed.
	usart->BRGR.cd = 1250; // CD = 12 000 000 / 9 600 => CD = 1250.

	usart->CR.rstrx = 0; // Reset Receiver
	usart->CR.rsttx = 0; // Reset transmitter

	
}

volatile char USART_getChar()
{
	volatile char hej = 'b';
	if(AVR32_USART1.CSR.rxrdy==1)
	{
		hej = (char)AVR32_USART1.RHR.rxchr;
	}
	
	return hej;
}
void USART_putChar(char c)
{
	AVR32_USART1.THR.txsynh = 0;
	AVR32_USART1.THR.txchr = c;
	volatile int a=0;
}
void USART_reset()
{
	volatile int i = 0; 
}