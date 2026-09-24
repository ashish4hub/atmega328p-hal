/* CLI  based system */

#define F_CPU 16000000UL

#include "CLI/CLI.h"

int main(void){

    USART_init(9600);                   // UART initialization at 9600 BUAD 
    timer_init();                     // TIMER0 initialization for timing (counting)
    HCSR04_init();                   // HCSR04 initialization 

    /* ADC Configuration */
    ADC_config_t adc_config = {
        .reference = ADC_REF_VCC,
        .prescaler = ADC_PS_128,
        .mode = ADC_INTERRUPT
    };

    ADC_init(&adc_config);      // ADC initialization

    sei();                         // Global interrupt enable

    USART_print("Enter commands\n");

    while(1){

        CLI_process();
        HCSR04_src();
        MOIST_src();
        pwm_src();
        led_src();
        MSG_src();
    }
}