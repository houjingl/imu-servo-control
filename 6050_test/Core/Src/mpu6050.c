/*
 * mpu6050.c
 *
 *  Created on: 2026年3月10日
 *      Author: 18701
 */

#include "mpu6050.h"

IMU_Angles_t imu_angles;

HAL_StatusTypeDef mpu6050_read_reg(I2C_HandleTypeDef* hi2c, uint8_t reg_addr, uint16_t data_size, uint8_t* data_buf)
{
	if (HAL_I2C_Mem_Read(hi2c, MPU6050_DEVICE_ADDRESS, reg_addr, 1, data_buf, data_size, TIMEOUT) != HAL_OK){
		print_msg("read reg failed\r\n");
		return HAL_ERROR;
	}
	return HAL_OK;
}

HAL_StatusTypeDef mpu6050_write_reg(I2C_HandleTypeDef* hi2c, uint8_t reg_addr, uint8_t value)
{
	uint8_t payload[2];
	payload[0] = reg_addr;
	payload[1] = value;

	if (HAL_I2C_Master_Transmit(hi2c, MPU6050_DEVICE_ADDRESS, payload, 2, TIMEOUT) != HAL_OK){
		print_msg("write reg failed\r\n");
		return HAL_ERROR;
	}

	return HAL_OK;
}

HAL_StatusTypeDef mpu6050_init(I2C_HandleTypeDef* hi2c, uint8_t AFS_SEL, uint8_t FS_SEL)
{
	uint8_t msg[100];
	if (HAL_I2C_IsDeviceReady(hi2c, MPU6050_DEVICE_ADDRESS, 1, TIMEOUT) != HAL_OK){
		print_msg("device not ready\r\n");
		return HAL_ERROR;
	}

	uint8_t temp_reg = 0;


	if (mpu6050_read_reg(hi2c, MPU6050_REG_WHO_AM_I, 1, &temp_reg) != HAL_OK){
		return HAL_ERROR;
	}

	if (temp_reg != 0x68){
		sprintf(msg, "Wrong product id (0x%x)\r\n", temp_reg);
		print_msg(msg);
		return HAL_ERROR;
	}
	// Writing 0x00 to the pwr management reg 0x6b to wake up the mpu6050
	PowerManagementRegister_t powerManagement={0};
	powerManagement.ClkSel = 0;
	powerManagement.Temp_Dis = 0;
	powerManagement.Reserved = 0;
	powerManagement.Cycle = 0;
	powerManagement.Sleep = 0;
	powerManagement.Device_Reset = 0;
	temp_reg = *((uint8_t*)&powerManagement);
	if (mpu6050_write_reg(hi2c, MPU6050_REG_PWR_MGMT_1, temp_reg) != HAL_OK){
		return HAL_ERROR;
	}
	// init accel reg -> AFS sel indicates range
	AccelConfigRegister_t accelConfig = {0};
	accelConfig.Reserved = 0;
	accelConfig.AFS_Sel = AFS_SEL;
	accelConfig.ZA_ST = 0;
	accelConfig.YA_ST = 0;
	accelConfig.XA_ST = 0;
	temp_reg = *((uint8_t*)&accelConfig);
	if (mpu6050_write_reg(hi2c, MPU6050_REG_ACCEL_CONFIG, temp_reg) != HAL_OK){
		return HAL_ERROR;
	}
	// init gyro reg -> fs_sel indicates gyro range
	GyroConfigRegister_t gyroConfig = {0};
	gyroConfig.Reserved = 0;
	gyroConfig.FS_Sel = FS_SEL;
	gyroConfig.ZG_ST = 0;
	gyroConfig.YG_ST = 0;
	gyroConfig.XG_ST = 0;
	temp_reg = *((uint8_t*)&gyroConfig);
	if (mpu6050_write_reg(hi2c, MPU6050_REG_GYRO_CONFIG, temp_reg) != HAL_OK){
		return HAL_ERROR;
	}

	// setting the sampling frequency to 1k hz
	if (mpu6050_write_reg(hi2c, MPU6050_REG_SMPLRT_DIV, 0x7) != HAL_OK){
		return HAL_ERROR;
	}

	if (mpu6050_write_reg(hi2c, MPU6050_REG_INT_EN, 0x1) != HAL_OK){
		return HAL_ERROR;
	}


	return HAL_OK;
}

void mpu6050_getAccelValue(I2C_HandleTypeDef *hi2cx, int16_t *accelData)
{
	uint8_t data[6]={0};
	mpu6050_read_reg(hi2cx, MPU6050_REG_ACCEL_XOUT_H, 6, data);
	accelData[0]=(int16_t)(data[0]<<8 | data[1]);
	accelData[1]=(int16_t)(data[2]<<8 | data[3]);
	accelData[2]=(int16_t)(data[4]<<8 | data[5]);
}

void mpu6050_getGyroValue(I2C_HandleTypeDef *hi2cx, int16_t *gyroData)
{
	uint8_t data[6]={0};
	mpu6050_read_reg(hi2cx, MPU6050_REG_GYRO_XOUT_H, 6, data);
	gyroData[0]=(int16_t)(data[0]<<8 | data[1]);
	gyroData[1]=(int16_t)(data[2]<<8 | data[3]);
	gyroData[2]=(int16_t)(data[4]<<8 | data[5]);
}

void mpu6050_toFloat(float* accel, float* gyro, int16_t* accel_raw, int16_t* gyro_raw)
{
    // Convert Accelerometer data (X, Y, Z)
    accel[0] = accel_raw[0] / ACCEL_SCALE_FACTOR;
    accel[1] = accel_raw[1] / ACCEL_SCALE_FACTOR;
    accel[2] = accel_raw[2] / ACCEL_SCALE_FACTOR;

    // Convert Gyroscope data (X, Y, Z)
    gyro[0] = gyro_raw[0] / GYRO_SCALE_FACTOR;
    gyro[1] = gyro_raw[1] / GYRO_SCALE_FACTOR;
    gyro[2] = gyro_raw[2] / GYRO_SCALE_FACTOR;
}

void mpu6050_calculate_angles(IMU_Angles_t *angles, float accel_g[3], float gyro_dps[3], float dt)
{
    float accel_roll  = atan2f(accel_g[1], accel_g[2]) * (180.0f / M_PI);
    float accel_pitch = atan2f(-accel_g[0], sqrtf(accel_g[1]*accel_g[1] + accel_g[2]*accel_g[2])) * (180.0f / M_PI);

    angles->roll  = 0.9f * (angles->roll  + (gyro_dps[0] * dt)) + (0.1f * accel_roll);
    angles->pitch = 0.9f * (angles->pitch + (gyro_dps[1] * dt)) + (0.1f * accel_pitch);

    float actual_gyro_z = gyro_dps[2];


    angles->yaw = angles->yaw + (actual_gyro_z * dt);
}


