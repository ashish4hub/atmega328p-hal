#ifndef UART_H
#define UART_H

#include <stdint.h>

#define Tx_buffer_size 64
#define Rx_buffer_size 64
#define line_size 32

 void uart_init(uint32_t BAUD);
 void uart_print(const char *s);
 uint8_t read_line(void);

 extern const char line[line_size];

 #endif
