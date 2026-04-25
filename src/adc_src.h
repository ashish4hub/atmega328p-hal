#ifndef ADC_SRC_H
#define ADC_SRC_H

#include "../driver/adc/adc.h"
#include <stdint.h>

/* Work state */
typedef enum{
    GAS_start_detect,
    GAS_stop_detect
}ADC_work_state;

/* GAS reading status */
typedef enum{
    GAS_idle,
    GAS_high,
    GAS_low
}GAS_reading_t;


void adc_set_state(ADC_work_state st);                  // set state to start or stop adc according to command
void adc_src(void);                                    // Reading adc
GAS_reading_t gas_status(void);                       // Return gas status (High or Low)
uint16_t gas_result(void);                           // Return conversion result
uint8_t gas_reading_done(void);                     // Return 1 when conversion is done 

#endif