/* ADC service for gas concentration detection */

#define threshold_high 520
#define threshold_low 470

#include "adc_src.h"

static ADC_work_state state = GAS_stop_detect;
static uint16_t result;
static GAS_reading_t status;

/* Set state */
void adc_set_state(ADC_work_state st){

    switch (st)
    {
    case GAS_start_detect:
        state = GAS_start_detect;
        break;
    
    case GAS_stop_detect:
        state = GAS_stop_detect;
        break;
    }
}

/*ADC reading function */
void adc_reading(void){

    // Start conversion only when previous conversion completed and read successfully 
    static uint8_t cnv_started = 0;
    if(!cnv_started){
        ADC_start(ADC_CH0);
        cnv_started = 1;
    }
    else{
        if(ADC_done()){
            result = ADC_get_result();
            cnv_started = 0;
        }
    }
    if(result >= threshold_high){
        status = GAS_high;
    }
    else if(result <= threshold_low){
        status = GAS_low;
    }
}

/* Return result function */
uint16_t gas_result(void){
    return result;
}

/* Return status function */
GAS_reading_t gas_status(void){
    return status;
}

/* ADC src */
void adc_src(void){

    switch (state)
    {
    case GAS_start_detect:
        ADC_enable();
        adc_reading();
        break;
    
    case GAS_stop_detect:
        ADC_disable();
        break;
    }
}