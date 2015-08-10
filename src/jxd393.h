#ifndef _JXD393_H
#define _JXD393_H

#include "stm8s_conf.h"
#include <stdint.h>

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

#endif // _JXD393_H
