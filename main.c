#include "uart/uart.h"
#include "timer/timer.h"
#include "pwm/pwm.h"
#include "adc/adc.h"
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#define adc_led PB4

int main(void){

    USART_init(9600);                 // UART initialization at 9600 BUAD 
    pwm_init(1000,pwm_TIMER1);       // Frequency = 1000 and TIMER1 PWM initialization
    pwm_init(5000,pwm_TIMER2);      // Frequency = 5000 and TIMER2 PWM initialization
    timer_init();                  // TIMER0 initialization for timing (counting)

    /* ADC configuration */
    ADC_config_t config = {
        .reference = ADC_REF_VCC,
        .mode = ADC_INTERRUPT,
        .prescaler = ADC_PS_128
    };
    
    ADC_init(&config);                     // ADC configuration
    uint16_t ADC_result = 0;
    
    DDRB |= (1 << adc_led);     // ADC alert led

    /* Time stamps for timing logic in pwm and timer driver */  
    uint32_t led1_wait = get_ticks();
    uint32_t adc_wait = get_ticks();

    /* Duty cycle variables for led */
    uint8_t led1_duty = 0;
    sei();

    while(1){
        if(nb_wait_ms(&led1_wait,50)){
            pwm_set(pwm_CH1A,led1_duty);
            led1_duty++;

            if(led1_duty > 100){
                led1_duty = 0;
            }
        }
        if(nb_wait_ms(&adc_wait,10)){
            ADC_start(ADC_CH0);
        }
        if(ADC_done()){
            ADC_result = ADC_get_result();
            
            if(ADC_result > 513){
                PORTB |= (1 << adc_led);
            }
            else {
                PORTB &= ~(1 << adc_led);
            }
        }
    }
}