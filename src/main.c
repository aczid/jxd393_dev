#include "jxd393.h"
#include "nRF24.h"
#include "stm8s_conf.h"
#include "debug.h"
#include "delay.h"
#include "mpu6050.h"
#include "soft_i2c.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

int main() {
    /*uint32_t i;*/
    uint8_t j = 0;
    /*uint8_t acc_data[14];*/
	// Configure pins
    int8_t ret;
    ret = mpu6050_init();

    leds_enable();

    /*nRF24_init();*/
    /*nRF24_TXMode();*/
    /*if(ret != 0){*/
    /*sprintf(log.buffer, "Error initializing MPU6050!\n");*/
    /*}*/
    // Loop
    do {
        if(mpu6050_read(log.buffer) != 0){
            sprintf(log.buffer, "Error reading MPU6050!\n");
        }
        /*log.buffer[j++ % sizeof(log)] = 'A';*/
        /*if(nRF24_TXPacket("AAAA", 4) == nRF24_MASK_MAX_RT){*/
        /**//*if(nRF24_TXPacket("AAAA", 4) == nRF24_MASK_TX_DS){*/
        if(ret == 0){
            leds_toggle();
        }
        _delay_ms(1000);
        /*for(i = 0; i < 78000; i++){*/
        /*}*/
    } while(1);
}
