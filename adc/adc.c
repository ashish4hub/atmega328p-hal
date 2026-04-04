/* Driver for Analog To Digital Converter (ADC) */
/* MCU = ATmega328P (8 bit) */
/* Frequency = 16MHz */

/* Supported ADC Channels on ATmega328P */
/* PC0, PC1, PC2, PC3, PC4, PC5 */

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

static volatile uint16_t adc_result = 0;    // Storing ADC reading
static volatile uint8_t adc_done = 0;      // Flag

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

static ADC_MODE_t adc_mode;

/* ADC Initialization */
void adc_init(const ADC_config_t *config){

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
        sei();                //Enable global interrupt
    }
}

 /* ADC reading */
 void ADC_start(ADC_Channels_t channel){
    if(ADCSRA & (1 << ADSC)){
        return;
    }
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
uint8_t ADC_done(void){
    return adc_done;
}

/* ADC result API */
uint16_t ADC_get_result(void){
    return adc_result;
}

/* ADC interrupt function*/
ISR(ADC_vect){
    adc_result = ADC;
    adc_done = 1;
}

