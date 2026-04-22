#ifndef PWM_SERVICE_H
#define PWM_SERVICE_H

#include "../driver/pwm/pwm.h"
#include "../driver/timer/timer.h"

#include <avr/io.h>
#include <stdint.h>

typedef enum{
    state_pwm_on,
    state_pwm_off
}pwm_state_t;


void run_pwm(void);
void pwm_set_state(pwm_state_t state);
void pwm_src(void);

#endif