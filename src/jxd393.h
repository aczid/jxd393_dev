#include "stm8s005k6.h"
#include <stdint.h>

#ifndef JXD393_H
#define JXD393_H
void leds_enable();
void leds_on();
void leds_off();
void leds_toggle();

void motor1_set_speed(uint8_t speed);
/*void motor2_set_speed(uint8_t);*/
/*void motor3_set_speed(uint8_t);*/
/*void motor4_set_speed(uint8_t);*/

/*void gyro_readout();*/
/*void radio_readout();*/

#endif
