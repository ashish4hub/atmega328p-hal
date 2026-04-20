/* PWM servicce */

#include "pwm_src.h"
#include "../CLI/CLI.h"


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

/* Stop pwm */
void stop_pwm(void){
    pwm_disable(pwm_CH1A);
}

/* Start pwm */
void start_pwm(void){
    pwm_start(pwm_CH1A);
}

