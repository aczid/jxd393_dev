#ifndef _DELAY_H
#define _DELAY_H

#include <stdint.h>

void delay_20_cycles(void);
void _delay_us(uint32_t d);
#define F_CPU 16000000
#define TICKS_PER_MS (F_CPU/1000)
#define TICKS_PER_US (TICKS_PER_MS/1000)

#endif // _DELAY_H
