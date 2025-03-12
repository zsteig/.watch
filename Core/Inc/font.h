/*
 * font.h
 *
 *  Created on: Feb 8, 2025
 *      Author: Zach
 */

#ifndef INC_FONT_H_
#define INC_FONT_H_

#include "stm32l4xx_hal.h"
#include "string.h"

typedef struct
{
	uint8_t FontWidth;
	uint8_t FontHeight;
	uint8_t CharBytes;
	const uint8_t* data;
} FontDef_t;

typedef  struct
{
	uint16_t Length;
	uint16_t Height;
} FONTS_SIZE_t;

extern FontDef_t Font_5x7;

char* FONTS_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* font);

#endif /* INC_FONT_H_ */
