#include "driver.h"
#include "evk1100.h"

void USART_init(volatile avr32_usart_t *usart)
{
	volatile int a=0;

	usart->CR.rstrx = 0; // Reset Receiver
	usart->CR.rsttx = 0; // Reset receiver
	usart->CR.rxen = 1;	// Enable receiver
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
	
	usart->MR.msbf = 1; // Least significant bit first
	usart->MR.mode9 = 0; // see usart->MR.chrl
	usart->MR.clko = 1; // CLK = Usart_clock
	usart->MR.over = 0; // 16 bit over-sampling (not used in synchronous mode)
	usart->MR.inack = 1; // no nacks generated
	usart->MR.dsnack = 0; // no parity bit used so this does nothing
	usart->MR.var_sync = 1; // Update when character is written into THR register
	usart->MR.max_iteration = 0; // Not used
	usart->MR.filter = 0; // no filtering or receive line
	usart->MR.man = 0; // Manchester is disabled
	usart->MR.modsync = 0; // irrelevant (Manchester)
	usart->MR.onebit = 1; // Start Frame delimiter is One Bit.

	//Interrupt handling for USART
	usart->IER.txempty = 1;
	usart->IER.txrdy = 1;	//Enable the transmitter interrupt 
	usart->IER.rxrdy = 1;	//Enable the receiver interrupt
	
	//Check if there is a clock in SSC that needs to be enabled --------------!!!!!!!!!!
	
	//This part enables the main system clock in usart. 
	volatile avr32_pm_t *pmart = &AVR32_PM;
	pmart->MCCTRL.mcsel = 0;
	volatile unsigned long temp = pmart->clkmask[2];
	if ((temp & (1<<9)) == 0)
	{
		pmart->clkmask[2] = temp + (1 << 9);
		a = 2;
	}
	pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);
	//This is the Baud generator controller set.
	usart->BRGR.fp = 0; // No fractions needed.
	usart->BRGR.cd = 1250; // CD = 12 000 000 / 9 600 => CD = 1250.

	volatile avr32_spi_t *spiart = &AVR32_SPI1;
	spiart->CR.spien = 1;

	//Enable pages 45+179 in data sheet and uc3a0512.h row 1090.
	volatile avr32_gpio_port_t *usartIO = &AVR32_GPIO.port[0]; // Fix define
	usartIO->pmr0c = 1 << 5; //RXD
	usartIO->pmr1c = 1 << 5; //RXD
	usartIO->gperc = 1 << 5;	//RXD
	
	usartIO->pmr0c = 1 << 6; //TXD
	usartIO->pmr1c = 1 << 6; //TXD
	usartIO->gperc = 1 << 6; //TXD

	usartIO->pmr0c = 1 << 7; //CLK
	usartIO->pmr1c = 1 << 7; //CLK
	usartIO->gperc = 1 << 7; //CLK
	
	/*usartIO = &AVR32_GPIO.port[1]; // Is not needed for our communication
	usartIO->pmr0s = 1 << 23; //DCD
	usartIO->pmr1c = 1 << 23; //DCD
	usartIO->pmr0s = 1 << 24; //DSR
	usartIO->pmr1c = 1 << 24; //DSR
	usartIO->pmr0s = 1 << 25; //DTR
	usartIO->pmr1c = 1 << 25; //DTR
	usartIO->pmr0s = 1 << 26; //RI
	usartIO->pmr1c = 1 << 26; //RI*/
	usartIO = &AVR32_GPIO.port[4]; // Fix define
	usartIO->pmr0s = 1 << 4; //RXD
	usartIO->pmr1c = 1 << 4; //RXD
	usartIO->gperc = 1 << 4; //RXD
	
	usartIO->pmr0s = 1 << 5; //TXD
	usartIO->pmr1c = 1 << 5; //TXD
	usartIO->gperc = 1 << 5; //TXD
	
	usartIO->pmr0s = 1 << 6; //CTS
	usartIO->pmr1c = 1 << 6; //CTS
	usartIO->gperc = 1 << 6; //CTS
	
	usartIO->pmr0s = 1 << 7; //RTS
	usartIO->pmr1c = 1 << 7; //RTS
	usartIO->gperc = 1 << 7; //RTS

}
void mdelay(int ms)
{
	long volatile cycles = 1050*ms;
	while (cycles != 0)
	{
		cycles--;
	}
}
volatile char USART_getChar()
{
	volatile char toTRX = 'b';
	
	while(AVR32_USART1.CSR.rxrdy==0);
	toTRX = (char)AVR32_USART1.RHR.rxchr;
	
	return toTRX;
}
void USART_putChar(char c)
{
	for(int i = 0; i < 4; i++)
	{
		if(AVR32_USART1.CSR.txrdy == 1)
		{	
			AVR32_USART1.THR.txsynh = 0;
			AVR32_USART1.THR.txchr = c;
			break;
		}
		else
		{
			mdelay(2);
		}
	}
}
void USART_reset()
{
	volatile int i = 0; 
}