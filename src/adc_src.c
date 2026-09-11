/* ADC service for gas concentration detection */

#define threshold_high 520
#define threshold_low 470

#include "adc_src.h"

static MOIST_work_state state = MOIST_stop_detect;
static uint16_t result;
static MOIST_reading_t status;

/* Set state */
void MOIST_set_state(MOIST_work_state st){

    switch (st)
    {
    case MOIST_start_detect:
        state = MOIST_start_detect;
        break;
    
    case MOIST_stop_detect:
        state = MOIST_stop_detect;
        break;
    }
}

/* Moisture reading function */
void MOIST_reading(void){

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
        status = MOIST_high;
    }
    else if(result <= threshold_low){
        status = MOIST_low;
    }
}

/* Return result function */
uint16_t MOIST_result(void){
    return result;
}

/* Return status function */
MOIST_reading_t MOIST_status(void){
    return status;
}

/* ADC src */
void MOISTURE_src(void){

    switch (state)
    {
    case MOIST_start_detect:
        ADC_enable();
        adc_reading();
        break;
    
    case MOIST_stop_detect:
        ADC_disable();
        break;
    }
}