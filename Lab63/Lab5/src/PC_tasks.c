#include "PC_tasks.h"

//Initiates LED 0, 1 and 2.
void initLED()
{
	volatile avr32_gpio_port_t *led_port;
	led_port = & AVR32_GPIO.port[LED_PORT];
	
	led_port->gpers = 1 << LED0_PIN;
	led_port->gpers = 1 << LED1_PIN;
	led_port->gpers = 1 << LED2_PIN;

	led_port->ovrs = 1 << LED0_PIN;
	led_port->ovrs = 1 << LED1_PIN;
	led_port->ovrs = 1 << LED2_PIN;

	led_port->oders = 1 << LED0_PIN;
	led_port->oders = 1 << LED1_PIN;
	led_port->oders = 1 << LED2_PIN;
}

//Initiates Button 0, 1 and 2.
void initBUTTON(void)
{
	volatile avr32_gpio_port_t *button_port;
	button_port = & AVR32_GPIO.port[BUTTON_PORT];
	button_port->gpers = 1 << BUTTON0_PIN;
	button_port->oderc = 1 << BUTTON0_PIN;
	
	button_port->gpers = 1 << BUTTON1_PIN;
	button_port->oderc = 1 << BUTTON1_PIN;
	
	button_port->gpers = 1 << BUTTON2_PIN;
	button_port->oderc = 1 << BUTTON2_PIN;
}


//Calls writeUsart(const char * message) from a critical state to prvent message overlapping.
void writeUSART_CRT(const char * message)
{
	//Prevent task writing over each other by entering critical state during writing
	taskENTER_CRITICAL();
	writeUSART(message);
	taskEXIT_CRITICAL();
}

//Light task. Reads light and sent it to queue
void LightTask(void * pvParameters)
{
	volatile int lig_value;
	portTickType xLastWakeTime;
	const portTickType xFrequency = 500;
	xLastWakeTime = xTaskGetTickCount();
	for (;;)
	{
		adc_start(&AVR32_ADC);
		lig_value = adc_get_value(&AVR32_ADC, ADC_LIGHT_CHANNEL); // Get the light sensor value
        xQueueSendToBack( QLight, &lig_value, 0);
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}
}

//Temp task. Reads temperature and sent it to queue
void TempTask(void * pvParameters)
{
	volatile int temp_value;
	portTickType xLastWakeTime;
	const portTickType xFrequency = 2000;
	xLastWakeTime = xTaskGetTickCount();
	for (;;)
	{
		adc_start(&AVR32_ADC);
		temp_value = adc_get_value(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL); // Get the light sensor value
		xQueueSendToBack( QTemp, &temp_value, 0);
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}
}
//Potentiometer task. Reads the potentiometer and sent the value to the queue
void PotenTask(void * pvParameters)
{
	volatile int pot_value;
	portTickType xLastWakeTime;
	const portTickType xFrequency = 50;
	xLastWakeTime = xTaskGetTickCount();
	for (;;)
	{
		adc_start(&AVR32_ADC);
		pot_value = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL); // Get the light sensor value
		xQueueSendToBack( QPotent, &pot_value, 0);
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}
}
//Display task. Reads the queues and if any value has changed, display it on LCD and USART
void DisplayTask(void * pvParameters)
{
	volatile int pot_value; //Potentiometer
	volatile int lig_value; //Light
	volatile int tem_value; //Temperature
	char text1[20];
	char text2[20];
	char text3[20];
	char USARTtext[70];
	int changed;
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10;
	xLastWakeTime = xTaskGetTickCount();
	//Display start text
	dip204_set_cursor_position(1,1);dip204_write_string("LIGHT:");
	dip204_set_cursor_position(1,2);dip204_write_string("TEMP :");
	dip204_set_cursor_position(1,3);dip204_write_string("POTEN:");
	
	
	for (;;)
	{
		changed=0;
        if( xQueueReceive( QLight,&lig_value, ( portTickType ) 0 ) )
        {
			dip204_set_cursor_position(1,1);
			sprintf(text1,"LIGHT:%04d",lig_value);
			dip204_printf_string(text1);
			changed=1;
        }
        if( xQueueReceive( QTemp,&tem_value, ( portTickType ) 0 ) )
        {
	        dip204_set_cursor_position(1,2);
	        sprintf(text2,"TEMP :%04d",tem_value);
	        dip204_printf_string(text2);
			changed=1;
        }
        if( xQueueReceive( QPotent,&pot_value, ( portTickType ) 0 ) )
        {
	        dip204_set_cursor_position(1,3);
	        sprintf(text3,"POTEN:%04d",pot_value);
	        dip204_printf_string(text3);
			changed=1;
        }
		if(changed){
			sprintf(USARTtext,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n%s\n%s\n%s",text1,text2,text3);
			writeUSART(USARTtext);
		}
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}
}