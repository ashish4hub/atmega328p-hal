#include "uart/uart_tx.h"
#include "uart/uart_rx.h"
#include "timer/timer.h"
#include <string.h>
#include <avr/interrupt.h>
#include <stdint.h>
int main(void){
    uart_init();
    rx_ini();
    timer_config();
    DDRB |= (1 << PB5);
    DDRB |= (1 << PB4);
    sei();
    while(1){
        uart_print("hello world!\r\n");
        if(read_line()){
            if (strcmp(line, "hello"))
            {
                uart_print("uart setup done!");
                PORTB |= (1 << PB5);
                PORTB |= (1 << PB4);
                if(nb_wait_ms(1000)){
                    PORTB &= ~(1 << PB5);
                    PORTB &= ~(1 << PB4);
                }
            }
            
        }
    }
    return 0;
}