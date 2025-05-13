/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rv2123.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SET_BTN_Pin GPIO_PIN_14
#define SET_BTN_GPIO_Port GPIOC
#define WAKE_BTN_Pin GPIO_PIN_0
#define WAKE_BTN_GPIO_Port GPIOA
#define OLED_SPI_SCK_Pin GPIO_PIN_1
#define OLED_SPI_SCK_GPIO_Port GPIOA
#define OLED_RST_Pin GPIO_PIN_3
#define OLED_RST_GPIO_Port GPIOA
#define OLED_SPI_CS_Pin GPIO_PIN_4
#define OLED_SPI_CS_GPIO_Port GPIOA
#define OLED_D_C_Pin GPIO_PIN_5
#define OLED_D_C_GPIO_Port GPIOA
#define OLED_VCC_EN_Pin GPIO_PIN_7
#define OLED_VCC_EN_GPIO_Port GPIOA
#define OLED_PWR_EN_Pin GPIO_PIN_0
#define OLED_PWR_EN_GPIO_Port GPIOB
#define OLED_SPI_MOSI_Pin GPIO_PIN_12
#define OLED_SPI_MOSI_GPIO_Port GPIOA
#define RTC_SPI_SCK_Pin GPIO_PIN_3
#define RTC_SPI_SCK_GPIO_Port GPIOB
#define RTC_SPI_MISO_Pin GPIO_PIN_4
#define RTC_SPI_MISO_GPIO_Port GPIOB
#define RTC_SPI_MOSI_Pin GPIO_PIN_5
#define RTC_SPI_MOSI_GPIO_Port GPIOB
#define RTC_SPI_CS_Pin GPIO_PIN_6
#define RTC_SPI_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
typedef struct
{
	uint8_t hour;
	uint8_t minute;
	uint8_t dow;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} TimeStruct;

extern char* monthNames[13];
extern char* dowNames[7];

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
