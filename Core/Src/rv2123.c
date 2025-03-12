/*
 * rv2123.c
 *
 *  Created on: Jan 25, 2025
 *      Author: Zach
 */
#include "rv2123.h"

extern SPI_HandleTypeDef hspi3;

// *****************************************************************************
// *****************************************************************************
// Section: RTC SPI Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

// Infrastructure Functions
void RV2123_Write(uint8_t address, uint8_t data)
{
	RV2123_SetCS();

	uint8_t txBuffer[2] = {(address & 0x7F) | 0x10, data};

	HAL_SPI_Transmit(&hspi3, txBuffer, sizeof(txBuffer), 1000);

	RV2123_ClearCS();
}

uint8_t RV2123_Read(uint8_t address)
{
	uint8_t txBuffer[2];
	uint8_t rxBuffer[2];
	uint8_t cmd = (0x90 | (address & 0x0F));

	txBuffer[0] = cmd;
	txBuffer[1] = 0x00; // Dummy byte to clock in the response

	RV2123_SetCS();

	if (HAL_SPI_TransmitReceive(&hspi3, txBuffer, rxBuffer, sizeof(txBuffer), 1000) != HAL_OK)
	{
		RV2123_ClearCS();
		return 7;
	}

	RV2123_ClearCS();
	return rxBuffer[1];
}

uint8_t RV2123_Init()
{
	RV2123_SetCS();

	RV2123_Write(0x10, RV2123_SR_SEQ);

	RV2123_ClearCS();

	return 0;
}

// Getting Functions
uint8_t RV2123_GetDayOfWeek()
{
    uint8_t value = RV2123_Read(RV2123_REG_DOW);
    return RV2123_DecodeBCD(value & 0x07);
}

uint8_t RV2123_GetDate()
{
    uint8_t value = RV2123_Read(RV2123_REG_DATE);
    return RV2123_DecodeBCD(value & 0x3F);
}

uint8_t RV2123_GetMonth()
{
    uint8_t value = RV2123_Read(RV2123_REG_MONTH);
    return RV2123_DecodeBCD(value & 0x1F);
}

uint8_t RV2123_GetYear()
{
    uint8_t value = RV2123_Read(RV2123_REG_YEAR);
    return RV2123_DecodeBCD(value);
}

uint8_t RV2123_GetHour()
{
    uint8_t value = RV2123_Read(RV2123_REG_HOUR);
    return RV2123_DecodeBCD(value & 0x3F);
}

uint8_t RV2123_GetMinute()
{
    uint8_t value = RV2123_Read(RV2123_REG_MINUTE);
    return RV2123_DecodeBCD(value & 0x7F);
}

uint8_t RV2123_GetSecond()
{
    uint8_t value = RV2123_Read(RV2123_REG_SECOND);
    return RV2123_DecodeBCD(value & 0x7F);
}

// Setting Functions
void RV2123_SetDayOfWeek(uint8_t dow)
{
    RV2123_Write(RV2123_REG_DOW, RV2123_EncodeBCD(dow));
}

void RV2123_SetDate(uint8_t date)
{
    RV2123_Write(RV2123_REG_DATE, RV2123_EncodeBCD(date));
}

void RV2123_SetMonth(uint8_t month)
{
    RV2123_Write(RV2123_REG_MONTH, RV2123_EncodeBCD(month));
}

void RV2123_SetYear(uint8_t year)
{
    RV2123_Write(RV2123_REG_YEAR, RV2123_EncodeBCD(year));
}

void RV2123_SetHour(uint8_t hour)
{
    RV2123_Write(RV2123_REG_HOUR, RV2123_EncodeBCD(hour));
}

void RV2123_SetMinute(uint8_t minute)
{
    RV2123_Write(RV2123_REG_MINUTE, RV2123_EncodeBCD(minute));
}

void RV2123_SetSecond(uint8_t second)
{
    RV2123_Write(RV2123_REG_SECOND, RV2123_EncodeBCD(second));
}

// Pin Functions
void RV2123_SetCS()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
}

void RV2123_ClearCS()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
}

// Conversion Functions
uint8_t RV2123_DecodeBCD(uint8_t bin)
{
	return (((bin & 0xF0) >> 4) * 10) + (bin & 0x0F);
}

uint8_t RV2123_EncodeBCD(uint8_t dec)
{
	return (dec % 10 + ((dec / 10) << 4));
}
