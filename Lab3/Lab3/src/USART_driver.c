#include "USART_driver.h"
#include "evk1100.h"


// USART_init sets bits in the USART register that initiate the Usart1 system.
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
	usart->MR.sync = 0; // asynchronous mode!
	usart->MR.par = 4; // No parity bit
	usart->MR.nbstop = 0; // 1 bit stop bit.
	usart->MR.chmode = 0; // normal channel mode
	usart->MR.msbf = 0; // Least significant bit first
	usart->MR.mode9 = 0; // see usart->MR.chrl
	usart->MR.clko = 1; // CLK = Usart_clock
	usart->MR.over = 0; // 16 bit over-sampling
	usart->MR.inack = 1; // no nacks generated
	usart->MR.dsnack = 0; // no parity bit used so this does nothing
	usart->MR.var_sync = 1; // Update when character is written into THR register
	usart->MR.max_iteration = 0; // Not used
	usart->MR.filter = 0; // no filtering or receive line
	usart->MR.man = 0; // Manchester is disabled
	usart->MR.modsync = 0; // irrelevant (Manchester)
	usart->MR.onebit = 1; // Start Frame delimiter is One Bit.

	
	// This part enables the 12Mhz system clock. 
	// Osccilator0 settings in Power Management and mask for usart access.
	volatile avr32_pm_t *pmart = &AVR32_PM; // Address of registry
	pmart->OSCCTRL0.mode = 4;
	pmart->OSCCTRL0.startup = 6;
	pmart->MCCTRL.osc0en = 1;
	pmart->MCCTRL.mcsel = 1;
	
	//This is the Baud generator controller set.
	usart->BRGR.fp = 0; // No fractions needed.
	usart->BRGR.cd = 78; // CD = 12 000 000 / (16*9 600) => CD = 78,125. 

	//Enable pages 45+179 in data sheet and uc3a0512.h row 1090.
	volatile avr32_gpio_port_t *usartIO = &AVR32_GPIO.port[0];
	usartIO->pmr0c = 1 << 5; //RXD
	usartIO->pmr1c = 1 << 5; //RXD
	usartIO->gperc = 1 << 5; //RXD
	
	usartIO->pmr0c = 1 << 6; //TXD
	usartIO->pmr1c = 1 << 6; //TXD
	usartIO->gperc = 1 << 6; //TXD

	usartIO->pmr0c = 1 << 7; //CLK
	usartIO->pmr1c = 1 << 7; //CLK
	usartIO->gperc = 1 << 7; //CLK
	
	// Initiate reset button for USART.
	volatile avr32_gpio_port_t * button0_port;
	button0_port = &AVR32_GPIO.port[BUTTON0_PORT];
	button0_port->gpers = BUTTON0_PIN;
	button0_port->oderc = BUTTON0_PIN;
}

// Polls the designated register (CSR) until pin rxrdy == 1 (is high). 
// Then it takes the char that was placed in the RHR register on rxchar pins and returns it.
char USART_getChar()
{
	char toTRX ;

	if(AVR32_USART1.CSR.rxrdy==1)
	{
		toTRX = (char)AVR32_USART1.RHR.rxchr;
		return toTRX;
	}
	else
	{
		return (char) 0;
	}
}

// Writes to the THR registry txchr pins IF the txrdy pin is 1 (high) in the CSR registry.
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

// Reset function which clears and sets bits in the Usart Control Register
void USART_reset()
{
	//Reset the control register
	volatile avr32_usart_t *usart = &AVR32_USART1;

	usart->CR.rstrx = 1; // Reset Receiver
	usart->CR.rsttx = 1; // Reset Transmitter
	usart->CR.rststa = 1; // Resets Status bit
	//Clear the bits just in case
	usart->CR.rstrx = 0;
	usart->CR.rsttx = 0;
	usart->CR.rststa = 0;
	
	usart->CR.rxen = 1;	// Enable receiver
	usart->CR.rxdis = 0; // Don't disable receiver
	usart->CR.txen = 1; // Enable Transmitter
	usart->CR.txdis = 0; // Don't disable transmitter

}

// Put string method. Buffers to fixed size char array that is defined by the calling function.
// Uses USART_putChar() to write to the register untill 'null' char found in the buffer.
void USART_putString(char *message)
{
	int i=0;
	while (message[i]!=0)
	{
		USART_putChar(message[i]);
		i++;
	}
	
}