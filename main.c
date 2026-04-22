/* CLI  based system */

#define F_CPU 16000000UL

#include "CLI/CLI.h"
#include <avr/interrupt.h>

int main(void){

    USART_init(9600);                 // UART initialization at 9600 BUAD 
    pwm_init(1000,pwm_TIMER1);       // Frequency = 1000 and TIMER1 PWM initialization
    timer_init();                   // TIMER0 initialization for timing (counting)
    
    sei();                         // Global interrupt enable

    USART_print("Enter commands\n");

    while(1){
        
        CLI_process();
        pwm_src();
        led_src();
    }
}