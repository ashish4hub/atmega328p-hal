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

    while(1){
        uart_print("hello world!\r\n");
        if(read_line()){
            if (strcmp(line, "hello") == 0)
            {
                uart_print("uart setup done!");
                PORTB |= (1 << PB5);
                PORTB |= (1 << PB4);
                if(nb_wait_ms(&wait,1000)){
                    PORTB &= ~(1 << PB5);
                    PORTB &= ~(1 << PB4);
                }
            }
            
        }
    }
    return 0;
}