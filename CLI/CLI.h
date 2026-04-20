#ifndef CLI_H
#define CLI_H

#include <stdint.h>

void CLI_process(void);
void CLI_execute(char *cmd);

extern uint8_t pwm_enable;
extern uint8_t gas_enable;

#endif