#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>


/* ADC references*/
typedef enum {
ADC_REF_AREF = 0x00,
ADC_REF_VCC = 0x01,
ADC_REF_INTERNAL = 0x03
}ADC_REF_t;

/* ADC mode selection */
typedef enum{
ADC_POLLING,
ADC_INTERRUPT
}ADC_MODE_t;

/* ADC prescaler */
typedef enum{
ADC_PS_2 = 0x01,
ADC_PS_4 = 0x02,
ADC_PS_8 = 0x03,
ADC_PS_16 = 0x04,
ADC_PS_32 = 0x05,
ADC_PS_64 = 0x06,
ADC_PS_128 = 0x07
}ADC_prescaler_t;


/* ADC Channels */
typedef enum {
    ADC_CH0 = 0x00,
    ADC_CH1 = 0x01,
    ADC_CH2 = 0x02,
    ADC_CH3 = 0x03,
    ADC_CH4 = 0x04,
    ADC_CH5 = 0x05
} ADC_Channels_t;

/* Configuration */
 typedef struct{
    ADC_REF_t reference;
    ADC_prescaler_t prescaler;
    ADC_MODE_t mode;
}ADC_config_t;


void ADC_init(const ADC_config_t *config);      /* Initialize ADC with desired Reference, Prescaler and Mode */
void ADC_start(ADC_Channels_t channel);        /* Start ADC conversion on selected Channel (pin) */
uint8_t ADC_done(void);                       /* Function to check if conversion is done */ /* Returns 1 if conversion is done */
uint16_t ADC_get_result(void);               /* Returns ADC result */

#endif