#ifndef SPI_H
#define SPI_H

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

void SPI_init(void);
uint8_t SPI_transfer(uint8_t data);

#endif