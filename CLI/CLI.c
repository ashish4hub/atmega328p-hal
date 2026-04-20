/* Command Line Interface */

#include "CLI.h"
#include "../uart/uart.h"
#include "../src/led_src.h"
#include "../src/pwm_src.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CLI_buffer_size 32

uint8_t pwm_enable = 0;

static char CLI_buffer[CLI_buffer_size];
static uint8_t CLI_index = 0;

/* Build string from received character and pass it to execute function */
void CLI_process(void){
    while(USART_rx_avail()){
        char c = USART_get_data();

        if(c == '\r') continue;

        if(c ==  '\n'){
            CLI_buffer[CLI_index] = '\0';         // Terminate string
            CLI_execute(CLI_buffer);             // Passing string to execute function
            CLI_index = 0;   // Buffer reset
        }
        else{
            if(CLI_index < CLI_buffer_size - 1){
                CLI_buffer[CLI_index++] = c;
            }
        }
    }
}

/* Execute function for triggering actions based on user commands */
void CLI_execute(char *cmd){

    if(strcmp(cmd, "led on") == 0){
        led_on();
        USART_print("LED ON\n");
    }
    else if(strcmp(cmd, "led off") == 0){
        led_off();
        USART_print("LED OFF\n");
    }
    else if(strcmp(cmd, "run pwm") == 0){
        pwm_enable = 1;
        USART_print("PWM STARTED\n");
    }
    else if(strcmp(cmd, "stop pwm") == 0){
        pwm_enable = 0;
        USART_print("PWM STOPPED\n");
    }
}
