#include "uart/uart.h"
#include "timer/timer.h"
#include <string.h>
#include <avr/interrupt.h>
#include <stdint.h>
int main(void){
    uart_init(9600);
    timer_ini();
    uint32_t wait = 0;  // timestamp
    DDRB |= (1 << PB5);
    DDRB |= (1 << PB4);
    sei();
    wait = get_ticks();

    while(1){
        if(nb_wait_ms(&wait,1000)){
            uart_print("Hello world!\r\n");
            PORTB ^= (1 << PB5);
        }
    }
    return 0;
}