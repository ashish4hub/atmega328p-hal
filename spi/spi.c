/* SPI driver for ATmega328P Microcontroller */
/* Hardware abstraction layer for SPI */
/* MCU = ATmega328P */


/* PINS 
* PB3 ----> MOSI (OUTPUT) ----> Master Output Slave In
* PB4 ----> MISO (INPUT) ----> Master In Slave Output
* PB2 ----> SS (OUTPUT) ----> Slave Select (MASTER)
* PB5 ----> SCK (OUTPUT) ----> Clock
*/

#include "spi.h"


/* SPI initialization */
void SPI_init(void){
    
    DDRB |= (1 << PB3) | (1<< PB2) | (1 << PB5);      // MOSI, SS, SCK as output
    DDRB &= ~ (1 << PB4);                             // MISO as input

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);          // SPI eanble as Master with F_CPU/64 clock
    SPCR &= ~((1 << CPOL) | (1 << CPHA));     // leading edge --> Risisng, sampling edge
}

/* SPI Transfer function */
uint8_t SPI_transfer(uint8_t data){
    SPDR = data;

    while(!(SPSR & (1 << SPIF)));    // wait

    return SPDR;     // received data
}

