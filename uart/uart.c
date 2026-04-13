/* UART driver */
/* Hardware abstraction layer for serial communicattion using USART */
/* MCU = ATmega328P */

/*--------------------APIs--------------------*/
/*
* USART_init() ----> Initialize USART with desired BAUD
* USART_print() ----> Print string
* USART_read_line() ----> Builds string from received characters and stores in 'line' array -> Returns 1 when ready for reading
* line ----> Character array where string is stored
*/


#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<stdint.h>

#define Tx_buffer_size 64  // TX Buffer size
#define RX_buffer_size 64   // RX Buffer size

#define line_size 32      // Line size for RX string

char line[line_size];     // Character array for forming string

uint8_t line_idx = 0;     // Line array index


volatile char RX_buffer[RX_buffer_size];
volatile uint8_t RX_head = 0;
volatile uint8_t RX_tail = 0;

volatile uint8_t Tx_buffer[Tx_buffer_size];
volatile uint8_t TX_tail = 0;
volatile uint8_t TX_head = 0;


/* USART initialization */
void USART_init(uint32_t BAUD){
  uint16_t BRC = ((F_CPU / (16UL * BAUD)) - 1UL);
  UBRR0H = (BRC >> 8);
  UBRR0L = BRC;
  UCSR0B = (1 << TXEN0) | (1 << RXCIE0) | (1 << RXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}


/*-------------------------------TX-------------------------------*/

/* 
* Store characters in TX buffer 
* Enable interrupt for transmission
* If interrupt is off kickstart transmission manually  
*/
void USART_TX_byte(uint8_t data){

    uint8_t next_head;

    while(1){
        next_head = (TX_head + 1) % Tx_buffer_size;
        if(next_head != TX_tail){
            break;
        }
    }

    Tx_buffer[TX_head] = data;
    TX_head = next_head;

    // If interrupt is OFF -> UART is idle -> kickstart manually
    if(!(UCSR0B & (1 << UDRIE0))){
        UDR0 = Tx_buffer[TX_tail];
        TX_tail = (TX_tail + 1) % Tx_buffer_size;
    }

    UCSR0B |= (1 << UDRIE0);
}

/* String print function */
void USART_print(const char *str){
  while(*str){
    USART_TX_byte(*str++);;
  }
}

/*
* Disable interrupt if buffer is empty and return
* If buffer has characters, starts transmission from TX buffer 
*/
ISR(USART_UDRE_vect){

  if(TX_head == TX_tail){
    UCSR0B &= ~(1 << UDRIE0);
    return;
  }
  UDR0 = Tx_buffer[TX_tail];
  TX_tail = (TX_tail + 1) % Tx_buffer_size;
}


/*---------------------------------------RX---------------------------------------*/

/* RX interrupt function */
ISR(USART_RX_vect){
  char data = UDR0; // Read received byte in data 
  uint8_t next = (RX_head + 1) % RX_buffer_size;
  
  //Drop byte is buffer is full
  if(next != RX_tail){
    RX_buffer[RX_head] = data;
    RX_head = next;
  }
}

/* If data is availabe for reading */
uint8_t USART_rx_avail(void){
  return (RX_head != RX_tail);
}

/* Get RX data */
char USART_get_data(void){
  if(RX_head == RX_tail) return 0;
  char data = RX_buffer[RX_tail];
  RX_tail = (RX_tail + 1) % RX_buffer_size;
  return data;
}

/* Building Line (string) */
uint8_t USART_read_line(void){
  while(USART_rx_avail()){
    char c = USART_get_data();
    
    if(c == '\r') continue;

    if(c == '\n'){
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