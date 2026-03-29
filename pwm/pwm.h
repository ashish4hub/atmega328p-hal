#ifndef pwm_H
#define pwm_H

#include <stdint.h>
typedef enum {
    pwm_CH1A,  /* PB1 */
    pwm_CH1B   /* PB2 */
}pwm_channel_t;


void pwm_init(uint32_t freq);  /* Initialize pwm with desired frequency and PIN */
void pwm_set(pwm_channel_t ch, uint32_t duty_percent);     /* Set pin (PB1 or PB2), set duty cycle (0 --> 1000)*/

#endif
