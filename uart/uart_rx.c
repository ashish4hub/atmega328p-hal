// RX ring buffer code 
// Upload on RX IC

#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<string.h>
#define BAUD 9600
#define BRC ((F_CPU / (16UL * BAUD)) - 1UL)
#define rx_buff_size 64
volatile char rx_buff[rx_buff_size];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

//Configure UART RX
void rx_ini(void){
  UCSR0B = (1 << RXCIE0) | (1 << RXEN0);
  UBRR0H = (BRC >> 8);
  UBRR0L = BRC;
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
//Interrupt function only read the byte received and store in 'data'
ISR(USART_RX_vect){
  char data = UDR0; // Read received byte in data 
  uint8_t next = (rx_head + 1) % rx_buff_size;
  
  //Drop byte is buffer is full
  if(next != rx_tail){
    rx_buff[rx_head] = data;
    rx_head = next;
  }
}
uint8_t uart_rx_avail(void){
  return (rx_head != rx_tail);
}
char uart_get_data(void){
  if(rx_head == rx_tail) return 0;
  char data = rx_buff[rx_tail];
  rx_tail = (rx_tail + 1) % rx_buff_size;
  return data;
}
//Building line 
#define line_size 32
char line[line_size];
uint8_t line_idx = 0;
uint8_t read_line(void){
  while(uart_rx_avail()){
    char c = uart_get_data();
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
