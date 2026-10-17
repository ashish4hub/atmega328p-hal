#ifndef LED_SRC_H
#define LED_SRC_H

#include "../driver/led/led.h"

/* State based control */
typedef enum {
    state_led_on,
    state_led_off
} led_state_t;


void led_set_state(led_state_t state);
void led_src(void);

#endif