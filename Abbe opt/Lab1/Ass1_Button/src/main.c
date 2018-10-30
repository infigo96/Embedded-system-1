# include "board.h"
# include "compiler.h"
/* Define the ports and pins to be used */
// The maximum number of pins in a port
# define GPIO_MAX_PIN_NUMBER 32
// This is the port which LED0_GPIO is located on
# define LED0_PORT ( LED0_GPIO / GPIO_MAX_PIN_NUMBER )
# define LED1_PORT ( LED1_GPIO / GPIO_MAX_PIN_NUMBER )
# define LED2_PORT ( LED2_GPIO / GPIO_MAX_PIN_NUMBER )
# define LED3_PORT ( LED3_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED0_PIN ( LED0_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
# define LED1_PIN ( LED1_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
# define LED2_PIN ( LED2_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
# define LED3_PIN ( LED3_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1 - bit written to the bit position of the GPIO pin
# define LED0_BIT_VALUE (1 << LED0_PIN )
# define LED1_BIT_VALUE (1 << LED1_PIN )
# define LED2_BIT_VALUE (1 << LED2_PIN )
# define LED3_BIT_VALUE (1 << LED3_PIN )

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
	volatile avr32_gpio_port_t * led1_port;
	volatile avr32_gpio_port_t * led2_port;
	volatile avr32_gpio_port_t * led3_port;

	led0_port = & AVR32_GPIO.port[LED0_PORT];
	led1_port = & AVR32_GPIO.port[LED1_PORT];
	led2_port = & AVR32_GPIO.port[LED2_PORT];
	led3_port = & AVR32_GPIO.port[LED3_PORT];
	/*
	* Choose GPIO peripheral function ,
	* by writing a 1 to gpers , GPIO Enable Register Set
	*/
	led0_port->gpers = LED0_BIT_VALUE;
	led1_port->gpers = LED1_BIT_VALUE;
	led2_port->gpers = LED2_BIT_VALUE;
	led3_port->gpers = LED3_BIT_VALUE;
	/*
	* Set the output value register to 1 in order to
	* turn the LED off (1= off , 0= on in this case ) ,
	* by writing a 1 to ovrs , Output Value Register Set
	*/
	led0_port->ovrs = LED0_BIT_VALUE;
	led1_port->ovrs = LED1_BIT_VALUE;
	led2_port->ovrs = LED2_BIT_VALUE;
	led3_port->ovrs = LED3_BIT_VALUE;
	/*
	* Set the output enable register ,
	* by writing a 1 to oders , Output Drive Enable
	* Register Set
	*/
	led0_port->oders = LED0_BIT_VALUE;
	led1_port->oders = LED1_BIT_VALUE;
	led2_port->oders = LED2_BIT_VALUE;
	led3_port->oders = LED3_BIT_VALUE;
};
int main(void)
{
	initLED();
	initButton();
	/* Main loop that will toggle a single bit on the GPIO port
	*/
	volatile long button_state;
	volatile avr32_gpio_port_t * button0_port;
	button0_port = &AVR32_GPIO.port[BUTTON0_PORT];
	while(1)
	{
		button_state = button0_port->pvr & BUTTON0_PIN;
		//#---
		AVR32_GPIO.port[LED1_PORT].ovrs = LED1_BIT_VALUE;
		AVR32_GPIO.port[LED0_PORT].ovrc = LED0_BIT_VALUE;
		mdelay(100);
		//-#--
		AVR32_GPIO.port[LED0_PORT].ovrs = LED0_BIT_VALUE;
		AVR32_GPIO.port[LED1_PORT].ovrc = LED1_BIT_VALUE;
		mdelay(100);
		//--#-
		AVR32_GPIO.port[LED1_PORT].ovrs = LED1_BIT_VALUE;
		AVR32_GPIO.port[LED2_PORT].ovrc = LED2_BIT_VALUE;
		mdelay(100);
		//---#
		AVR32_GPIO.port[LED2_PORT].ovrs = LED2_BIT_VALUE;
		AVR32_GPIO.port[LED3_PORT].ovrc = LED3_BIT_VALUE;
		mdelay(100);
		//--#-
		AVR32_GPIO.port[LED3_PORT].ovrs = LED3_BIT_VALUE;
		AVR32_GPIO.port[LED2_PORT].ovrc = LED2_BIT_VALUE;
		mdelay(100);
		//-#--
		AVR32_GPIO.port[LED2_PORT].ovrs = LED2_BIT_VALUE;
		AVR32_GPIO.port[LED1_PORT].ovrc = LED1_BIT_VALUE;
		mdelay(100);

	
	}
}