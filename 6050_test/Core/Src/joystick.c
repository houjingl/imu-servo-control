#include "joystick.h"
#include <stdlib.h>


uint16_t adc_values[2] = {0};
uint16_t* vx = &adc_values[0];
uint16_t* vy = &adc_values[1];

HAL_StatusTypeDef adc_dma_init(ADC_HandleTypeDef* hadc)
{
	return HAL_ADC_Start_DMA(hadc, adc_values, 2);
}

float current_servo_angle = 90.0f;
float current_stepper_angle = 0.0f;
static uint8_t last_motor_set_zero = 0;

//function to be called inside main every 20ms
void joystick_control(uint8_t motor_set_zero_flag)
{
	int16_t x_offset = (int16_t)(*vx) - JOYSTICK_CENTER;
	int16_t y_offset = (int16_t)(*vy) - JOYSTICK_CENTER;
	//get relative x and y axis offset relative to the joystick center value

	if (abs(x_offset) < JOYSTICK_DEADZONE) x_offset = 0;
	if (abs(y_offset) < JOYSTICK_DEADZONE) y_offset = 0;
	//detect dead zone

	//map the x and y offset to the servo and step motor speed scale
	float delta_servo_angle = x_offset * SERVO_SPEED_SCALE;
	float delta_stepper_angle = y_offset * STEPPER_SPEED_SCALE;

	//this is for the servo motor -> I used the motor for yaw
	if (motor_set_zero == 0) {
		current_servo_angle += delta_servo_angle;

		if (current_servo_angle > 180.0f) current_servo_angle = 180.0f;
		if (current_servo_angle < 0.0f)   current_servo_angle = 0.0f;

		motors[2].angle = current_servo_angle;
		sg90_set_angle(&htim2, &motors[2]);
	}

	//this is for updating the step motor angle
	if (motor_set_zero == 1) {
		if (delta_stepper_angle != 0.0f) {
			int32_t steps_to_move = (int32_t)((delta_stepper_angle / 360.0f) * 4076.0f);
			if (steps_to_move != 0) {
				Stepper_MoveRelativeSteps(steps_to_move);
			}
		}
		current_stepper_angle = 0.0f;
	}
	else {
		if (last_motor_set_zero == 1) {
			Stepper_ResetZero();
		}

		current_stepper_angle += delta_stepper_angle;

		if (current_stepper_angle > 80.0f)  current_stepper_angle = 80.0f;
		if (current_stepper_angle < -80.0f) current_stepper_angle = -80.0f;

		Stepper_SetTargetAngle(current_stepper_angle);
	}

	last_motor_set_zero = motor_set_zero;
}
