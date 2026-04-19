#ifndef UART_H
#define UART_H

#include <stdint.h>

#define Tx_buffer_size 64
#define Rx_buffer_size 64
#define line_size 32

 void USART_init(uint32_t BAUD);                 /* Initialize USART with desired baud rate */
 void USART_print(const char *str);             /* Print character or string */
 void USART_printIN(int num);    /* Print integers */
 uint8_t USART_read_line(void);               /* Build string from received characters Returns 1 when done */

 extern char line[line_size];      /* String storage array used for reading the received string */

 #endif