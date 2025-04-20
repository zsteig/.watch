/*
 * ssd1331.c
 *
 *  Created on: Jan 25, 2025
 *      Author: Zach
 */
#include "ssd1331.h"
#include "rv2123.h"

#include <stdio.h>

extern SPI_HandleTypeDef hspi1;


// Coordinates for OLED pixel cursor
	static uint8_t cursorX = 0;
	static uint8_t cursorY = 0;

// Strings for time display
	char* promptStr = "root~$ ";
	char* timeStr = "./t";
	char* endStr = "end";
	char* setStr = "set";
	char* cancelStr = "^C";

// Arrays for value printing
	char timeLine[20];
	char dayLine[20];
	char monthLine[20];

// *****************************************************************************
// *****************************************************************************
// Section: SSD1331 SPI Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

// Infrastructure Functions
void OLED_SendCommand(uint8_t command)
{
	SSD1331_ClearRST();
	SSD1331_ClearDC();
	SSD1331_ClearCS();

//	HAL_SPI_Transmit(&hspi1, &command, sizeof(command), 100);
	HAL_SPI_Transmit_IT(&hspi1, &command, sizeof(command));

	SSD1331_SetCS();
}

void OLED_SendData(uint8_t data)
{
	SSD1331_ClearRST();
	SSD1331_SetDC();
	SSD1331_ClearCS();

//	HAL_SPI_Transmit(&hspi1, &data, sizeof(data), 100);
	HAL_SPI_Transmit_IT(&hspi1, &data, sizeof(data));

	SSD1331_SetCS();
}

void setCursorX(uint8_t x)
{
	cursorX = x;
}

void setCursorY(uint8_t y)
{
	cursorY = y;
}

uint8_t getCursorX()
{
	return cursorX;
}

uint8_t getCursorY()
{
	return cursorY;
}

void SSD1331_Init()
{
	SSD1331_ClearRST();
	HAL_Delay(1);
	SSD1331_SetRST();
	HAL_Delay(100);

	OLED_SendCommand(SSD1331_0xFD_Lock_Unlock);
	OLED_SendCommand(SSD1331_0x12_Param_Unlock);

	OLED_SendCommand(SSD1331_0xAE_Display_Off_Sleep);
	OLED_SendCommand(SSD1331_0xA4_Mode_Normal);

	OLED_SendCommand(SSD1331_0x15_Column_Address);
	OLED_SendCommand(0);
	OLED_SendCommand(95);

	OLED_SendCommand(SSD1331_0x75_Row_Address);
	OLED_SendCommand(0);
	OLED_SendCommand(63);

	OLED_SendCommand(SSD1331_0x87_Master_Current);
	OLED_SendCommand(9);

	OLED_SendCommand(SSD1331_0x81_Contrast_A_Blue);
	OLED_SendCommand(102);
	OLED_SendCommand(SSD1331_0x82_Contrast_B_Green);
	OLED_SendCommand(59);
	OLED_SendCommand(SSD1331_0x83_Contrast_C_Red);
	OLED_SendCommand(199);

	OLED_SendCommand(SSD1331_0x8A_Second_Precharge);
	OLED_SendCommand(0x61);
	OLED_SendCommand(0x8B);
	OLED_SendCommand(0x62);
	OLED_SendCommand(0x8C);
	OLED_SendCommand(0x63);

	OLED_SendCommand(SSD1331_0xA0_Remap_Data_Format);
	OLED_SendCommand(0x71);

	OLED_SendCommand(SSD1331_0xA1_Start_Line);
	OLED_SendCommand(0);

	OLED_SendCommand(SSD1331_0xA2_Vertical_Offset);
	OLED_SendCommand(0);

	OLED_SendCommand(SSD1331_0xA8_Multiplex_Ratio);
	OLED_SendCommand(63);

	OLED_SendCommand(SSD1331_0xAD_Master_Configuration);
	OLED_SendCommand(SSD1331_0x8F_Param_Not_External_VCC);

	OLED_SendCommand(SSD1331_0xB0_Power_Save_Mode);
	OLED_SendCommand(SSD1331_0x1A_Param_Yes_Power_Save);

	OLED_SendCommand(SSD1331_0xB1_Phase_1_2_Period);
	OLED_SendCommand(0xF1);

	OLED_SendCommand(SSD1331_0xB3_Clock_Divide_Frequency);
	OLED_SendCommand(0xD0);

	OLED_SendCommand(SSD1331_0xBB_Precharge_Voltage);
	OLED_SendCommand(0x3E);

	OLED_SendCommand(SSD1331_0xBE_VCOMH_Voltage);
	OLED_SendCommand(0x30);

	OLED_SendCommand(SSD1331_0xB9_Linear_Gamma);

	OLED_SendCommand(SSD1331_0xAD_Master_Configuration);
	OLED_SendCommand(SSD1331_0x8E_Param_Set_External_VCC);

	OLED_SendCommand(SSD1331_0x26_Fill_Copy_Options);
	OLED_SendCommand(0x01);

	OLED_SendCommand(SSD1331_0x25_Clear_Window);
	OLED_SendCommand(0);
	OLED_SendCommand(0);
	OLED_SendCommand(95);
	OLED_SendCommand(63);

	HAL_Delay(1);

	OLED_SendCommand(SSD1331_0xAF_Display_On_Normal);
}

void putPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
	OLED_SendCommand(SSD1331_0x21_Draw_Line);
	OLED_SendCommand(y);
	OLED_SendCommand(x);
	OLED_SendCommand(y);
	OLED_SendCommand(x);

	OLED_SendCommand(r >> 2);
	OLED_SendCommand(g >> 2);
	OLED_SendCommand(b >> 2);
}

void SSD1331_Putc(char ch, uint8_t r, uint8_t g, uint8_t b)
{
	FontDef_t* Font = &Font_5x7;

	for (int8_t i = 0; i < 5; ++i)
	{
		uint8_t line = Font->data[ch * 5 + i];
		for (int8_t j = 0; j < 7; ++j, line >>= 1)
		{
			if (line & 1)
			{
				putPixel(cursorX + i, cursorY + j, r, g, b);
			}
			else
			{
				putPixel(cursorX + i, cursorY + j, 0, 0, 0);
			}
		}
	}
	for (int8_t j = 0; j < 8; ++j)
	{
		putPixel(cursorX + 5, cursorY + 5, 0, 0, 0);
	}

	setCursorX(cursorX + Font->FontWidth + 1);
}

void SSD1331_Print(char* str, uint8_t r, uint8_t g, uint8_t b)
{
	while (*str)
	{
		SSD1331_Putc(*str, r, g, b);
		str++;
	}
}

void SSD1331_Underline(uint8_t x, uint8_t y, uint8_t chars)
{
	  OLED_SendCommand(SSD1331_0x21_Draw_Line);
	  OLED_SendCommand(y);
	  OLED_SendCommand(x);

	  OLED_SendCommand(y);
	  uint8_t charAdjust = (chars > 2) ? (chars - 2) : 0;
	  OLED_SendCommand(x + (chars * 5) + charAdjust);

	  OLED_SendCommand(255 >> 2);
	  OLED_SendCommand(255 >> 2);
	  OLED_SendCommand(255 >> 2);
}

void SSD1331_ClearUnderline(uint8_t x, uint8_t y, uint8_t chars)
{
	  OLED_SendCommand(SSD1331_0x21_Draw_Line);
	  OLED_SendCommand(y);
	  OLED_SendCommand(x);

	  OLED_SendCommand(y);
	  uint8_t charAdjust = (chars > 2) ? (chars - 2) : 0;
	  OLED_SendCommand(x + (chars * 5) + charAdjust);

	  OLED_SendCommand(0 >> 2);
	  OLED_SendCommand(0 >> 2);
	  OLED_SendCommand(0 >> 2);
}

