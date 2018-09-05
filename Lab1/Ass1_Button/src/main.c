# include "board.h"
# include "compiler.h"
/* Define the ports and pins to be used */
// The maximum number of pins in a port
# define GPIO_MAX_PIN_NUMBER 32
// This is the port which LED0_GPIO is located on
# define LED0_PORT ( LED0_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED0_PIN ( LED0_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1 - bit written to the bit position of the GPIO pin
# define LED0_BIT_VALUE (1 << LED0_PIN )

//Define how Button 1 is set up
#define BUTTON0_PORT (GPIO_PUSH_BUTTON_0 >> 5)#define BUTTON0_PIN (1 << (GPIO_PUSH_BUTTON_0 & 0x1f))

void initLED(void);
void mdelay(int ms);
void initButton(void);


void mdelay(int ms)
{
	long volatile cycles = 10*ms;
	while (cycles != 0)
	{
		cycles--;
	}
}
void initButton(void)
{
	volatile avr32_gpio_port_t * button0_port;
	button0_port = &AVR32_GPIO.port[BUTTON0_PORT];
	button0_port->gpers = BUTTON0_PIN;
	button0_port->oderc = BUTTON0_PIN;
}
void initLED(void)
{
	/*
	* First make access to the GPIO port registers
	* shorter to type , just for our convenience
	*/
	volatile avr32_gpio_port_t * led0_port;
	led0_port = & AVR32_GPIO.port[LED0_PORT];
	/*
	* Choose GPIO peripheral function ,
	* by writing a 1 to gpers , GPIO Enable Register Set
	*/
	led0_port->gpers = LED0_BIT_VALUE;
	/*
	* Set the output value register to 1 in order to
	* turn the LED off (1= off , 0= on in this case ) ,
	* by writing a 1 to ovrs , Output Value Register Set
	*/
	led0_port->ovrs = LED0_BIT_VALUE;
	/*
	* Set the output enable register ,
	* by writing a 1 to oders , Output Drive Enable
	* Register Set
	*/
	led0_port->oders = LED0_BIT_VALUE;
};
int main(void)
{
	initLED();
	initButton();
	/* Main loop that will toggle a single bit on the GPIO port
	*/
	volatile int i = 0;
	
	while(1)
	{
		volatile unsigned long button_state = AVR32_GPIO.port[BUTTON0_PORT].pvr & BUTTON0_PIN;
		while(button_state==0)
		{
				/* toggle register bit */
			AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE;
			mdelay(100);
			i++;
		}
	}
}