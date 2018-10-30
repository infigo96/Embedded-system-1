#include "game_framwork.h"
__attribute__((__interrupt__)) static void tick(void)
{
	increment_tick();
	tc_read_sr(&AVR32_TC, 0);
}

void framework_init(){
		display_init();
		initGame();
		volatile avr32_tc_t *tc = &AVR32_TC;
		initCLK(tc);
		tc_start(tc,0);
		onTick();
}

void GPIO_init(){
	avr32_gpio_port_t *button_port = &AVR32_GPIO.port[BUTTON_PORT];
	button_port->gpers = BUTTON_0_PIN; button_port->oderc = BUTTON_0_PIN;
	button_port->gpers = BUTTON_1_PIN; button_port->oderc = BUTTON_1_PIN;
	button_port->gpers = BUTTON_2_PIN; button_port->oderc = BUTTON_2_PIN;
	
	avr32_gpio_port_t *joystick_port = &AVR32_GPIO.port[JOYSTICK_PORT];
	joystick_port->gpers = JOYSTICK_UP_PIN; joystick_port->oderc = JOYSTICK_UP_PIN;
	joystick_port->gpers = JOYSTICK_DOWN_PIN; joystick_port->oderc = JOYSTICK_DOWN_PIN;
	joystick_port->gpers = JOYSTICK_LEFT_PIN; joystick_port->oderc = JOYSTICK_LEFT_PIN;
	joystick_port->gpers = JOYSTICK_RIGHT_PIN; joystick_port->oderc = JOYSTICK_RIGHT_PIN;
	
}
void initCLK(volatile avr32_tc_t *tc){
	global_time = 0;
	static tc_waveform_opt_t opt;
	unsigned int channel = 0; //Compiler complains if 0 is written directly
	opt.channel = channel;
	opt.wavsel = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER;
	opt.tcclks = TC_CLOCK_SOURCE_TC4; //CLK / 128 50+14=64
	tc_init_waveform(tc, &opt);
	tc_write_rc(tc,channel,9375); //12MHz / (128 * 9375) = 0.1 second between interrupts
	
	//Add timer to interrupts
	Disable_global_interrupt();
	INTC_init_interrupts();
	tc_interrupt_t bitfield;
	bitfield.cpcs = 1;
	INTC_register_interrupt(&tick,AVR32_TC_IRQ0 ,AVR32_INTC_INT0); //Set time/-counter interrupts
	tc_configure_interrupts(tc,0,&bitfield);
	Enable_global_interrupt();
}
void increment_tick(){
	global_time++;
}


DIRECTIONS getJoystickInput(){
	avr32_gpio_port_t *gpio = &AVR32_GPIO.port[JOYSTICK_PORT];
	if((gpio->pvr & JOYSTICK_LEFT_PIN) == 0)
		return LEFT;
	if((gpio->pvr & JOYSTICK_RIGHT_PIN) == 0)
		return RIGHT;
	if((gpio->pvr & JOYSTICK_UP_PIN) == 0)
		return UP;
	if((gpio->pvr & JOYSTICK_DOWN_PIN) == 0)
		return DOWN;
	if((gpio->pvr & JOYSTICK_PUSH_PIN) == 0)
		return PUSH;
	return NONE;
}
int getButtonInput(int button){
	avr32_gpio_port_t *gpio = &AVR32_GPIO.port[BUTTON_PORT];
	switch(button){
		case 0:
			if((gpio->pvr & BUTTON_0_PIN) == 0)
			return 1;
			break;
		case 1:
			if((gpio->pvr & BUTTON_1_PIN) == 0)
			return 1;
			break;
		case 2:
			if((gpio->pvr & BUTTON_2_PIN) == 0)
			return 1;
			break;
	}
	return 0;
}

void drawAtPos(char c, int x, int y){
	dip204_set_cursor_position(x+1,y+1);
	dip204_write_data(c);
}

void delay(int t){
	unsigned long int i = global_time;
	while (i+t>global_time)
	{
	}
}