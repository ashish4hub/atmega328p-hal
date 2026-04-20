/* AVR drivers application */
/* CLI  based system */



#define F_CPU 16000000UL

#include "uart/uart.h"
#include "timer/timer.h"
#include "pwm/pwm.h"
#include "adc/adc.h"
#include "CLI/CLI.h"
#include "src/pwm_src.h"

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define adc_led PB0      // Led 

int main(void){

    USART_init(9600);                 // UART initialization at 9600 BUAD 
    pwm_init(1000,pwm_TIMER1);       // Frequency = 1000 and TIMER1 PWM initialization
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

    /* Wait flag */
    uint8_t wait_flag1 = 1;
    uint8_t wait_flag2  = 1;

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
        
        CLI_process();
        
        if(pwm_enable == 1){
            start_pwm();
            run_pwm();
        }
        if(pwm_enable == 0){
            stop_pwm();
        }
    }
}