void clearChar()
{
	  OLED_SendCommand(SSD1331_0x22_Draw_Rectangle);
	  OLED_SendCommand(cursorY);
	  OLED_SendCommand(cursorX - 6);

	  OLED_SendCommand(cursorY + 8);
	  OLED_SendCommand(cursorX);

	  OLED_SendCommand(0 >> 2);
	  OLED_SendCommand(0 >> 2);
	  OLED_SendCommand(0 >> 2);

	  OLED_SendCommand(0 >> 2);
	  OLED_SendCommand(0 >> 2);
	  OLED_SendCommand(0 >> 2);

	  setCursorX(cursorX - 6);
}

void drawPrompt(int y)
{
	setCursorX(0);
	setCursorY(y);
//	SSD1331_Print(promptStr, 0, 255, 0); 	// Green
	SSD1331_Print(promptStr, 165, 0, 115); 	// Purple
}

void drawStart()
{
	SSD1331_Print("_", 255, 255, 255);
	HAL_Delay(300);

	clearChar();

	for (int8_t i = 0; timeStr[i] != '\0'; i++)
	{
		char currChar = timeStr[i];
		SSD1331_Putc(currChar, 255, 255, 255);
		HAL_Delay(50);
	}

	HAL_Delay(250);
}

void drawTime(uint8_t hour, uint8_t minute)
{
	sprintf(timeLine, "> %02d:%02d", hour, minute);

	setCursorX(0);
	setCursorY(LINE2_Y);
	HAL_Delay(100);

	for (int8_t i = 0; timeLine[i] != '\0'; i++)
	{
		char currChar = timeLine[i];
		SSD1331_Putc(currChar, 255, 255, 255);
		HAL_Delay(25);
	}
}

void drawDOW(char* dow)
{
	sprintf(dayLine, "> %s", dow);

	setCursorX(0);
	setCursorY(LINE3_Y);
	HAL_Delay(100);

	for (int8_t i = 0; dayLine[i] != '\0'; i++)
	{
		char currChar = dayLine[i];
		SSD1331_Putc(currChar, 255, 255, 255);
		HAL_Delay(25);
	}
}

void drawDate(uint8_t date, char* month)
{
	sprintf(monthLine, "> %02d %s", date, month);

	setCursorX(0);
	setCursorY(LINE4_Y);
	HAL_Delay(100);

	for (int8_t i = 0; monthLine[i] != '\0'; i++)
	{
		char currChar = monthLine[i];
		SSD1331_Putc(currChar, 255, 255, 255);
		HAL_Delay(25);
	}
}

void drawEnd()
{
	setCursorX(45);
	setCursorY(LINE5_Y);

	for (int8_t i = 0; endStr[i] != '\0'; i++)
	{
		char currChar = endStr[i];
		SSD1331_Putc(currChar, 255, 255, 255);
		HAL_Delay(50);
	}

	HAL_Delay(500);
}

void drawSet()
{
	setCursorX(45);
	setCursorY(LINE5_Y);

	for (int8_t i = 0; setStr[i] != '\0'; i++)
	{
		char currChar = setStr[i];
		SSD1331_Putc(currChar, 255, 255, 255);
		HAL_Delay(50);
	}

	HAL_Delay(250);
}

void drawCancel()
{
	setCursorX(0);
	setCursorY(LINE6_Y);

	for (int8_t i = 0; cancelStr[i] != '\0'; i++)
	{
		char currChar = cancelStr[i];
		SSD1331_Putc(currChar, 255, 255, 255);
		HAL_Delay(50);
	}

	HAL_Delay(250);
}

void SSD1331_SetDC()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

void SSD1331_ClearDC()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}

void SSD1331_SetCS()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void SSD1331_ClearCS()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void SSD1331_SetRST()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
}

void SSD1331_ClearRST()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
}

void SSD1331_SetVCC()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}

void SSD1331_ClearVCC()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}

void SSD1331_LoadSwitchEnable()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

void SSD1331_LoadSwitchDisable()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}
