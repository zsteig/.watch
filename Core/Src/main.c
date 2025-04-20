/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

#include "rv2123.h"
#include "ssd1331.h"
#include "font.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
enum field {hour, minute, dow, date, month};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char* monthNames[13] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}; // Empty first index to align with RTC month values
char* dowNames[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
TimeStruct globalTime =
{
		0,
		0,
		0,
		0,
		0,
		0
};

uint8_t secondsElapsed = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (secondsElapsed < 5)
		secondsElapsed++;
}

void Toggle_PA0_Function(bool wakeup)
{
	if (wakeup)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_HIGH);
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	else
	{
		HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1_HIGH);
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void Enter_Shutdown()
{
	// Enable WKUP1 Pin
	Toggle_PA0_Function(1);

	// OLED shutdown
	OLED_SendCommand(SSD1331_0xAE_Display_Off_Sleep);
	SSD1331_ClearVCC();
	HAL_Delay(100);
	SSD1331_LoadSwitchDisable();
	HAL_PWREx_EnterSHUTDOWNMode();
}

void Update_Time()
{
	RV2123_SetHour(globalTime.hour);
	RV2123_SetMinute(globalTime.minute);
	RV2123_SetDayOfWeek(globalTime.dow);
	RV2123_SetDate(globalTime.date);
	RV2123_SetMonth(globalTime.month);
}

void Increment_Value(uint8_t currentField)
{
	static char incr_val[10];

	switch(currentField)
	{
	case 0: // Hour
		if (globalTime.hour >= MIN_HOUR && globalTime.hour < MAX_HOUR)
		{
			globalTime.hour++;
		}
		else
		{
			globalTime.hour = MIN_HOUR;
		}

		sprintf(incr_val, "%02d", globalTime.hour);

		setCursorX(HOUR_UNDERLINE + 12);
		setCursorY(LINE2_Y);

		clearChar();
		clearChar();

		SSD1331_Print(incr_val, 255, 255, 255);
		SSD1331_Underline(HOUR_UNDERLINE, (LINE2_Y + 8), 2);
		break;
	case 1: // Minute
		if (globalTime.minute >= MIN_MINUTE && globalTime.minute < MAX_MINUTE)
		{
			globalTime.minute++;
		}
		else
		{
			globalTime.minute = MIN_MINUTE;
		}

		sprintf(incr_val, "%02d", globalTime.minute);

		setCursorX(MINUTE_UNDERLINE + 12);
		setCursorY(LINE2_Y);

		clearChar();
		clearChar();

		SSD1331_Print(incr_val, 255, 255, 255);
		SSD1331_Underline(MINUTE_UNDERLINE, (LINE2_Y + 8), 2);
		break;
	case 2: // Day of the Week
		if (globalTime.dow >= MIN_DOW && globalTime.dow < MAX_DOW)
		{
			globalTime.dow++;
		}
		else
		{
			globalTime.dow = MIN_DOW;
		}

		sprintf(incr_val, "%s", dowNames[globalTime.dow]);

		setCursorX(DOW_UNDERLINE + 18);
		setCursorY(LINE3_Y);

		clearChar();
		clearChar();
		clearChar();

		SSD1331_Print(incr_val, 255, 255, 255);
		SSD1331_Underline(DOW_UNDERLINE, (LINE3_Y + 8), 3);
		break;
	case 3: // Date
		if (globalTime.date >= MIN_DATE && globalTime.date < MAX_DATE)
		{
			globalTime.date++;
		}
		else
		{
			globalTime.date = MIN_DATE;
		}

		sprintf(incr_val, "%02d", globalTime.date);

		setCursorX(DATE_UNDERLINE + 12);
		setCursorY(LINE4_Y);

		clearChar();
		clearChar();

		SSD1331_Print(incr_val, 255, 255, 255);
		SSD1331_Underline(DATE_UNDERLINE, (LINE4_Y + 8), 2);
		break;
	case 4: // Month
		if (globalTime.month >= MIN_MONTH && globalTime.month < MAX_MONTH)
		{
			globalTime.month++;
		}
		else
		{
			globalTime.month = MIN_MONTH;
		}

		sprintf(incr_val, "%s", monthNames[globalTime.month]);

		setCursorX(MONTH_UNDERLINE + 18);
		setCursorY(LINE4_Y);

		clearChar();
		clearChar();
		clearChar();

		SSD1331_Print(incr_val, 255, 255, 255);
		SSD1331_Underline(MONTH_UNDERLINE, (LINE4_Y + 8), 3);
		break;
	}
}

