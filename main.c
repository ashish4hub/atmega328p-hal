#include "uart_tx.h"
#include "uart_rx.h"
#include <string.h>
#include <avr/interrupt.h>
int main(void){
    uart_init();
    rx_ini();
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
            }
            
        }
    }
    return 0;
}