/* TIMER1 PWM driver */
/* MCU = ATmega328P */

#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<stdint.h>

/* Channels */
typedef enum {
    pwm_CH1A,  /* PB1 */
    pwm_CH1B   /* PB2 */
} pwm_channel_t;


/* Prescaler table */
typedef struct {
    uint16_t prescaler_val;
    uint8_t cs_bits;
} prescaler_t;

const prescaler_t prescalers[] = {
    {1, (1 << CS10)},
    {8, (1 << CS11)},
    {64, (1 << CS11) | (1 << CS10)},
    {256, (1 << CS12)},
    {1024, (1 << CS12) | (1 << CS10)}
};

/* Computation fucntion for Prescaler */
uint8_t pwm_compute(uint32_t freq, uint16_t *top, uint8_t *cs_bits){

    /* Selection of minimum prescaler */
    for(uint8_t i = 0; i < 5; i++){
        uint32_t temp = (F_CPU / (prescalers[i].prescaler_val * freq)) - 1;

        if(temp <= 65535){         
            *top = (uint16_t)temp;      // Type cast from 32bit to 16bit to prevent compiler warning
            *cs_bits = prescalers[i].cs_bits;        // Assigning prescaler bits 
            return 1;    // Prescaler found and frequency achieved
        }
    }
    return 0;    // Frequency not achievable
}

static uint16_t pwm_top;   /*  ICR1 */

void pwm_init(uint32_t freq){    /* PWM Initialisation and TOP (ICR1) calculation based on passed frequency */
    if(freq == 0){               /* Frequency edge case */
        return;
    }
    uint8_t cs_bits;
    if(!pwm_compute(freq, &pwm_top, &cs_bits)){
        return;;
    }
    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM12) | (1 << WGM13) | cs_bits;
    ICR1 = pwm_top;
}

/*PWM channel (pin) selection */
void pwm_enb_channel(pwm_channel_t channel){
    if(channel == pwm_CH1A){
        DDRB |= (1 << PB1);
        TCCR1A |= (1 << COM1A1);
    }
    else if (channel == pwm_CH1B)
    {
        DDRB |= (1 << PB2);
        TCCR1A |= (1 << COM1B1);
    }
}
void pwm_set(pwm_channel_t ch, uint32_t duty_percent){   /*PWM channel and duty percent to be passed*/
    
    if(duty_percent > 100) duty_percent = 100; 
    
    uint16_t duty = (pwm_top * duty_percent) / 100;    /*Duty percentage to OCR1x value conversion*/
    
    if(ch == pwm_CH1A){
        OCR1A = duty;
    }
    else if (ch == pwm_CH1B){
        OCR1B =  duty;
    }
}
