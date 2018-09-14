#include "driver.h"
#include "evk1100.h"

void USART_init(volatile avr32_usart_t *usart)
{
	volatile int a=0;

	usart->CR.rstrx = 0; // Reset Receiver
	usart->CR.rsttx = 0; // Reset Transmitter
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
	usart->MR.sync = 0; // FUCKING!! asynchronous mode!
	usart->MR.par = 4; // No parity bit
	usart->MR.nbstop = 0; // 1 bit stop bit.
	
	usart->MR.chmode = 0; // normal channel mode
	
	usart->MR.msbf = 0; // Least significant bit first
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
	pmart->OSCCTRL0.mode = 4;
	pmart->OSCCTRL0.startup = 6;
	pmart->MCCTRL.osc0en = 1;
	pmart->MCCTRL.mcsel = 1;
	volatile unsigned long temp = pmart->clkmask[2];
	if ((temp & (1<<9)) == 0)
	{
		pmart->clkmask[2] = temp + (1 << 9);
		a = 2;
	}
	
	//This is the Baud generator controller set.
	usart->BRGR.fp = 0; // No fractions needed.
	usart->BRGR.cd = 78; // CD = 12 000 000 / (16*9 600) => CD = 78,125. 

	//Enable pages 45+179 in data sheet and uc3a0512.h row 1090.
	volatile avr32_gpio_port_t *usartIO = &AVR32_GPIO.port[0]; // Fix define
	usartIO->pmr0c = 1 << 5; //RXD
	usartIO->pmr1c = 1 << 5; //RXD
	usartIO->gperc = 1 << 5; //RXD
	
	usartIO->pmr0c = 1 << 6; //TXD
	usartIO->pmr1c = 1 << 6; //TXD
	usartIO->gperc = 1 << 6; //TXD

	usartIO->pmr0c = 1 << 7; //CLK
	usartIO->pmr1c = 1 << 7; //CLK
	usartIO->gperc = 1 << 7; //CLK
	
	
}
void mdelay(int ms)
{
	long volatile cycles = 1050*ms;
	while (cycles != 0)
	{
		cycles--;
	}
}
char USART_getChar()
{
	char toTRX ;
	volatile avr32_gpio_port_t *a = &AVR32_GPIO.port[BUTTON0_PORT];
	volatile unsigned long btnstat;
	while(AVR32_USART1.CSR.rxrdy==0)
	{
		btnstat = a->pvr & BUTTON0_PIN;
		if(btnstat == 0)
		{
			USART_reset();
		}
	}
	toTRX = (char)AVR32_USART1.RHR.rxchr;
	
	return toTRX;
}
void USART_putChar(char c)
{
	while(1)
	{
		if(AVR32_USART1.CSR.txrdy == 1)
		{	
			AVR32_USART1.THR.txsynh = 0;
			AVR32_USART1.THR.txchr = c;
			break;
		}
	}
}
void USART_reset()
{
	//Reset the control register
	volatile avr32_usart_t *usart = &AVR32_USART1;

	usart->CR.rstrx = 1; // Reset Receiver
	usart->CR.rsttx = 1; // Reset Transmitter
	usart->CR.rststa = 1; // Resets Status bit
	//Clear the bits
	usart->CR.rstrx = 0;
	usart->CR.rsttx = 0;
	usart->CR.rststa = 0;
	
	usart->CR.rxen = 1;	// Enable receiver
	usart->CR.rxdis = 0; // Don't disable receiver
	usart->CR.txen = 1; // Enable Transmitter
	usart->CR.rxdis = 0; // Don't disable receiver

}
void USART_getString(char *message)
{
	int i=0;
	do
	{
		message[i] = USART_getChar();
		i++;
	}
	while(message[i-1]!='\n');
	message[i] = 0;
}
void USART_putString(char *message)
{
	int i=0;
	while (message[i]!=0)
	{
		USART_putChar(message[i]);
		i++;
	}
	
}