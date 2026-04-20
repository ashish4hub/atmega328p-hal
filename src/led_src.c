/* Led service */

#include "led_src.h"

/* LED on function */
void led_on(void){
    PORTB |= (1 << PB0);
}

/* LED off function */
void led_off(void){
    PORTB &= ~(1 << PB0);
}
