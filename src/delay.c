#include "delay.h"

// call: 6 cycles
void delay_20_cycles(void){
    // nops: 1 cycle each
    __asm__("nop\n"
            "nop\n"
            "nop\n");
    // ret: 11 cycles
}

void _delay_us(uint32_t d){
    int i = 0;
    // fudged number: adjust for loop accounting
    for(i = 0; i < d/TICKS_PER_US/20-1; i++){
        delay_20_cycles();
    }
}


