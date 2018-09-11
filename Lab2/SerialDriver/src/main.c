# include "board.h"
# include "compiler.h"
# include <driver.h>

# define GPIO_MAX_PIN_NUMBER 32

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	volatile avr32_usart_t *usart = &AVR32_USART1;
	USART_init(usart);

	/* Insert application code here, after the board has been initialized. */
}
