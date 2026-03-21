//TX code ring buffer
// Upload on Transmitter IC

#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<stdint.h>
#define BAUD 9600
#define BRC ((F_CPU / (16UL * BAUD)) - 1UL)
#define Tx_buffer_size 64 
volatile uint8_t Tx_buff[Tx_buffer_size];
volatile uint8_t tx_tail = 0;
volatile uint8_t tx_head = 0;
volatile uint32_t tick = 0;

void uart_put_tx(char c){
  uint8_t next = (tx_head +1 ) % Tx_buffer_size;
  
//drops character if buffer is full
  if(next == tx_tail){
    return;
  }
  Tx_buff[tx_head] = c;
  tx_head = next;
  UCSR0B |= (1 << UDRIE0);
}
void uart_print(const char *s){
  while(*s){
    uart_put_tx(*s++);
  }
}
void time_setup(void){
  TCCR0A |= (1 << WGM01);
  TCCR0B |= (1 << CS00) | (1 << CS01);
  OCR0A = 249; // 16MHz / 64 = 250kHz  --> 250 timer counts = 1ms tick
  TIMSK0 |= (1 << OCIE0A);
}
void uart_init(void){
  time_setup();
  UBRR0H = (BRC >> 8);
  UBRR0L = BRC;
  UCSR0B = (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
//TX (UDRE) interrupt function 
ISR(USART_UDRE_vect)
{
    if (tx_head == tx_tail) {
        UCSR0B &= ~(1 << UDRIE0);   // Buffer empty
    } else {
        UDR0 = Tx_buff[tx_tail];
        tx_tail = (tx_tail + 1) % Tx_buffer_size;
    }
}
//Timer interrupt function
ISR(TIMER0_COMPA_vect){
  tick++;
}