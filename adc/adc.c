/* Driver for Analog To Digital Converter (ADC) */
/* MCU = ATmega328P (8 bit) */
/* Frequency = 16MHz */

/* Supported ADC Channels on ATmega328P */
/* PC0, PC1, PC2, PC3, PC4, PC5 */

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

/* ADC Channels */
typedef enum {
    ADC_CH0,
    ADC_CH1,
    ADC_CH2,
    ADC_CH3,
    ADC_CH4,
    ADC_CH5
} ADC_Channels_t;

void adc_init(void){
    ADMUX |= (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

}

/* Channel selection and reading */
uint16_t adc_read(ADC_Channels_t channel){

    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);     // Channel config
    ADCSRA |= (1 << ADSC);                        // start conversion
    while(ADCSRA & (1 << ADSC));                 // wait until conversion is complete (polling)
    ADCSRA |= (1 << ADIF);                      // Clear flag
    return ADC;                                // return ADC converted value
}