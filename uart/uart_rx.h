#ifndef UART_RX_H
#define UART_RX_H

#include <avr/io.h>
#include <stdint.h>

#define Rx_buffer_size 64
#define line_size 32
 uint8_t uart_rx_avail(void);
 char uart_get_data(void);
 uint8_t read_line(void);
 void rx_ini(void);

 extern volatile uint8_t rx_buff[Tx_buffer_size];
 extern volatile uint8_t rx_tail;
 extern volatile uint8_t rx_head;
 extern const char line[line_size];

 #endif
