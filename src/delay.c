#include "delay.h"

// call: 6 cycles
void _delay_us(uint32_t d){
    uint32_t i = 0;
    // fudged number: adjust for loop accounting
    for(i = 0; i < (d/TICKS_PER_US); i++){
    }
    // ret: 11 cycles
}

void _delay_ms(uint32_t d){
    _delay_us(1000*d);
}


