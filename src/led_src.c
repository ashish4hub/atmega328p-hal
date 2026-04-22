/* Led service */

#include "led_src.h"

led_state_t led_state = state_led_off;

void led_set_state(led_state_t state){

    switch (state)
    {
    case state_led_on:
        led_state = state_led_on;
        break;
    
    case state_led_off:
        led_state = state_led_off;
        break;
    }
}

/* LED SRC */
void led_src(void){

    switch (led_state)
    {
    case state_led_on:
        led_on();
        break;
    
    case state_led_off:
        led_off();
        break;
    }
}
