/* ADC service */

#include "adc_src.h"

static uint16_t threshold_high = 520;      // Threshold for high
static uint16_t threshold_low = 460;      // Threshold for low
static uint16_t adc_result = 0;          // ADC result

GAS_reading_t status = GAS_idle;            // Status (High or Low) according to threshold
ADC_work_state state;                      // Working status

/* State assignment */
void adc_state_assign(ADC_work_state st){
    if(st == GAS_start_detect){
        state = GAS_start_detect;
    }
    else {
        state = GAS_stop_detect;
    }
}

/* Start and read ADC value */
void adc_reading(void){

    ADC_start(ADC_CH0);

    if(ADC_done()){

        adc_result = ADC_get_result();
    }
    if(adc_result > threshold_high){
        status = GAS_high;
    }
    else if(adc_result < threshold_low){
        status = GAS_low;
    }
}

/* ADC SRC */
void adc_src(void){

    switch (state)
    {
    case GAS_start_detect:
        adc_reading();
        break;
    
    case GAS_stop_detect:
        ADC_disable();
        break;
    }
}

/* GAS status function */
GAS_reading_t gas_status_rt(void){
    return status;
}

/* Return result function */
uint16_t gas_result(void){
    return adc_result;
}