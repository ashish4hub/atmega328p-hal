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


void adc_state_assign(ADC_work_state st);               // Start or stop adc according to command
void adc_src(void);                                    // Reading adc
GAS_reading_t gas_status_rt(void);
uint16_t gas_result(void);

#endif