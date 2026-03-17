/*
 * sg90.h
 *
 *  Created on: 2026年3月16日
 *      Author: 18701
 */

#ifndef INC_SG90_H_
#define INC_SG90_H_

// This is for the sg90 servo motor

// Cubemx config:
/*
 * Using TIM2, enable all channels
 * SG 90 PWM Period is 20ms. TIM2 ARR & CCR has 32bit, and clock freq is 84mhz
 * SO the ARR is configured to 1680673 * 11.9ns = 20ms
 */
#include "main.h"

#define PWM_PERIOD 		1680673
#define PWM_PERIOD_uS	0.0119f

#define PWM_MIN 		42017
#define PWM_MIN_uS		500 //-90deg

#define PWM_MAX			201681
#define PWM_MAX_uS		2400 //90deg

#define PWM_ZERO 		(PWM_MAX + PWM_MIN)/2

#define PWM_RANGE		PWM_MAX - PWM_MIN

#define MOTOR_COUNT		4

typedef struct {
	int32_t angle;
	uint32_t tim_channel;
} motor_t;

extern motor_t motors[MOTOR_COUNT];

HAL_StatusTypeDef sg90_set_angle(TIM_HandleTypeDef* htim, const motor_t* motor);
HAL_StatusTypeDef sg90_init (TIM_HandleTypeDef* htim);
void sg90_sweep_test (TIM_HandleTypeDef* htim);


#endif /* INC_SG90_H_ */
