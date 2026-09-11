#ifndef ADC_SRC_H
#define ADC_SRC_H

#include "../driver/adc/adc.h"
#include <stdint.h>

/* Work state */
typedef enum{
    MOIST_start_detect,
    MOIST_stop_detect
}MOIST_work_state;

/* GAS reading status */
typedef enum{
    MOIST_idle,
    MOIST_high,
    MOIST_low
}MOIST_reading_t;


void MOIST_set_state(MOIST_work_state st);                  // set state to start or stop adc according to command
void MOISTURE_src(void);                                    // Reading adc
MOIST_reading_t MOIST_status(void);                       // Return gas status (High or Low)
uint16_t MOIST_result(void);                           // Return conversion result

#endif