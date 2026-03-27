#ifndef UART_H
#define UART_H

#include <stdint.h>

#define Tx_buffer_size 64
#define Rx_buffer_size 64
#define line_size 32

 void uart_put_tx(char c);
 void uart_print(const char *s);
 void uart_init(uint32_t BAUD);
 int8_t uart_rx_avail(void);
 char uart_get_data(void);
 uint8_t read_line(void);

 extern volatile uint8_t Tx_buff[Tx_buffer_size];
 extern volatile uint8_t tx_tail;
 extern volatile uint8_t tx_head;
 extern volatile uint8_t rx_buff[Tx_buffer_size];
 extern volatile uint8_t rx_tail;
 extern volatile uint8_t rx_head;
 extern const char line[line_size];

 #endif
