/*
 * rv2123.h
 *
 *  Created on: Jan 25, 2025
 *      Author: Zach
 */

#ifndef INC_RV2123_H_
#define INC_RV2123_H_

#include <stdint.h>

#include "stm32l4xx_hal.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */

#define RV2123_REG_SECOND 	0x02
#define RV2123_REG_MINUTE 	0x03
#define RV2123_REG_HOUR  	0x04
#define RV2123_REG_DOW    	0x06

#define RV2123_REG_DATE   	0x05
#define RV2123_REG_MONTH  	0x07
#define RV2123_CENTURY 		0x07
#define RV2123_REG_YEAR   	0x08

#define RV2123_SR_SEQ       0x58

#define MIN_HOUR 			0
#define MAX_HOUR 			23
#define MIN_MINUTE 			0
#define MAX_MINUTE 			59
#define MIN_SECOND 			0
#define MAX_SECOND 			59
#define MIN_DOW 			0
#define MAX_DOW 			6
#define MIN_DATE 			1
#define MAX_DATE 			31
#define MIN_MONTH 			1
#define MAX_MONTH			12

// *****************************************************************************
// *****************************************************************************
// Section: RTC SPI Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

// Infrastructure Functions
void RV2123_Write(uint8_t address, uint8_t data);
uint8_t RV2123_Read(uint8_t address);
uint8_t RV2123_Init(void);

// Getting Functions
uint8_t RV2123_GetDayOfWeek(void);
uint8_t RV2123_GetDate(void);
uint8_t RV2123_GetMonth(void);
uint8_t RV2123_GetYear(void);
uint8_t RV2123_GetHour(void);
uint8_t RV2123_GetMinute(void);
uint8_t RV2123_GetSecond(void);

// Setting Functions
void RV2123_SetDayOfWeek(uint8_t);
void RV2123_SetDate(uint8_t);
void RV2123_SetMonth(uint8_t);
void RV2123_SetYear(uint8_t);
void RV2123_SetHour(uint8_t);
void RV2123_SetMinute(uint8_t);
void RV2123_SetSecond(uint8_t);

// Pin Functions
void RV2123_SetCS(void);
void RV2123_ClearCS(void);

// Conversion Functions
uint8_t RV2123_DecodeBCD(uint8_t bin);
uint8_t RV2123_EncodeBCD(uint8_t dec);

#endif /* INC_RV2123_H_ */
