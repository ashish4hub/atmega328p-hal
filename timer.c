#include <avr/io.h>
#include <avr/interrupt.h>
volatile uint32_t tick = 0;
void timer_config(void){
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS00) | (1 << CS01);
    OCR0A = 249;
    TIMSK0 |= (1 << OCIE0A);
}
ISR(TIMER0_COMPA_vect){
    tick++;
}
uint8_t nb_wait_ms(uint32_t time){
    tick = 0;
    if(tick == time){
        return 0;
    }
    else{
        return 1;
    }
}

