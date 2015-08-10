#ifndef _MPU6050_H
#define _MPU6050_H

#include <stdint.h>

int8_t mpu6050_init(void);
int8_t mpu6050_read(uint8_t *);

#endif // _MPU6050_H
