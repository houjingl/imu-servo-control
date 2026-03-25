/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu6050.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

extern volatile uint8_t imu_flag;
extern volatile int16_t accel_data[3];
extern volatile int16_t gyro_data[3];
extern float accel_g[3];
extern float gyro_dps[3];
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern uint16_t cur_time;
extern uint16_t last_time;

extern float gyro_yaw_zero_bias;

extern uint16_t counts;
extern char buffer[100];

extern uint16_t SSD1306_CurrentX;
extern uint16_t SSD1306_CurrentY;

extern int8_t current_col;
extern int8_t last;
extern int8_t pressed;
extern uint64_t disable;
extern uint64_t disable_start;

extern uint8_t motor_set_zero;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define COL1_Pin GPIO_PIN_4
#define COL1_GPIO_Port GPIOE
#define COL1_EXTI_IRQn EXTI4_IRQn
#define COL2_Pin GPIO_PIN_5
#define COL2_GPIO_Port GPIOE
#define COL2_EXTI_IRQn EXTI9_5_IRQn
#define COL3_Pin GPIO_PIN_6
#define COL3_GPIO_Port GPIOE
#define COL3_EXTI_IRQn EXTI9_5_IRQn
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define USER_Btn_EXTI_IRQn EXTI15_10_IRQn
#define JOYSTICK_BTN_Pin GPIO_PIN_3
#define JOYSTICK_BTN_GPIO_Port GPIOF
#define JOYSTICK_BTN_EXTI_IRQn EXTI3_IRQn
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define VRX_Pin GPIO_PIN_0
#define VRX_GPIO_Port GPIOC
#define VRY_Pin GPIO_PIN_3
#define VRY_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define imu_exti_pin_Pin GPIO_PIN_9
#define imu_exti_pin_GPIO_Port GPIOC
#define imu_exti_pin_EXTI_IRQn EXTI9_5_IRQn
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define ROW4_Pin GPIO_PIN_4
#define ROW4_GPIO_Port GPIOD
#define ROW3_Pin GPIO_PIN_5
#define ROW3_GPIO_Port GPIOD
#define ROW2_Pin GPIO_PIN_6
#define ROW2_GPIO_Port GPIOD
#define ROW1_Pin GPIO_PIN_7
#define ROW1_GPIO_Port GPIOD
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
void print_msg(char * msg);
#define ROW_Port GPIOD
#define COL_Port GPIOE
#define TIMER_SCALE 				1000000.0f
#define __CALC_DT(_CUR_TIME_)	 	(float)_CUR_TIME_ / TIMER_SCALE


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
