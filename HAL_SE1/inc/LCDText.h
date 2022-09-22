/**
 * 	@file			LCDText.h
 * 	@brief		Contains the LCD API
 * 	@version 	1.0
 * 	@date		29 Nov 2021
 * 	@author		Daniel Dron 46101, Miguel Dourado 46063, Diogo Cunha 46105
 *
 *	Copyright(C) 2021-2022, Grupo 12
 *	All rights reserved.
 *
 */

#ifndef LCDTEXT_H_
#define LCDTEXT_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include "LPC17xx.h"
#include "Wait.h"

/**	@defgroup LCD LCDText
 *  This package provides the core capabilities for the LCD functions.
 *  @{
 */

#define MAX_ROW 2
#define MAX_COLUMN 16

#define RS_POS_ID 0x1E
#define E_POS_ID  0x1F
#define DATA_LOW_POS_ID 0x17

static volatile int __row = 0;
static volatile int __column = 0;

static bool _onStatus = true;

/** @defgroup LCD_Public_Function LCDText Public Functions
 *  @{
 */

/**
 * @brief	Initialises the LCD API with 2 lines, 16 columns and blinking cursor.
 * @note	This function must be called prior to any other LCDText functions.
 */
void LCDText_Init();
/**
 * @brief	Writes a char to the LCD screen.
 * @param	ch: Char to be written.
 * @note	The cursor will be updated automatically after the char is written.
 */
void LCDText_WriteChar(char ch);
/**
 * @brief	Writes a string of chars to the LCD screen.
 * @param	str: C-String to be written.
 * @note	The cursor will be updated automatically after each char is written.
 */
void LCDText_WriteString(char* str);
/**
 * @brief	Changes the cursor position on the LCD screen.
 * @param	row, column: new cursor position
 */
void LCDText_SetCursor(int row, int column);
/**
 * @brief	Clears the LCD screen and resets cursor position
 */
void LCDText_Clear();

/**
 * @brief 	Turns off or on the LCD screen
 * @param	status: True for on, false for off
 */
void LCDText_SetStatus(bool status);

/**
 * @brief 	Get on or off status of the LCD
 * @return 	status of LCD (on or off)
 */
bool LCDText_GetStatus();

/**
 * @brief 	Turns off or on the LCD cursor
 * @param	status: True for on, false for off
 */
void LCDText_SetCursorStatus(bool status);

void LCDText_Printf(char* fmt, ...);
/**
 * 	@}
 */

/** @defgroup LCD_Private_Function LCDText Private Functions
 *  @{
 */

/**
 * @brief	Sends 4 bits to the LCD.
 * @param	rs: Indicates whether the bits being sent are part of a command or data
 * @param	data: Bits to send to the LCD
 */
void __LCD_Write5Bits(bool rs, int8_t data);
/**
 * @brief	Sends 8 bits to the LCD.
 * @param	rs: Indicates whether the bits being sent are part of a command or data
 * @param	data: Bits to send to the LCD
 */
void __LCD_Write9Bits(bool rs, int8_t data);
/**
 * @brief	Toggles the enable signal.
 */
void PULSE_ENABLE();
/**
 * 	@}
 */

/**
 * 	@}
 */
#endif /* LCDTEXT_H_ */
