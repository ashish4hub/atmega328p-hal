#include "uart/uart.h"
#include "timer/timer.h"
#include "pwm/pwm.h"
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdint.h>

int main(void){

    uart_init(9600);                 // UART initialization at 9600 BUAD 
    pwm_init(1000,pwm_TIMER1);      // Frequency = 1000 and TIMER1 PWM initialization
    pwm_init(5000,pwm_TIMER2);     // Frequency = 5000 and TIMER2 PWM initialization
    timer_ini();                  // TIMER0 initialization for timing (counting)

    /* Time stamps for timing logic in pwm and timer driver */
    uint32_t uart_wait = get_ticks();  
    uint32_t led1_wait = get_ticks();
    uint32_t led2_wait = get_ticks();
    uint32_t led3_wait = get_ticks();

    /* Duty cycle variables for led */
    uint8_t led1_duty = 0;
    uint8_t led2_duty = 0;
    uint8_t led3_duty = 0;

    sei();

    while(1){

        if(nb_wait_ms(&led1_wait,20)){
            pwm_set(pwm_CH1A,led1_duty);    /* Set duty cycle for pwm pin */

            led1_duty++;

            if(led1_duty > 100){
                led1_duty = 0;
            }
        }

        if(nb_wait_ms(&led2_wait,40)){
            pwm_set(pwm_CH1B,led2_duty);

            led2_duty++;

            if(led2_duty > 100){
                led2_duty = 0;             /*  */
            }
        }

        if(nb_wait_ms(&led3_wait,60)){
            pwm_set(pwm_CH2B, led3_duty);

            led3_duty++;

            if(led3_duty > 100){
                led3_duty = 0;
            }
        }
        
        if(nb_wait_ms(&uart_wait,1000)){
            uart_print("Hello world!\r\n"); /* Printing message every 1 sec (1000ms) */
        }
    }
}