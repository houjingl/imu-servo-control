#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "main.h"
#include "sg90.h"
#include "step_motor.h"


extern uint16_t* vx;
extern uint16_t* vy;
extern float target_servo_angle;

#define ADC_RANGE 4095
#define JOYSTICK_CENTER   2048
#define JOYSTICK_DEADZONE 150
#define SERVO_SPEED_SCALE   0.002f

#define STEPPER_SPEED_DIVISOR 1000
#define STEPPER_MAX_STEPS     905

HAL_StatusTypeDef adc_dma_init(ADC_HandleTypeDef* hadc);
void joystick_control(TIM_HandleTypeDef* htim, uint8_t motor_set_zero_flag);




#endif