enum field Cycle_Field(uint8_t currentValue, uint8_t currentField)
{
	enum field nextField;
	// Update globalTime value and move underline
	switch(currentField)
	{
	case 0: // Hour
//		globalTime.hour = currentValue;
		SSD1331_ClearUnderline(HOUR_UNDERLINE, (LINE2_Y + 8), 2);
		SSD1331_Underline(MINUTE_UNDERLINE, (LINE2_Y + 8), 2);
		nextField = minute;
		break;
	case 1: // Minute
//		globalTime.minute = currentValue;
		SSD1331_ClearUnderline(MINUTE_UNDERLINE, (LINE2_Y + 8), 2);
		SSD1331_Underline(DOW_UNDERLINE, (LINE3_Y + 8), 3);
		nextField = dow;
		break;
	case 2: // Day of the Week
//		globalTime.dow = currentValue;
		SSD1331_ClearUnderline(DOW_UNDERLINE, (LINE3_Y + 8), 3);
		SSD1331_Underline(DATE_UNDERLINE, (LINE4_Y + 8), 2);
		nextField = date;
		break;
	case 3: // Date
//		globalTime.date = currentValue;
		SSD1331_ClearUnderline(DATE_UNDERLINE, (LINE4_Y + 8), 2);
		SSD1331_Underline(MONTH_UNDERLINE, (LINE4_Y + 8), 3);
		nextField = month;
		break;
	case 4: // Month
//		globalTime.month = currentValue;
		SSD1331_ClearUnderline(MONTH_UNDERLINE, (LINE4_Y + 8), 3);
		SSD1331_Underline(HOUR_UNDERLINE, (LINE2_Y + 8), 2);
		nextField = hour;
		break;
	}

	return nextField;
}

void Setting_Mode_Handler()
{
	HAL_TIM_Base_Start_IT(&htim2);

	// To prevent holding buttons and causing unwanted inputs
	bool wakeButtonReleased = 0;
	bool setButtonReleased = 0;

	// State machine
	uint8_t currentValue = globalTime.hour;
	enum field currentField = hour;
	SSD1331_Underline(HOUR_UNDERLINE, (LINE2_Y + 8), 2);

	while (secondsElapsed < 5)
	{
		if (HAL_GPIO_ReadPin(WAKE_BTN_GPIO_Port, WAKE_BTN_Pin) == GPIO_PIN_SET && wakeButtonReleased)
		{
			wakeButtonReleased = 0;
			currentField = Cycle_Field(currentValue, currentField);
			HAL_Delay(200);
			secondsElapsed = 0;
		}

		if (HAL_GPIO_ReadPin(SET_BTN_GPIO_Port, SET_BTN_Pin) == GPIO_PIN_SET && setButtonReleased)
		{
			setButtonReleased = 0;
			Increment_Value(currentField);
			HAL_Delay(200);
			secondsElapsed = 0;
		}

		if (HAL_GPIO_ReadPin(WAKE_BTN_GPIO_Port, WAKE_BTN_Pin) == GPIO_PIN_RESET)
		{
			wakeButtonReleased = 1;
		}

		if (HAL_GPIO_ReadPin(SET_BTN_GPIO_Port, SET_BTN_Pin) == GPIO_PIN_RESET)
		{
			setButtonReleased = 1;
		}
	}

	drawCancel();

	// Stop Timer
	HAL_TIM_Base_Stop_IT(&htim2);

	Update_Time();

	Enter_Shutdown();
}

