/* Driver for Analog To Digital Converter (ADC) */
/* MCU = ATmega328P (8 bit) */
/* Frequency = 16MHz */

/* Supported ADC Channels on ATmega328P */
/* PC0, PC1, PC2, PC3, PC4, PC5 */

/*
* ADC_init() ----> Initializes ADC with selected prescaler, Mode and reference
* ADC_start() ----> Starts converison on selected ADC channel according to selected mode (POLLING/INTERRUPT)
* ADC_done() ----> Returns ADC conversion status (if conversion is completed or not)
* ADC_get_result() ----> Returns ADC conversion results 
*/

#define F_CPU 16000000UL
#include "adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

static volatile uint16_t adc_result = 0;    // Storing ADC reading
static volatile uint8_t adc_done = 0;      // Flag
static ADC_MODE_t adc_mode;               // Selected Mode

/* ADC Initialization */
void ADC_init(const ADC_config_t *config){

    adc_mode = config->mode;               // Store Mode

    /*-------------------ADMUX-------------------*/
    ADMUX = 0;
    ADMUX |= (config->reference << REFS0);

    /*-------------------ADCSRA-------------------*/
    ADCSRA = 0;
    ADCSRA |= (1 << ADEN);   // Enable ADC
    ADCSRA |= (config->prescaler);      // Set prescaler bits

    if(config->mode == ADC_INTERRUPT){
        ADCSRA |= (1 << ADIE);      // ADC interrupt enable
    }
}

 /* ADC reading */
 void ADC_start(ADC_Channels_t channel){

    if(channel > ADC_CH5) return;

    if(ADCSRA & (1 << ADSC)){
        return;
    }
    DIDR0 |= (1 << channel);
    adc_done = 0;
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);

    if(adc_mode == ADC_POLLING){
        while(ADCSRA & (1 << ADSC));
        adc_result = ADC;
        adc_done = 1;    
    }
}

/* ADC status API */
/* Retturns 1 if conversion is done */
uint8_t ADC_done(void){
    return adc_done;
}

/* ADC result API */
/* Returns ADC reulst */
uint16_t ADC_get_result(void){
    return adc_result;
}

/* ADC interrupt function*/
ISR(ADC_vect){
    adc_result = ADC;
    adc_done = 1;
}

