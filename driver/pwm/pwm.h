#ifndef pwm_H
#define pwm_H


#include<avr/io.h>
#include<avr/interrupt.h>
#include<stdint.h>

/* Channels */
typedef enum {
    pwm_CH1A,     /* PB1 */
    pwm_CH1B,    /* PB2 */
    pwm_CH2B    /* PD3 */
} pwm_channel_t;

/*TIMER selection for PWM initialization */
typedef enum {
    pwm_TIMER1,
    pwm_TIMER2
} pwm_timer_t;


void pwm_init(uint32_t freq, pwm_timer_t timer);            /* Initialize pwm with desired frequency and PIN */
void pwm_set(pwm_channel_t ch, uint32_t duty_percent);     /* Set pin (PB1 or PB2), set duty cycle (0 --> 1000)*/
void pwm_disable(pwm_channel_t ch);                       /* Disable PWM and sets pin low */
void pwm_start(pwm_channel_t ch);                        /* Start PWM */

#endif
