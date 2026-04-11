/* UART driver */
/* Hardware abstraction layer for serial communicattion using USART */
/* MCU = ATmega328P */

#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<stdint.h>

#define Tx_buffer_size 64  // TX Buffer size
#define rx_buff_size 64   // RX Buffer size

#define line_size 32      // Line size for RX string

char line[line_size];     // Character array for forming string

uint8_t line_idx = 0;     // Line array index


volatile char rx_buff[rx_buff_size];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

volatile uint8_t Tx_buff[Tx_buffer_size];
volatile uint8_t tx_tail = 0;
volatile uint8_t tx_head = 0;

/* Store characters in TX buffer and enables TX interrupt */
void USART_put_tx(char c){

  uint8_t next = (tx_head +1 ) % Tx_buffer_size;
  
//drops character if buffer is full
  if(next == tx_tail){
    return;
  }
  Tx_buff[tx_head] = c;
  tx_head = next;
  UCSR0B |= (1 << UDRIE0);
}

/* UART print API */
void USART_print(const char *s){
  while(*s){
    USART_put_tx(*s++);
  }
}

/* USART initialization */
void USART_init(uint32_t BAUD){
  uint16_t BRC = ((F_CPU / (16UL * BAUD)) - 1UL);
  UBRR0H = (BRC >> 8);
  UBRR0L = BRC;
  UCSR0B = (1 << TXEN0) | (1 << RXCIE0) | (1 << RXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/* TX (UDRE) interrupt function */ 
ISR(USART_UDRE_vect)
{
    if (tx_head == tx_tail) {
        UCSR0B &= ~(1 << UDRIE0);   // Buffer empty
    } else {
        UDR0 = Tx_buff[tx_tail];
        tx_tail = (tx_tail + 1) % Tx_buffer_size;
    }
}

/* RX interrupt function */
ISR(USART_RX_vect){
  char data = UDR0; // Read received byte in data 
  uint8_t next = (rx_head + 1) % rx_buff_size;
  
  //Drop byte is buffer is full
  if(next != rx_tail){
    rx_buff[rx_head] = data;
    rx_head = next;
  }
}

/* If data is availabe for reading */
uint8_t USART_rx_avail(void){
  return (rx_head != rx_tail);
}

/* Get RX data */
char USART_get_data(void){
  if(rx_head == rx_tail) return 0;
  char data = rx_buff[rx_tail];
  rx_tail = (rx_tail + 1) % rx_buff_size;
  return data;
}

/* Building Line (string) */
uint8_t read_line(void){
  while(USART_rx_avail()){
    char c = USART_get_data();
    if(c == '\n' || c == '\r'){
      line[line_idx] = '\0'; //end string
      line_idx = 0;
      return 1;
    }
    if(line_idx < line_size - 1){
      line[line_idx++] = c;
    }
  }
  return 0;
}