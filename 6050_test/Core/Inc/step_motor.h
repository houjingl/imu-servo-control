/*
 * step_motor.h
 *
 *  Created on: 2026年3月25日
 *      Author: 18701
 */

#ifndef INC_STEP_MOTOR_H_
#define INC_STEP_MOTOR_H_
#include "main.h"


#define STEPS_PER_REV 4076.0f
#define STEP_DELAY_MS 1

#define STEP_MOTOR_COUNT 1

extern int32_t current_absolute_steps;
extern int32_t target_absolute_steps;

void Stepper_SetPhase(int32_t step_index);
void Stepper_Update(uint32_t global_tick);
//void Stepper_SetTargetAngle(float target_angle);
//void Stepper_MoveRelativeSteps(int32_t steps);
//void Stepper_ResetZero(void);


#endif /* INC_STEP_MOTOR_H_ */
