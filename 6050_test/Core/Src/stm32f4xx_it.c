/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "mpu6050.h"
#include "ssd1306.h"
#include "sg90.h"
#include "joystick.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t last_btn_intr_time = 0;
static uint16_t servo_update_counter = 0;


volatile float current_arr = 10000.0f;

#define ARR_MIN_SPEED 10000.0f
#define ARR_MAX_SPEED 100.0f
#define DECEL_STEPS   200

float arr_alpha = 0.05f;

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim7;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line 3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */
	if (__HAL_GPIO_EXTI_GET_IT(JOYSTICK_BTN_Pin) != RESET && (HAL_GetTick() - last_btn_intr_time >= 10)) {
		claw_open_flag ^= 1;
		last_btn_intr_time = HAL_GetTick();
	}

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(JOYSTICK_BTN_Pin);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line 4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */

	if (__HAL_GPIO_EXTI_GET_IT(PLAYBACK_Pin) != RESET && (HAL_GetTick() - last_btn_intr_time >= 10)) {
		motor_play_back ^=1;
		last_btn_intr_time = HAL_GetTick();
		if (motor_play_back) HAL_TIM_Base_Stop_IT(&htim3);
		else HAL_TIM_Base_Start_IT(&htim3);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
	}

  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(PLAYBACK_Pin);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */
	if (__HAL_GPIO_EXTI_GET_FLAG(imu_exti_pin_Pin)){
		cur_time = __HAL_TIM_GET_COUNTER(&htim1);
		HAL_TIM_Base_Stop(&htim1);
		__HAL_TIM_SET_COUNTER(&htim1, 0);
		imu_flag = 1;
		mpu6050_getAccelValue(&hi2c2, accel_data);
		mpu6050_getGyroValue(&hi2c2, gyro_data);
		mpu6050_toFloat(accel_g, gyro_dps, accel_data, gyro_data);
		if(imu_flag) HAL_TIM_Base_Start(&htim1);
		if (!motor_set_zero && !motor_play_back){
			mpu6050_calculate_angles(&imu_angles, accel_g, gyro_dps, __CALC_DT(cur_time));
			motors[0].angle = imu_angles.roll;
			sg90_set_angle(&htim2, &motors[0]);
			motors[1].angle = imu_angles.pitch;
			sg90_set_angle(&htim2, &motors[1]);
//			motors[2].angle = imu_angles.yaw;
//			sg90_set_angle(&htim2, &motors[2]);
		}

		counts ++;
		if (counts >= 17){
			SSD1306_CurrentX=0;
			  SSD1306_CurrentY=0;
			  //char buffer[100];
			  sprintf(buffer, " Roll: %.3f", imu_angles.roll);//Y angle: %.2f\nZ angle: %.2f
			  SSD1306_Puts(buffer, &Font_7x10);
			  SSD1306_CurrentX=0;
			  SSD1306_CurrentY=20;
			  sprintf(buffer, " Pitch: %.3f",  imu_angles.pitch);
			  SSD1306_Puts(buffer, &Font_7x10);
			  SSD1306_CurrentX=0;
			  SSD1306_CurrentY=40;
			  if(motor_set_zero){
				  sprintf(buffer, "ZEROING");
				  SSD1306_Puts(buffer, &Font_7x10);
			  } else if (motor_play_back){
				  sprintf(buffer, "PLAYBACK");
				  SSD1306_Puts(buffer, &Font_7x10);
			  } else {
				  sprintf(buffer, "          ");
				  SSD1306_Puts(buffer, &Font_7x10);
			  }

			  SSD1306_UpdateScreen();
		}
	}

	if (__HAL_GPIO_EXTI_GET_IT(SNAPSHOT_1_Pin) != RESET && (HAL_GetTick() - last_btn_intr_time >= 10)) {
		for(int j = 0; j < MOTOR_COUNT; j++) {
			motor_snapshot[0][j] = motors[j].angle;
		}
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		last_btn_intr_time = HAL_GetTick();
	  }
	if (__HAL_GPIO_EXTI_GET_IT(SNAPSHOT_2_Pin) != RESET && (HAL_GetTick() - last_btn_intr_time >= 10)) {
		for(int j = 0; j < MOTOR_COUNT; j++) {
			motor_snapshot[1][j] = motors[j].angle;
		}
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		last_btn_intr_time = HAL_GetTick();
	}
	if (__HAL_GPIO_EXTI_GET_IT(SNAPSHOT_3_Pin) != RESET && (HAL_GetTick() - last_btn_intr_time >= 10)) {
		for(int j = 0; j < MOTOR_COUNT; j++) {
			motor_snapshot[2][j] = motors[j].angle;
		}
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		last_btn_intr_time = HAL_GetTick();
	}


  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(SNAPSHOT_3_Pin);
  HAL_GPIO_EXTI_IRQHandler(SNAPSHOT_2_Pin);
  HAL_GPIO_EXTI_IRQHandler(SNAPSHOT_1_Pin);
  HAL_GPIO_EXTI_IRQHandler(imu_exti_pin_Pin);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */



  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
	if (!motor_play_back){
		servo_update_counter++;

		if (servo_update_counter >= 0) {
			servo_update_counter = 0;

			float alpha = 0.05f;
			float error = target_servo_angle - joystick_motors[0]->angle;

			if (fabs(error) < 0.5f) {
				joystick_motors[0]->angle = target_servo_angle;
			} else {
				joystick_motors[0]->angle += (error * alpha);
			}

			if (joystick_motors[0]->angle >= 90.0f) {
				joystick_motors[0]->angle = 90.0f;
			} else if (joystick_motors[0]->angle <= -90.0f) {
				joystick_motors[0]->angle = -90.0f;
			}

			sg90_set_angle(&htim2, joystick_motors[0]);
		}
	}

//	Stepper_Update_500us();

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
	if (!motor_play_back || motor_set_zero){
	  adc_dma_init(&hadc1);
	  joystick_control(&htim2, motor_set_zero);
	}

  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
	if (__HAL_GPIO_EXTI_GET_IT(USER_Btn_Pin) != RESET && (HAL_GetTick() - last_btn_intr_time >= 10)){
		motor_set_zero ^= 1;
		last_btn_intr_time = HAL_GetTick();
	}

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(USER_Btn_Pin);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */
	if (current_absolute_steps == target_absolute_steps) {
		HAL_TIM_Base_Stop_IT(&htim7);
		Stepper_Update_500us();
		return;
	}

	Stepper_Update_500us();

	int32_t remaining = abs(target_absolute_steps - current_absolute_steps);
	float target_arr;

	if (remaining <= DECEL_STEPS) {
		target_arr = ARR_MIN_SPEED;
	} else {
		target_arr = ARR_MAX_SPEED;
	}

	current_arr += (target_arr - current_arr) * arr_alpha;

	__HAL_TIM_SET_AUTORELOAD(&htim7, (uint32_t)current_arr);

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/* USER CODE BEGIN 1 */



/* USER CODE END 1 */
