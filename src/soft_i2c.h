#ifndef _SOFT_I2C_H
#define _SOFT_I2C_H

// PORTA1 = SDA (pin 2)
// PORTA2 = SCLK (pin 3)
#include "stm8s_conf.h"
#include <stdint.h>

// set to output and set to low
#define i2c_sda_lo() (GPIOA->DDR |= 0x04); (GPIOA->ODR = GPIOA->IDR & ~0x04)
#define i2c_sclk_lo() (GPIOA->DDR |= 0x02); (GPIOA->ODR = GPIOA->IDR & ~0x02)

// set to input (pull up enabled)
#define i2c_sda_hi() (GPIOA->ODR = GPIOA->DDR & ~0x04)
#define i2c_sclk_hi() (GPIOA->ODR = GPIOA->DDR & ~0x02)

#define i2c_bit_delay() _delay_us(160)

void soft_i2c_init(void);
void soft_i2c_start(void);
void soft_i2c_stop(void);
void soft_i2c_writebit(uint8_t bit);
uint8_t soft_i2c_readbit(void);
uint8_t soft_i2c_write(uint8_t byte);
uint8_t soft_i2c_read(uint8_t ack);
void soft_i2c_restart();
uint8_t soft_i2c_request(uint8_t addr);
uint8_t soft_i2c_transmit(uint8_t addr);

#endif // _SOFT_I2C_H

