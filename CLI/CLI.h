#ifndef CLI_H
#define CLI_H

#include "../driver/uart/uart.h"
#include "../src/led_src.h"
#include "../src/pwm_src.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void CLI_process(void);
void CLI_execute(char *cmd);

#endif