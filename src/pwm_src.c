/* PWM servicce */

#include "pwm_src.h"

pwm_state_t pwm_state = state_pwm_off;

/* Start led ram up */
void run_pwm(void){

    static uint32_t duty_wait = 0;
    static uint8_t duty = 0;

    if(duty_wait == 0){
        duty_wait = get_ticks();
    }
    
    if(nb_wait_ms(&duty_wait,50)){

        pwm_set(pwm_CH1A,duty);
        duty++;

        if(duty > 100){
            duty = 0;
        }
    }
}

/* State based control */
void pwm_set_state(pwm_state_t state){

    switch (state)
    {
    case state_pwm_on:
        pwm_state = state_pwm_on;
        break;
    case state_pwm_off:
    pwm_state = state_pwm_off;
        break;
    }
}

/* PWM SRC*/
void pwm_src(void){
    
    switch (pwm_state)
    {
    case state_pwm_on:
        pwm_start(pwm_CH1A);
        run_pwm();
        break;
    
    case state_pwm_off:
        pwm_disable(pwm_CH1A);
        break;
    }
}

