/*
 * ssd1331.h
 *
 *  Created on: Jan 25, 2025
 *      Author: Zach
 */

#ifndef INC_SSD1331_H_
#define INC_SSD1331_H_

#include <stdbool.h>

#include "stm32l4xx_hal.h"

#include "font.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */

// Defines for the SSD1331 registers.
// ref: https://www.crystalfontz.com/controllers/SolomonSystech/SSD1331/
#define SSD1331_0x15_Column_Address         (0x15)
#define SSD1331_0x21_Draw_Line              (0x21)
#define SSD1331_0x22_Draw_Rectangle         (0x22)
#define SSD1331_0x23_Copy                   (0x23)
#define SSD1331_0x24_Dim_Window             (0x24)
#define SSD1331_0x25_Clear_Window           (0x25)
#define SSD1331_0x26_Fill_Copy_Options      (0x26)
#define SSD1331_0x27_Scrolling_Options      (0x27)
#define SSD1331_0x2E_Scrolling_Stop         (0x2E)
#define SSD1331_0x2F_Scrolling_Start        (0x2F)
#define SSD1331_0x75_Row_Address            (0x75)
#define SSD1331_0x81_Contrast_A_Blue        (0x81)
#define SSD1331_0x82_Contrast_B_Green       (0x82)
#define SSD1331_0x83_Contrast_C_Red         (0x83)
#define SSD1331_0x87_Master_Current         (0x87)
#define SSD1331_0x8A_Second_Precharge       (0x8A)
#define SSD1331_0xA0_Remap_Data_Format      (0xA0)
#define SSD1331_0xA1_Start_Line             (0xA1)
#define SSD1331_0xA2_Vertical_Offset        (0xA2)
#define SSD1331_0xA4_Mode_Normal            (0xA4)
#define SSD1331_0xA5_Mode_All_On            (0xA5)
#define SSD1331_0xA6_Mode_All_Off           (0xA6)
#define SSD1331_0xA7_Mode_Inverse           (0xA7)
#define SSD1331_0xA8_Multiplex_Ratio        (0xA8)
#define SSD1331_0xAB_Dim_Mode_Setting       (0xAB)
#define SSD1331_0xAD_Master_Configuration   (0xAD)
#define SSD1331_0x8F_Param_Not_External_VCC (0x8F)
#define SSD1331_0x8E_Param_Set_External_VCC (0x8E)
#define SSD1331_0xAC_Display_On_Dim         (0xAC)
#define SSD1331_0xAE_Display_Off_Sleep      (0xAE)
#define SSD1331_0xAF_Display_On_Normal      (0xAF)
#define SSD1331_0xB0_Power_Save_Mode        (0xB0)
#define SSD1331_0x1A_Param_Yes_Power_Save   (0x1A)
#define SSD1331_0x0B_Param_No_Power_Save    (0x0B)
#define SSD1331_0xB1_Phase_1_2_Period       (0xB1)
#define SSD1331_0xB3_Clock_Divide_Frequency (0xB3)
#define SSD1331_0xB8_Gamma_Table            (0xB8)
#define SSD1331_0xB9_Linear_Gamma           (0xB9)
#define SSD1331_0xBB_Precharge_Voltage      (0xBB)
#define SSD1331_0xBE_VCOMH_Voltage          (0xBE)
#define SSD1331_0xFD_Lock_Unlock            (0xFD)
#define SSD1331_0x12_Param_Unlock           (0x12)
#define SSD1331_0x16_Param_Lock             (0x16)

// Y coordinates for each line
#define LINE1_Y 0
#define LINE2_Y 16
#define LINE3_Y 32
#define LINE4_Y 48
#define LINE5_Y 64
#define LINE6_Y 80

// Underline X coordinates
#define HOUR_UNDERLINE 		18
#define MINUTE_UNDERLINE	36
#define DOW_UNDERLINE		18
#define DATE_UNDERLINE		18
#define MONTH_UNDERLINE		36

// Strings for time display
	extern char* promptStr;
	extern char* timeStr;
	extern char* endStr;
	extern char* setStr;
	extern char* cancelStr;

// Arrays for value printing
	extern char timeLine[20];
	extern char dayLine[20];
	extern char monthLine[20];

// *****************************************************************************
// *****************************************************************************
// Section: OLED SPI Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

void OLED_SendCommand(uint8_t command);
void OLED_SendData(uint8_t data);

void setCursorX(uint8_t x);
void setCursorY(uint8_t y);
uint8_t getCursorX(void);
uint8_t getCursorY(void);

void SSD1331_Init(void);

void putPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
void SSD1331_Putc(char ch, uint8_t r, uint8_t g, uint8_t b);
void SSD1331_Print(char* str, uint8_t r, uint8_t g, uint8_t b);

void SSD1331_Underline(uint8_t x, uint8_t y, uint8_t chars);
void SSD1331_ClearUnderline(uint8_t x, uint8_t y, uint8_t chars);

void clearChar(void);
void drawPrompt(int y);
void drawStart(void);
void drawTime(uint8_t hour, uint8_t minute);
void drawDOW(char* dow);
void drawDate(uint8_t date, char* month);
void drawEnd(void);
void drawSet(void);
void drawCancel(void);
//TODO: Need to add setting functions

void SSD1331_SetDC(void);
void SSD1331_ClearDC(void);
void SSD1331_SetCS(void);
void SSD1331_ClearCS(void);
void SSD1331_SetRST(void);
void SSD1331_ClearRST(void);
void SSD1331_SetVCC(void);
void SSD1331_ClearVCC(void);
void SSD1331_LoadSwitchEnable(void);
void SSD1331_LoadSwitchDisable(void);

#endif /* INC_SSD1331_H_ */
