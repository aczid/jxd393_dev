#include "jxd393.h"

void leds_enable(){
	PD_DDR = 0x80;
	PD_CR1 = 0x80;
}

void leds_on(){
    PD_ODR |= 0x80;
}

void leds_off(){
    PD_ODR &= ~0x80;
}

void leds_toggle(){
    PD_ODR ^= 0x80;
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
