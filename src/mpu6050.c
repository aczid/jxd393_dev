#include "mpu6050.h"
#include "soft_i2c.h"
#include <string.h>
#include <stdio.h>

int8_t mpu6050_init(void){
    uint8_t ret;
    soft_i2c_init();
    ret = soft_i2c_transmit(0x68);
    if(ret){
        return -1;
    }
    ret = soft_i2c_write(0x6B); // PWR_MGMT_1 register
    if(ret != 0){
        return -1;
    }
    ret = soft_i2c_write(0x0); // set to zero (wakes up the MPU6050)
    soft_i2c_stop();
    if(ret != 0){
        return -1;
    }
    return 0;
}

int8_t mpu6050_read(uint8_t *buf){
    uint8_t i;
    uint8_t ret = 0;
    ret = soft_i2c_transmit(0x68);
    if(ret != 0){
        return -1;
    }
    ret = soft_i2c_write(0x3B); // starting with register ACCEL_XOUT_H
    if(ret != 0){
        return -1;
    }
    soft_i2c_stop();

    ret = soft_i2c_request(0x68);
    if(ret != 0){
        return -1;
    }
    soft_i2c_start();
    for(i = 0; i < 14; i++){
        buf[i] = soft_i2c_read(1);
    }
    if(ret != 0){
        return -1;
    }
    soft_i2c_stop();
    return 0;
}
