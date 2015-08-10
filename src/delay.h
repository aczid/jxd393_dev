#ifndef _DELAY_H
#define _DELAY_H

#include "stm8s_conf.h"
#include <stdint.h>

void _delay_us(uint32_t d);
void _delay_ms(uint32_t d);

#define TICKS_PER_MS ((uint32_t) (F_CPU/1000.0))
#define TICKS_PER_US ((uint32_t) (TICKS_PER_MS/1000.0))

#endif // _DELAY_H
