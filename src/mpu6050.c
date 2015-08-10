#include "mpu6050.h"
#include "soft_i2c.h"
#include "delay.h"
#include <string.h>
#include <stdio.h>

static uint8_t mpu6050_addr;

bool mpu6050_read(uint8_t *buf){
    uint8_t i;
    bool ret = false;
    ret |= soft_i2c_transmit(0x68);
    ret |= soft_i2c_write(0x3B); // starting with register ACCEL_XOUT_H
    soft_i2c_stop();

    ret |= soft_i2c_request(0x68);
    soft_i2c_start();
    for(i = 0; i < 14; i++){
        buf[i] = soft_i2c_read(1);
    }
    soft_i2c_stop();
    return 0;
}

/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
bool mpu6050_set_full_scale_gyro_range(uint8_t range) {
    bool ret = 0;
    ret |= soft_i2c_transmit(mpu6050_addr);
    ret |= soft_i2c_write(MPU6050_RA_GYRO_CONFIG);
    ret |= soft_i2c_write(MPU6050_GCONFIG_FS_SEL_BIT);
    ret |= soft_i2c_write(MPU6050_GCONFIG_FS_SEL_LENGTH);
    ret |= soft_i2c_write(range);
    soft_i2c_stop();
    return ret;
}


/** Set clock source setting.
 * An internal 8MHz oscillator, gyroscope based clock, or external sources can
 * be selected as the MPU-60X0 clock source. When the internal 8 MHz oscillator
 * or an external source is chosen as the clock source, the MPU-60X0 can operate
 * in low power modes with the gyroscopes disabled.
 *
 * Upon power up, the MPU-60X0 clock source defaults to the internal oscillator.
 * However, it is highly recommended that the device be configured to use one of
 * the gyroscopes (or an external clock source) as the clock reference for
 * improved stability. The clock source can be selected according to the following table:
 *
 * <pre>
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
 * </pre>
 *
 * @param source New clock source setting
 * @see getClockSource()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_CLKSEL_BIT
 * @see MPU6050_PWR1_CLKSEL_LENGTH
 */
bool mpu6050_set_clock_source(uint8_t source) {
    bool ret = 0;
    ret |= soft_i2c_transmit(mpu6050_addr);
    ret |= soft_i2c_write(MPU6050_RA_PWR_MGMT_1);
    ret |= soft_i2c_write(source);
    soft_i2c_stop();
    return ret;
}

/** Set full-scale accelerometer range.
 * @param range New full-scale accelerometer range setting
 * @see getFullScaleAccelRange()
 */
bool mpu6050_set_full_scale_accel_range(uint8_t range) {
    bool ret = 0;
    ret |= soft_i2c_transmit(mpu6050_addr);
    ret |= soft_i2c_write(MPU6050_RA_ACCEL_CONFIG);
    ret |= soft_i2c_write(MPU6050_ACONFIG_AFS_SEL_BIT);
    ret |= soft_i2c_write(MPU6050_ACONFIG_AFS_SEL_LENGTH);
    ret |= soft_i2c_write(range);
    soft_i2c_stop();
    return ret;
}

/** Set sleep mode status.
 * @param enabled New sleep mode enabled status
 * @see getSleepEnabled()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_SLEEP_BIT
 */
bool mpu6050_set_sleep_enabled(bool enabled) {
    bool ret = 0;
    ret |= soft_i2c_transmit(mpu6050_addr);
    ret |= soft_i2c_write(MPU6050_RA_PWR_MGMT_1);
    ret |= soft_i2c_write(MPU6050_PWR1_SLEEP_BIT);
    ret |= soft_i2c_write(enabled);
    soft_i2c_stop();
    return ret;
}

// PWR_MGMT_1 register

/** Trigger a full device reset.
 * A small delay of ~50ms may be desirable after triggering a reset.
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_DEVICE_RESET_BIT
 */
