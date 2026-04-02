#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* ADC Channels */
typedef enum {
    ADC_CH1,
    ADC_CH2,
    ADC_CH3,
    ADC_CH4,
    ADC_CH5,
    ADC_CH6
} ADC_Channels_t;

void adc_init(void);
uint16_t adc_read(ADC_Channels_t channel);

#endif