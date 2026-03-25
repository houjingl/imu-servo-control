/*
 * step_motor.c
 *
 *  Created on: 2026年3月25日
 *      Author: 18701
 */

#include "step_motor.h"



int32_t current_absolute_steps = 0;
int32_t target_absolute_steps = 0;
uint32_t last_step_tick = 0;

const uint8_t step_sequence[8][4] = {
    {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0},
    {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1}
};

void Stepper_SetPhase(int32_t step_index)
{
    int8_t phase = step_index % 8;
    if (phase < 0) phase += 8;

    HAL_GPIO_WritePin(STEP_PIN_0_GPIO_Port, STEP_PIN_0_Pin, step_sequence[phase][0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STEP_PIN_1_GPIO_Port, STEP_PIN_1_Pin, step_sequence[phase][1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STEP_PIN_2_GPIO_Port, STEP_PIN_2_Pin, step_sequence[phase][2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STEP_PIN_3_GPIO_Port, STEP_PIN_3_Pin, step_sequence[phase][3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Stepper_Update(uint32_t global_tick)
{
    if (current_absolute_steps != target_absolute_steps) {
        if ((global_tick - last_step_tick) >= STEP_DELAY_MS) {
            last_step_tick = global_tick;

            if (target_absolute_steps > current_absolute_steps) {
                current_absolute_steps++;
            } else {
                current_absolute_steps--;
            }

            Stepper_SetPhase(current_absolute_steps);
        }
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
    }
}

void Stepper_SetTargetAngle(float target_angle)
{
    target_absolute_steps = (int32_t)((target_angle / 360.0f) * STEPS_PER_REV);
}

void Stepper_MoveRelativeSteps(int32_t steps)
{
    target_absolute_steps += steps;
}

void Stepper_ResetZero(void)
{
    current_absolute_steps = 0;
    target_absolute_steps = 0;
}
