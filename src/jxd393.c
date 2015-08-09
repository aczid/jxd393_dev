#include "jxd393.h"

void leds_enable(){
    GPIOD->DDR = 0x80;
    GPIOD->CR1 = 0x80;
    GPIOD->CR2 = 0x80;
}

void leds_on(){
    GPIOD->ODR = GPIOD->IDR | 0x80;
}

void leds_off(){
    GPIOD->ODR = GPIOD->IDR & ~0x80;
}

void leds_toggle(){
    GPIOD->ODR ^= 0x80;
}

void motor1_set_speed(uint8_t speed){
    uint8_t the_speed = speed;
}

/*void motor2_set_speed(uint8_t speed){*/
/*}*/

/*void motor3_set_speed(uint8_t speed){*/
/*}*/

/*void motor4_set_speed(uint8_t speed){*/
/*}*/
