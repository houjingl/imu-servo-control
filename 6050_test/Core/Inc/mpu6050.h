/*
 * mpu6050.h
 *
 *  Created on: 2026年3月8日
 *      Author: 18701
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_
#include <string.h>
#include <stdio.h>
#include "main.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


#define TIMEOUT                   1000
#define MPU6050_DEVICE_ADDRESS    0xD0
#define MPU6050_REG_WHO_AM_I      0x75
#define MPU6050_REG_PWR_MGMT_1    0x6B
#define MPU6050_REG_PWR_MGMT_2    0x6C
#define MPU6050_REG_ACCEL_CONFIG  0x1C
#define MPU6050_REG_ACCEL_XOUT_H  0x3B
#define MPU6050_REG_GYRO_CONFIG   0x1B
#define MPU6050_REG_GYRO_XOUT_H   0x43
#define MPU6050_REG_TEMP_OUT_H    0x41
#define MPU6050_REG_SMPLRT_DIV	  0x19
#define MPU6050_REG_INT_EN 		  0x38

#define MPU6050_ACCEL_RANGE_2G     0x00
#define MPU6050_ACCEL_RANGE_4G     0x01
#define MPU6050_ACCEL_RANGE_8G     0x02
#define MPU6050_ACCEL_RANGE_16G    0x03

#define MPU6050_GYRO_RANGE_250     0x00
#define MPU6050_GYRO_RANGE_500     0x01
#define MPU6050_GYRO_RANGE_1000    0x02
#define MPU6050_GYRO_RANGE_2000    0x03

#define ACCEL_SCALE_FACTOR 16384.0f
#define GYRO_SCALE_FACTOR 131.0f

typedef struct{
	uint8_t ClkSel: 3;
	uint8_t Temp_Dis: 1;
	uint8_t Reserved: 1;
	uint8_t Cycle: 1;
	uint8_t Sleep: 1;
	uint8_t Device_Reset: 1;
}PowerManagementRegister_t;

typedef struct{
	uint8_t Reserved: 3;
	uint8_t AFS_Sel: 2;
	uint8_t ZA_ST: 1;
	uint8_t YA_ST: 1;
	uint8_t XA_ST: 1;
}AccelConfigRegister_t;

typedef struct{
	uint8_t Reserved: 3;
	uint8_t FS_Sel: 2;
	uint8_t ZG_ST: 1;
	uint8_t YG_ST: 1;
	uint8_t XG_ST:1;
}GyroConfigRegister_t;

typedef struct {
    float roll;	//around X
    float pitch; //around Y
    float yaw; //around Z
} IMU_Angles_t;

HAL_StatusTypeDef mpu6050_read_reg(I2C_HandleTypeDef* hi2c, uint8_t reg_addr, uint16_t data_size, uint8_t* data_buf);
HAL_StatusTypeDef mpu6050_write_reg(I2C_HandleTypeDef* hi2c, uint8_t reg_addr, uint8_t value);
HAL_StatusTypeDef mpu6050_init(I2C_HandleTypeDef* hi2c, uint8_t AFS_SEL, uint8_t FS_SEL);
void mpu6050_getAccelValue(I2C_HandleTypeDef *hi2cx, int16_t *accelData);
void mpu6050_getGyroValue(I2C_HandleTypeDef *hi2cx, int16_t *gyroData);
void mpu6050_toFloat(float* accel, float* gyro, int16_t* accel_raw, int16_t* gyro_raw);
void mpu6050_calculate_angles(IMU_Angles_t *angles, float accel_g[3], float gyro_dps[3], float dt) ;



#endif /* INC_MPU6050_H_ */
