/*
 * sg90.c
 *
 *  Created on: 2026年3月16日
 *      Author: 18701
 */

#include "sg90.h"

motor_t motors[MOTOR_COUNT];
float motor_snapshot[3][MOTOR_COUNT];
const uint32_t tim_channel_lut[4] = {TIM_CHANNEL_1,TIM_CHANNEL_2,TIM_CHANNEL_3,TIM_CHANNEL_4};

HAL_StatusTypeDef sg90_set_angle(TIM_HandleTypeDef* htim, const motor_t* motor)
{
	if (motor -> angle > 90 || motor -> angle < -90){
		return HAL_ERROR;
	}

	if (motor -> angle == 90) {
		__HAL_TIM_SET_COMPARE(htim, motor -> tim_channel, PWM_MAX);
		return HAL_OK;
	}
	if (motor -> angle == -90) {
		__HAL_TIM_SET_COMPARE(htim, motor -> tim_channel, PWM_MIN);
		return HAL_OK;
	}
	if (motor -> angle == 0) {
		__HAL_TIM_SET_COMPARE(htim, motor -> tim_channel, PWM_ZERO);
		return HAL_OK;
	}

	uint32_t digital = (uint32_t)((float)((float)(motor -> angle + 90.0) / 180.0) * PWM_RANGE) + PWM_MIN;
	__HAL_TIM_SET_COMPARE(htim, motor -> tim_channel, digital);
	return HAL_OK;

}

HAL_StatusTypeDef sg90_init (TIM_HandleTypeDef* htim)
{
	HAL_TIM_PWM_Init(htim);

	for(int i = 0; i < MOTOR_COUNT; i ++){
		motors[i].angle = 0;
		motors[i].tim_channel = tim_channel_lut[i];
		if(sg90_set_angle(htim, &motors[i]) != HAL_OK) return HAL_ERROR;
	}

	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_4);

	return HAL_OK;
}

void sg90_sweep_test (TIM_HandleTypeDef* htim)
{

	for (int current_angle = 0; current_angle <= 90; current_angle++){
		for (int i = 0; i < MOTOR_COUNT; i ++){
			motors[i].angle = current_angle;
			sg90_set_angle(htim, &motors[i]);
		}
		HAL_Delay(10);
	}

	for (int current_angle = 90; current_angle >= -90; current_angle--){
		for (int i = 0; i < MOTOR_COUNT; i ++){
			motors[i].angle = current_angle;
			sg90_set_angle(htim, &motors[i]);
		}
		HAL_Delay(10);
	}
}

void sg90_set_zero (TIM_HandleTypeDef* htim)
{
	for (int i = 0; i < MOTOR_COUNT; i ++){
		motors[i].angle = 0;
		sg90_set_angle(htim, &motors[i]);
	}

}

void sg90_motor_play_back(TIM_HandleTypeDef* htim){

}


