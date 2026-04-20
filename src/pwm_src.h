#ifndef PWM_SERVICE_H
#define PWM_SERVICE_H

#include "../pwm/pwm.h"
#include "../timer/timer.h"

#include <avr/io.h>
#include <stdint.h>

void run_pwm(void);
void stop_pwm(void);
void start_pwm(void);


#endif