#include "joystick.h"
#include <stdlib.h>

uint16_t adc_values[2] = {0};
uint16_t* vx = &adc_values[0];
uint16_t* vy = &adc_values[1];

HAL_StatusTypeDef adc_dma_init(ADC_HandleTypeDef* hadc)
{
	return HAL_ADC_Start_DMA(hadc, adc_values, 2);
}

uint8_t last_motor_set_zero = 0; //a neg edge trigger flag
float target_servo_angle = 0;

//function to be called inside main every 20ms
void joystick_control(TIM_HandleTypeDef* htim, uint8_t motor_set_zero_flag)
{
	int16_t x_offset = (int16_t)(*vx) - JOYSTICK_CENTER;
	int16_t y_offset = (int16_t)(*vy) - JOYSTICK_CENTER;
	//get relative x and y axis offset relative to the joystick center value

	if (abs(x_offset) < JOYSTICK_DEADZONE) x_offset = 0;
	if (abs(y_offset) < JOYSTICK_DEADZONE) y_offset = 0;
	//detect dead zone

	//map the x and y offset to the servo and step motor speed scale
	float delta_servo_angle = x_offset * SERVO_SPEED_SCALE;
	int32_t delta_stepper_steps = y_offset / STEPPER_SPEED_DIVISOR;

	//this is for the servo motor -> I used the motor for yaw
	if (motor_set_zero_flag == 0) {
		target_servo_angle += delta_servo_angle;

		if (target_servo_angle > 90.0f) target_servo_angle = 90.0f;
		if (target_servo_angle < -90.0f)   target_servo_angle = -90.0f;
	}
	else {
		target_servo_angle = 0.0f;
	}

	//this is for updating the step motor angle
	if (motor_set_zero_flag == 1) {
	        if (delta_stepper_steps != 0) {
	            target_absolute_steps += delta_stepper_steps;
	            // so that the motor still rotates while we are resetting
	        }
	    }
	else {
		if (last_motor_set_zero == 1) {
			current_absolute_steps = 0;
			target_absolute_steps = 0;
			//due to the neg edge trigger, now the motor is completely reset to zero
		}

		target_absolute_steps += delta_stepper_steps;

		if (target_absolute_steps > STEPPER_MAX_STEPS)  target_absolute_steps = STEPPER_MAX_STEPS;
		if (target_absolute_steps < -STEPPER_MAX_STEPS) target_absolute_steps = -STEPPER_MAX_STEPS;
	}

	last_motor_set_zero = motor_set_zero_flag;
}