bool mpu6050_reset() {
    bool ret = 0;
    ret |= soft_i2c_transmit(mpu6050_addr);
    ret |= soft_i2c_write(MPU6050_RA_PWR_MGMT_1);
    ret |= soft_i2c_write(0x80);
    soft_i2c_stop();
    return ret;
}

bool mpu6050_init(void){
    bool ret = 0;
    soft_i2c_init();
    mpu6050_addr = MPU6050_DEFAULT_ADDRESS;
    ret |= mpu6050_set_clock_source(MPU6050_CLOCK_PLL_XGYRO);
    ret |= mpu6050_set_full_scale_gyro_range(MPU6050_GYRO_FS_250);
    ret |= mpu6050_set_full_scale_accel_range(MPU6050_ACCEL_FS_2);
    ret |= mpu6050_set_sleep_enabled(0);
    ret |= mpu6050_reset();
    return ret;
}

// WHO_AM_I register

/** Get Device ID.
 * This register is used to verify the identity of the device (0b110100, 0x34).
 * @return Device ID (6 bits only! should be 0x34)
 * @see MPU6050_RA_WHO_AM_I
 * @see MPU6050_WHO_AM_I_BIT
 * @see MPU6050_WHO_AM_I_LENGTH
 */
uint8_t mpu6050_get_device_id() {
    bool ret = 0, dev_id = 0;
    ret |= soft_i2c_transmit(mpu6050_addr);
    ret |= soft_i2c_write(MPU6050_RA_WHO_AM_I);
    dev_id = soft_i2c_read(1);
    soft_i2c_stop();
    return dev_id;
}

bool mpu6050_test_connection(void) {
    return mpu6050_get_device_id() >= 0xf4;
}

// BANK_SEL register

bool mpu6050_set_memory_bank(uint8_t bank, uint8_t prefetchEnabled, uint8_t userBank) {
    bool ret = 0;
    bank &= 0x1F;
    if (userBank) bank |= 0x20;
    if (prefetchEnabled) bank |= 0x40;
    soft_i2c_init();
    ret |= soft_i2c_transmit(mpu6050_addr);
    ret |= soft_i2c_write(MPU6050_RA_BANK_SEL);
    ret |= soft_i2c_write(bank);
    soft_i2c_stop();
    return ret;
}

// MEM_START_ADDR register

bool mpu6050_set_memory_start_address(uint8_t address) {
    bool ret = 0;
    soft_i2c_init();
    ret |= soft_i2c_transmit(mpu6050_addr);
    ret |= soft_i2c_write(MPU6050_RA_MEM_START_ADDR);
    ret |= soft_i2c_write(address);
    soft_i2c_stop();
    return ret;
}

// MEM_R_W register

uint8_t mpu6050_read_memory_byte() {
    uint8_t byte;
    soft_i2c_init();
    soft_i2c_transmit(mpu6050_addr);
    soft_i2c_write(MPU6050_RA_MEM_R_W);
    byte = soft_i2c_read(1);
    soft_i2c_stop();
    return byte;
}

// ======== UNDOCUMENTED/DMP REGISTERS/METHODS ========

// XG_OFFS_TC register

uint8_t mpu6050_get_otp_bank_valid() {
    uint8_t byte;
    soft_i2c_init();
    soft_i2c_transmit(mpu6050_addr);
    soft_i2c_write(MPU6050_RA_XG_OFFS_TC);
    byte = soft_i2c_read(1);
    soft_i2c_stop();
    return byte;
}
bool mpu6050_set_otp_bank_valid(bool enabled) {
    bool ret = 0;
    soft_i2c_init();
    ret |= soft_i2c_transmit(mpu6050_addr);
    ret |= soft_i2c_write(MPU6050_RA_XG_OFFS_TC);
    ret |= soft_i2c_write(enabled);
    soft_i2c_stop();
    return ret;
}
int8_t mpu6050_getXGyroOffsetTC() {
    uint8_t ret = 0;
    soft_i2c_init();
    soft_i2c_transmit(mpu6050_addr);
    soft_i2c_write(MPU6050_RA_XG_OFFS_TC);
    soft_i2c_write(MPU6050_TC_OFFSET_BIT);
    ret = soft_i2c_read(1);
    soft_i2c_stop();
    return ret;
}
void mpu6050_setXGyroOffsetTC(int8_t offset) {
}

