#ifndef _DELAY_H
#define _DELAY_H

#include "stm8s_conf.h"
#include <stdint.h>

void _delay_us(uint32_t d);
void _delay_ms(uint32_t d);
#define TICKS_PER_MS (F_CPU/1000)
#define TICKS_PER_US (TICKS_PER_MS/1000)

#endif // _DELAY_H
