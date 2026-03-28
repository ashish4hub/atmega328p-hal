#include "uart/uart.h"
#include "timer/timer.h"
#include "pwm/pwm.h"
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdint.h>
int main(void){
    uart_init(9600);
    pwm_init(1000);   // Frequency = 1000
    timer_ini();
    uint32_t wait = get_ticks();  /* Time stamps for timing logic in pwm and timer driver */
    uint32_t led1_wait = get_ticks();
    uint32_t led2_wait = get_ticks();
    uint16_t led1_duty = 0;
    uint16_t led2_duty = 0;
    pwm_enb_channel(pwm_CH1A);    /* PWM channel (pin) selection */
    pwm_enb_channel(pwm_CH1B);
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
        if(nb_wait_ms(&wait,1000)){
            uart_print("Hello world!\r\n"); /* Printing message every 1 sec (1000ms) */
        }
    }
}