/* AVR drivers application */

#define F_CPU 16000000UL

#include "uart/uart.h"
#include "timer/timer.h"
#include "pwm/pwm.h"
#include "adc/adc.h"
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define adc_led PB4      // Led 

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
    uint32_t usart_wait = get_ticks();

    uint8_t wait_flag1 = 1;

    /* Duty cycle variables for led */
    uint8_t led1_duty = 0;

    sei();        // Global interrupt enable

    /* Initial message */
    USART_print("Type start\n");

    /* State */
    typedef enum {
        STATE_IDLE,
        STATE_MENU,
        STATE_ADC,
        STATE_PWM
    } sys_state;

    sys_state state = STATE_IDLE;    // Initially at IDLE state

    while(1){

        /* Start command and operation menu */
        if(USART_read_line()){
            if(strcmp(line,"START") == 0){
                state = STATE_MENU;
                USART_print("Choose operation\n");
                USART_print("1. Start ADC\n");
                USART_print("2. Start PWM\n");
            }
            
            /* ADC mode */
            else if(strcmp(line,"Start ADC") == 0){
                state = STATE_ADC;
                USART_print("ADC mode activated!\n");
            }

            /* PWM mode */
            else if(strcmp(line,"Start PWM") == 0){
                state = STATE_PWM;
                USART_print("PWM started\n");
            }
        }

        /* State based system control */
        switch (state){

        case STATE_ADC:
        if(nb_wait_ms(&adc_wait,5)){
            ADC_start(ADC_CH0);
        }
        if(ADC_done()){
            if(wait_flag1 == 1){
                USART_print("Conversion done!\n");
                wait_flag1 = 0;
            }
            ADC_result = ADC_get_result();
            if(ADC_result > 513){
                PORTB |= (1 << adc_led);
            }
            else{
                PORTB &= ~(1 << adc_led);
            }
        }
            break;
            case  STATE_PWM:
            pwm_set(pwm_CH1A,led1_duty);
            if(nb_wait_ms(&led1_wait,10)){
                led1_duty ++;
                if(led1_duty > 100){
                    led1_duty = 0;
                }
            }
            break;
        }
        if(nb_wait_ms(&usart_wait,5000)){
            wait_flag1 = 1;
        }
    }
}