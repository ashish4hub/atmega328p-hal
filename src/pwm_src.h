#ifndef PWM_SERVICE_H
#define PWM_SERVICE_H

#include "../driver/pwm/pwm.h"
#include "../driver/timer/timer.h"

#include <avr/io.h>
#include <stdint.h>

typedef enum{
    state_ramp_on,
    state_ramp_off,
    state_led_fade
} pwm_state_t;


void run_led_rampup(void);
void led_fade(void);
void pwm_set_state(pwm_state_t state);
void pwm_src(void);

#endif