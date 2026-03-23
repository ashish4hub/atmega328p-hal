#ifndef UART_TX_H
#define UART_TX_H

#include <avr/io.h>
#include <stdint.h>

#define Tx_buffer_size 64
 void uart_put_tx(char c);
 void uart_print(const char *s);
 void uart_init(void);

 extern volatile uint8_t Tx_buff[Tx_buffer_size];
 extern volatile uint8_t tx_tail;
 extern volatile uint8_t tx_head;
 extern volatile uint32_t tick;

 #endif
