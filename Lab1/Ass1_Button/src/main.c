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

// This is the port which LED1_GPIO is located on
# define LED1_PORT ( LED0_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED1_PIN ( LED1_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1 - bit written to the bit position of the GPIO pin
# define LED1_BIT_VALUE (1 << LED1_PIN )

// This is the port which LED2_GPIO is located on
# define LED2_PORT ( LED2_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED2_PIN ( LED2_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1 - bit written to the bit position of the GPIO pin
# define LED2_BIT_VALUE (1 << LED2_PIN )

//Define how Button 1 is set up
#define BUTTON0_PORT (GPIO_PUSH_BUTTON_0 >> 5)#define BUTTON0_PIN (1 << (GPIO_PUSH_BUTTON_0 & 0x1f))//Define how Button 1 is set up
#define BUTTON1_PORT (GPIO_PUSH_BUTTON_1 >> 5)#define BUTTON1_PIN (1 << (GPIO_PUSH_BUTTON_1 & 0x1f))//Define how Button 1 is set up
#define BUTTON2_PORT (GPIO_PUSH_BUTTON_2 >> 5)#define BUTTON2_PIN (1 << (GPIO_PUSH_BUTTON_2 & 0x1f))
// Prototypes to avoid warnings.
void initLED(void);
void mdelay(int ms);
void initButton(void);
void part1(void);
void part2(volatile int *toggle, volatile unsigned long *pressing_button);

void part1(void)
{
	volatile unsigned long button_state = AVR32_GPIO.port[BUTTON0_PORT].pvr & BUTTON0_PIN;
	while(button_state==0)
	{
		/* toggle register bit */
		AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE;
		mdelay(100);
	}	
}

void part2(volatile int *toggle, volatile unsigned long *pressing_button)
{
	volatile unsigned long button_state_1;
	volatile unsigned long button_state_2;
	
	button_state_1 = AVR32_GPIO.port[BUTTON1_PORT].pvr & BUTTON1_PIN;
	button_state_2 = AVR32_GPIO.port[BUTTON2_PORT].pvr & BUTTON2_PIN;
	//If button1 is being pressed, light the LED until the button is released
	if (button_state_1 == 0 || *toggle == 1)
	{
		//Start the light
		AVR32_GPIO.port[LED1_PORT].ovrc = LED1_BIT_VALUE;
	}
	else {
		//Turn off the light
		AVR32_GPIO.port[LED1_PORT].ovrs = LED1_BIT_VALUE;
	}
	
	if (button_state_2 == 0 && *pressing_button != 0)
	{
		if (*toggle == 0)
		*toggle = 1;
		else
		*toggle = 0;
	}
	*pressing_button = button_state_2; //Save previous state
	
}


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
	
	volatile avr32_gpio_port_t * button1_port;
	button1_port = &AVR32_GPIO.port[BUTTON1_PORT];
	button1_port->gpers = BUTTON1_PIN;
	button1_port->oderc = BUTTON1_PIN;	
	
	volatile avr32_gpio_port_t * button2_port;
	button2_port = &AVR32_GPIO.port[BUTTON2_PORT];
	button2_port->gpers = BUTTON2_PIN;
	button2_port->oderc = BUTTON2_PIN;	
}
void initLED(void)
{
	//----------------------------------------LED0-----------------------------------------------------//
	//First make access to the GPIO port registers
	volatile avr32_gpio_port_t * led0_port;
	led0_port = & AVR32_GPIO.port[LED0_PORT];

	//Choose GPIO peripheral function. By writing a 1 to gpers , GPIO Enable Register Set	
	led0_port->gpers = LED0_BIT_VALUE;

	//Set the output value register to 1 in order (turn the LED off)
	led0_port->ovrs = LED0_BIT_VALUE;

	//by writing a 1 to oders , Output Drive Enable
	led0_port->oders = LED0_BIT_VALUE;
	
	
	//----------------------------------------LED1-----------------------------------------------------//
	//First make access to the GPIO port registers
	volatile avr32_gpio_port_t * led1_port;
	led1_port = & AVR32_GPIO.port[LED1_PORT];

	//Choose GPIO peripheral function. By writing a 1 to gpers , GPIO Enable Register Set
	led1_port->gpers = LED1_BIT_VALUE;

	//Set the output value register to 1 in order (turn the LED off)
	led1_port->ovrs = LED1_BIT_VALUE;

	//by writing a 1 to oders , Output Drive Enable
	led1_port->oders = LED1_BIT_VALUE;
	
	
	//----------------------------------------LED2-----------------------------------------------------//
	//First make access to the GPIO port registers
	volatile avr32_gpio_port_t * led2_port;
	led2_port = & AVR32_GPIO.port[LED2_PORT];

	//Choose GPIO peripheral function. By writing a 1 to gpers , GPIO Enable Register Set
	led2_port->gpers = LED2_BIT_VALUE;

	//Set the output value register to 1 in order (turn the LED off)
	led2_port->ovrs = LED2_BIT_VALUE;

	//by writing a 1 to oders , Output Drive Enable
	led2_port->oders = LED2_BIT_VALUE;
};
int main(void)
{
	initLED();
	initButton();
	/* Main loop that will toggle a single bit on the GPIO port
	*/
	volatile int i = 0;
	volatile int toggle = 0;
	volatile unsigned long pressing_button = AVR32_GPIO.port[BUTTON2_PORT].pvr & BUTTON2_PIN;
	
	
	while(1)
	{
		part1();
		
		part2(&toggle, &pressing_button);
		
		//Debugging purposes
		i++;
	}
}