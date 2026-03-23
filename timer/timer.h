#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <stdint.h>

void timer_config(void);
uint8_t nb_wait_ms(uint32_t time);

extern volatile uint32_t tick;

 #endif