uint8_t Startup_Validation(uint8_t value, uint8_t min, uint8_t max, uint8_t defaultVal)
{
	if (value >= min && value <= max)
	{
		return value;
	}
	else
	{
		return defaultVal;
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

//  HAL_UART_Transmit(&huart2, (uint8_t*)"STM32 waking up!\r\n\n", 20, 100);

  RV2123_Init();
  HAL_Delay(50);

  // Checking if RTC contains valid time
  globalTime.hour = Startup_Validation(RV2123_GetHour(), MIN_HOUR, MAX_HOUR, 0);
  globalTime.minute = Startup_Validation(RV2123_GetMinute(), MIN_MINUTE, MAX_MINUTE, 0);
  RV2123_SetSecond(Startup_Validation(RV2123_GetSecond(), MIN_SECOND, MAX_SECOND, 0));
  globalTime.dow = Startup_Validation(RV2123_GetDayOfWeek(), MIN_DOW, MAX_DOW, 0);
  globalTime.date = Startup_Validation(RV2123_GetDate(), MIN_DATE, MAX_DATE, 1);
  globalTime.month = Startup_Validation(RV2123_GetMonth(), MIN_MONTH, MAX_MONTH, 0);

  // OLED Initialization
  SSD1331_LoadSwitchEnable();
  SSD1331_ClearVCC();
  SSD1331_ClearRST();
  SSD1331_ClearDC();
  SSD1331_SetCS();

  SSD1331_Init();
  HAL_Delay(50);
  SSD1331_SetVCC();
  HAL_Delay(50);

  Toggle_PA0_Function(0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  //Regular Run Loop
	  drawPrompt(LINE1_Y);
	  drawStart();
	  drawTime(globalTime.hour, globalTime.minute);
	  drawDOW(dowNames[globalTime.dow]);
	  drawDate(globalTime.date, monthNames[globalTime.month]);
	  drawPrompt(LINE5_Y);

	  for (int8_t i = 0; i < 10; i++)
	  {
		  if (i % 2)
		  {
			  clearChar();
		  }
		  else
		  {
			  SSD1331_Putc('_', 255, 255, 255);
		  }

		  uint32_t elapsedDelay = 0;

		  while (elapsedDelay < 500)
		  {
			  if (HAL_GPIO_ReadPin(SET_BTN_GPIO_Port, SET_BTN_Pin) == GPIO_PIN_SET)
			  {
				  HAL_Delay(20);
				  if (!(i % 2))
				  {
					  clearChar();
				  }

				  drawSet();
				  // Handle Setting mode
				  Setting_Mode_Handler();
			  }

			  HAL_Delay(10);
			  elapsedDelay += 10;
		  }
//		  HAL_Delay(500);
	  }

	  drawEnd();
	  Enter_Shutdown();

//		  HAL_Delay(2000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  htim2.Init.Period = 31999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, OLED_RST_Pin|OLED_SPI_CS_Pin|OLED_D_C_Pin|OLED_VCC_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OLED_PWR_EN_Pin|RTC_SPI_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : OLED_RST_Pin OLED_SPI_CS_Pin OLED_D_C_Pin OLED_VCC_EN_Pin */
  GPIO_InitStruct.Pin = OLED_RST_Pin|OLED_SPI_CS_Pin|OLED_D_C_Pin|OLED_VCC_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA8 PA10 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : OLED_PWR_EN_Pin RTC_SPI_CS_Pin */
  GPIO_InitStruct.Pin = OLED_PWR_EN_Pin|RTC_SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SET_BTN_Pin */
  GPIO_InitStruct.Pin = SET_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SET_BTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OLD_SET_BTN_Pin */
  GPIO_InitStruct.Pin = OLD_SET_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OLD_SET_BTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PH3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
