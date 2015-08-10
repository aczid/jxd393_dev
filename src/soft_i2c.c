// PORTA1 = SDA (pin 2)
// PORTA2 = SCLK (pin 3)
#include "soft_i2c.h"
#include "delay.h"

void soft_i2c_init(void){
    // input with pull-up in input mode, push/pull in output mode
    GPIOA->CR1 = 0x06;
    // interrupts disabled in input mode, output at 2MHz in output mode
    GPIOA->CR2 = 0x00;

    i2c_sda_hi();
    i2c_sclk_hi();
    i2c_bit_delay();
}

void soft_i2c_start(void){
    i2c_sclk_hi();
    i2c_sda_lo();
    i2c_bit_delay();

    i2c_sclk_lo();
}

void soft_i2c_stop(void){
    i2c_sclk_hi();
    i2c_sda_hi();
    i2c_bit_delay();
}

void soft_i2c_writebit(uint8_t bit){
    if(bit){
        i2c_sda_hi();
    } else {
        i2c_sda_lo();
    }

    i2c_sclk_hi();
    i2c_bit_delay();

    i2c_sclk_lo();
}

uint8_t soft_i2c_readbit(void){
    uint8_t bit;
    i2c_sda_hi();
    i2c_sclk_hi();
    i2c_bit_delay();

    bit = GPIOA->IDR & 0x04;

    i2c_sclk_lo();
    i2c_bit_delay();

    return bit;
}

uint8_t soft_i2c_write(uint8_t byte){
    uint8_t mask = 0x80, i;
    for(i = 0; i < 8; i++){
        soft_i2c_writebit(byte & mask);
        mask >>= 1;
    }
    return soft_i2c_readbit(); // ack bit
}

uint8_t soft_i2c_read(uint8_t ack){
    uint8_t byte = 0x00, i;
    for(i = 0; i < 8; i++){
        byte |= soft_i2c_readbit();
        byte <<= 1;
    }
    if(ack){
        soft_i2c_writebit(0);
    } else {
        soft_i2c_writebit(1);
    }
    i2c_bit_delay();
    return byte;
}

void soft_i2c_restart(){
    i2c_sda_lo();
    i2c_sclk_hi();

    i2c_sclk_lo();
    i2c_bit_delay();

    i2c_sda_hi();
    i2c_bit_delay();

    i2c_sclk_hi();
    i2c_bit_delay();
}

uint8_t soft_i2c_transmit(uint8_t addr){
    soft_i2c_start();
    return soft_i2c_write(addr<<1 | 0); // set write bit
}

uint8_t soft_i2c_request(uint8_t addr){
    soft_i2c_start();
    return soft_i2c_write(addr<<1 | 1); // set read bit
}