bool mpu6050_dmp_initialize() {
    bool ret = 0, hwRevision, otpValid;
    mpu6050_reset();
    _delay_ms(30); // wait after reset

    // disable sleep mode
    /*DEBUG_PRINTLN(F("Disabling sleep mode..."));*/
    ret |= mpu6050_set_sleep_enabled(false);

    // get MPU hardware revision
    /*DEBUG_PRINTLN(F("Selecting user bank 16..."));*/


    /*setMemoryBank(0x10, true, true);*/
    ret |= mpu6050_set_memory_bank(0x10, true, true);
    /*DEBUG_PRINTLN(F("Selecting memory byte 6..."));*/
    ret |= mpu6050_set_memory_start_address(0x06);
    /*setMemoryStartAddress(0x06);*/
    /*DEBUG_PRINTLN(F("Checking hardware revision..."));*/
    hwRevision = mpu6050_read_memory_byte();
    /*DEBUG_PRINT(F("Revision @ user[16][6] = "));*/
    /*DEBUG_PRINTLNF(hwRevision, HEX);*/
    /*DEBUG_PRINTLN(F("Resetting memory bank selection to 0..."));*/
    /*setMemoryBank(0, false, false);*/
    ret |= mpu6050_set_memory_bank(0, false, false);

    /*// check OTP bank valid*/
    /*DEBUG_PRINTLN(F("Reading OTP bank valid flag..."));*/
    /*uint8_t otpValid = getOTPBankValid();*/
    otpValid = mpu6050_get_otp_bank_valid();
    /*DEBUG_PRINT(F("OTP bank is "));*/
    /*DEBUG_PRINTLN(otpValid ? F("valid!") : F("invalid!"));*/

    /*// get X/Y/Z gyro offsets*/
    /*DEBUG_PRINTLN(F("Reading gyro offset TC values..."));*/
    /*int8_t xgOffsetTC = getXGyroOffsetTC();*/
    /*int8_t ygOffsetTC = getYGyroOffsetTC();*/
    /*int8_t zgOffsetTC = getZGyroOffsetTC();*/
    /*DEBUG_PRINT(F("X gyro offset = "));*/
    /*DEBUG_PRINTLN(xgOffset);*/
    /*DEBUG_PRINT(F("Y gyro offset = "));*/
    /*DEBUG_PRINTLN(ygOffset);*/
    /*DEBUG_PRINT(F("Z gyro offset = "));*/
    /*DEBUG_PRINTLN(zgOffset);*/

    /*// setup weird slave stuff (?)*/
    /*DEBUG_PRINTLN(F("Setting slave 0 address to 0x7F..."));*/
    /*setSlaveAddress(0, 0x7F);*/
    /*DEBUG_PRINTLN(F("Disabling I2C Master mode..."));*/
    /*setI2CMasterModeEnabled(false);*/
    /*DEBUG_PRINTLN(F("Setting slave 0 address to 0x68 (self)..."));*/
    /*setSlaveAddress(0, 0x68);*/
    /*DEBUG_PRINTLN(F("Resetting I2C Master control..."));*/
    /*resetI2CMaster();*/
    /*delay(20);*/

    /*// load DMP code into memory banks*/
    /*DEBUG_PRINT(F("Writing DMP code to MPU memory banks ("));*/
    /*DEBUG_PRINT(MPU6050_DMP_CODE_SIZE);*/
    /*DEBUG_PRINTLN(F(" bytes)"));*/
    /*if (writeProgMemoryBlock(dmpMemory, MPU6050_DMP_CODE_SIZE)) {*/
    /*DEBUG_PRINTLN(F("Success! DMP code written and verified."));*/

    /*// write DMP configuration*/
    /*DEBUG_PRINT(F("Writing DMP configuration to MPU memory banks ("));*/
    /*DEBUG_PRINT(MPU6050_DMP_CONFIG_SIZE);*/
    /*DEBUG_PRINTLN(F(" bytes in config def)"));*/
    /*if (writeProgDMPConfigurationSet(dmpConfig, MPU6050_DMP_CONFIG_SIZE)) {*/
    /*DEBUG_PRINTLN(F("Success! DMP configuration written and verified."));*/

    /*DEBUG_PRINTLN(F("Setting clock source to Z Gyro..."));*/
    /*setClockSource(MPU6050_CLOCK_PLL_ZGYRO);*/

    /*DEBUG_PRINTLN(F("Setting DMP and FIFO_OFLOW interrupts enabled..."));*/
    /*setIntEnabled(0x12);*/

    /*DEBUG_PRINTLN(F("Setting sample rate to 200Hz..."));*/
    /*setRate(4); // 1khz / (1 + 4) = 200 Hz*/

    /*DEBUG_PRINTLN(F("Setting external frame sync to TEMP_OUT_L[0]..."));*/
    /*setExternalFrameSync(MPU6050_EXT_SYNC_TEMP_OUT_L);*/

    /*DEBUG_PRINTLN(F("Setting DLPF bandwidth to 42Hz..."));*/
    /*setDLPFMode(MPU6050_DLPF_BW_42);*/

    /*DEBUG_PRINTLN(F("Setting gyro sensitivity to +/- 2000 deg/sec..."));*/
    /*setFullScaleGyroRange(MPU6050_GYRO_FS_2000);*/

    /*DEBUG_PRINTLN(F("Setting DMP configuration bytes (function unknown)..."));*/
    /*setDMPConfig1(0x03);*/
    /*setDMPConfig2(0x00);*/

    /*DEBUG_PRINTLN(F("Clearing OTP Bank flag..."));*/
    /*setOTPBankValid(false);*/

    /*DEBUG_PRINTLN(F("Setting X/Y/Z gyro offset TCs to previous values..."));*/
    /*setXGyroOffsetTC(xgOffsetTC);*/
    /*setYGyroOffsetTC(ygOffsetTC);*/
    /*setZGyroOffsetTC(zgOffsetTC);*/

    /*//DEBUG_PRINTLN(F("Setting X/Y/Z gyro user offsets to zero..."));*/
    /*//setXGyroOffset(0);*/
    /*//setYGyroOffset(0);*/
    /*//setZGyroOffset(0);*/

    /*DEBUG_PRINTLN(F("Writing final memory update 1/7 (function unknown)..."));*/
    /*uint8_t dmpUpdate[16], j;*/
    /*uint16_t pos = 0;*/
    /*for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);*/
    /*writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);*/

    /*DEBUG_PRINTLN(F("Writing final memory update 2/7 (function unknown)..."));*/
    /*for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);*/
    /*writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);*/

    /*DEBUG_PRINTLN(F("Resetting FIFO..."));*/
    /*resetFIFO();*/

    /*DEBUG_PRINTLN(F("Reading FIFO count..."));*/
    /*uint16_t fifoCount = getFIFOCount();*/
    /*uint8_t fifoBuffer[128];*/

    /*DEBUG_PRINT(F("Current FIFO count="));*/
    /*DEBUG_PRINTLN(fifoCount);*/
    /*getFIFOBytes(fifoBuffer, fifoCount);*/

    /*DEBUG_PRINTLN(F("Setting motion detection threshold to 2..."));*/
    /*setMotionDetectionThreshold(2);*/

    /*DEBUG_PRINTLN(F("Setting zero-motion detection threshold to 156..."));*/
    /*setZeroMotionDetectionThreshold(156);*/

    /*DEBUG_PRINTLN(F("Setting motion detection duration to 80..."));*/
    /*setMotionDetectionDuration(80);*/

    /*DEBUG_PRINTLN(F("Setting zero-motion detection duration to 0..."));*/
    /*setZeroMotionDetectionDuration(0);*/

    /*DEBUG_PRINTLN(F("Resetting FIFO..."));*/
    /*resetFIFO();*/

    /*DEBUG_PRINTLN(F("Enabling FIFO..."));*/
    /*setFIFOEnabled(true);*/

    /*DEBUG_PRINTLN(F("Enabling DMP..."));*/
    /*setDMPEnabled(true);*/

    /*DEBUG_PRINTLN(F("Resetting DMP..."));*/
    /*resetDMP();*/

    /*DEBUG_PRINTLN(F("Writing final memory update 3/7 (function unknown)..."));*/
    /*for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);*/
    /*writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);*/

    /*DEBUG_PRINTLN(F("Writing final memory update 4/7 (function unknown)..."));*/
    /*for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);*/
    /*writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);*/

    /*DEBUG_PRINTLN(F("Writing final memory update 5/7 (function unknown)..."));*/
    /*for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);*/
    /*writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);*/

    /*DEBUG_PRINTLN(F("Waiting for FIFO count > 2..."));*/
    /*while ((fifoCount = getFIFOCount()) < 3);*/

    /*DEBUG_PRINT(F("Current FIFO count="));*/
    /*DEBUG_PRINTLN(fifoCount);*/
    /*DEBUG_PRINTLN(F("Reading FIFO data..."));*/
    /*getFIFOBytes(fifoBuffer, fifoCount);*/

    /*DEBUG_PRINTLN(F("Reading interrupt status..."));*/
    /*uint8_t mpuIntStatus = getIntStatus();*/

    /*DEBUG_PRINT(F("Current interrupt status="));*/
    /*DEBUG_PRINTLNF(mpuIntStatus, HEX);*/

    /*DEBUG_PRINTLN(F("Reading final memory update 6/7 (function unknown)..."));*/
    /*for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);*/
    /*readMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);*/

    /*DEBUG_PRINTLN(F("Waiting for FIFO count > 2..."));*/
    /*while ((fifoCount = getFIFOCount()) < 3);*/

    /*DEBUG_PRINT(F("Current FIFO count="));*/
    /*DEBUG_PRINTLN(fifoCount);*/

    /*DEBUG_PRINTLN(F("Reading FIFO data..."));*/
    /*getFIFOBytes(fifoBuffer, fifoCount);*/

    /*DEBUG_PRINTLN(F("Reading interrupt status..."));*/
    /*mpuIntStatus = getIntStatus();*/

    /*DEBUG_PRINT(F("Current interrupt status="));*/
    /*DEBUG_PRINTLNF(mpuIntStatus, HEX);*/

    /*DEBUG_PRINTLN(F("Writing final memory update 7/7 (function unknown)..."));*/
    /*for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);*/
    /*writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);*/

    /*DEBUG_PRINTLN(F("DMP is good to go! Finally."));*/

    /*DEBUG_PRINTLN(F("Disabling DMP (you turn it on later)..."));*/
    /*setDMPEnabled(false);*/

    /*DEBUG_PRINTLN(F("Setting up internal 42-byte (default) DMP packet buffer..."));*/
    /*dmpPacketSize = 42;*/
    /*            *//*if ((dmpPacketBuffer = (uint8_t *)malloc(42)) == 0) {*/
    /*return 3; // TODO: proper error code for no memory*/
    /*            }*/

    /*DEBUG_PRINTLN(F("Resetting FIFO and clearing INT status one last time..."));*/
    /*resetFIFO();*/
    /*getIntStatus();*/
    /*} else {*/
    /*DEBUG_PRINTLN(F("ERROR! DMP configuration verification failed."));*/
    /*return 2; // configuration block loading failed*/
    /*}*/
    /*} else {*/
    /*DEBUG_PRINTLN(F("ERROR! DMP code verification failed."));*/
    /*return 1; // main binary block loading failed*/
    /*}*/
    return ret;
}
