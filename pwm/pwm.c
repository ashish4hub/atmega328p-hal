/* TIMER1 PWM driver */
/* MCU = ATmega328P */
#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<stdint.h>
typedef enum {
    pwm_CH1A,  /* PB1 */
    pwm_CH1B   /* PB2 */
} pwm_channel_t; 
void pwm_init(pwm_channel_t chnl, uint32_t freq){
    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);
    ICR1 = (F_CPU / (8UL * freq)) - 1;
    if(chnl == pwm_CH1A){
        DDRB |= (1 << PB1);
        TCCR1A |= (1 << COM1A1);
    }
    else if (chnl == pwm_CH1B){
        DDRB |= (1 << PB2);
        TCCR1A |= (1 << COM1B1);
    }
}
void pwm_set(pwm_channel_t ch, uint32_t duty){
    if(duty > ICR1){
        return;
    }
    if(ch == pwm_CH1A){
        OCR1A = duty;
    }
    else if(ch == pwm_CH1B){
        OCR1B = duty;
    }
}
