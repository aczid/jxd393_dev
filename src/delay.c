#include "delay.h"

// simple but probably bad implementation

void _delay_us(uint32_t d){
    uint32_t i = 0;
    for(i = 0; i < (d/TICKS_PER_US); i++){
    }
}

void _delay_ms(uint32_t d){
    _delay_us(1000*d);
}


