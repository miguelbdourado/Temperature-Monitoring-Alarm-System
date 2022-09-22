/*
 * LCDText.c
 *
 *  Created on: 25 Nov 2021
 *      Author: Daniel Dron
 */
#include "LCDText.h"

/*
 * P0[23..26] -> DB4-DB7 -> 11-14
 * P1[30..31] -> RS/E	  -> 4/6
 */
void LCDText_Init()
{
	//Init chrono
	WAIT_ChronosInit();

	//Set GPIO function for pins P0[23..26] and P1[30..31]
	LPC_PINCON->PINSEL1 &= ~(0x3FC000);
	LPC_PINCON->PINSEL3 &= ~(0xF0000000);

	//Set output mode for pins P0[23..26] and P1[30..31]
	LPC_GPIO0->FIODIR |= (0x7800000);
	LPC_GPIO1->FIODIR |= (0xC0000000);

	__row    = 0;
	__column = 0;

	//Wait more than 15ms
	WAIT_ChronoUs(20 * 1000);	//20 ms

	//Step 1
	__LCD_Write5Bits(0, 0x3);

	//Wait more than 5ms
	WAIT_ChronoUs(10 * 1000);	//10ms

	//Step 2
	__LCD_Write5Bits(0, 0x3);

	//Wait more than 100 nanoseconds
	WAIT_ChronoUs(1);

	//Step 3
	__LCD_Write5Bits(0, 0x3);

	//Step 4
	__LCD_Write5Bits(0, 0x2);
	__LCD_Write9Bits(0, 0x28);//2 lines and 5x8 dots
	__LCD_Write9Bits(0, 0x8);
	__LCD_Write9Bits(0, 0x1); //Clear display
	__LCD_Write9Bits(0, 0x6); //Increments cursor on writes

	WAIT_ChronoUs(10 * 1000);	//10ms
	__LCD_Write9Bits(0, 0xF); //Turn on display, turn on blinking cursor
}

void LCDText_SetCursor(int row, int column) {
	int new_row = 0, new_column = 0;

	//Clamp row
	if(row < 0) new_row = 0;
	else if(row >= MAX_ROW) new_row = MAX_ROW - 1;
	else new_row = row;

	//Clamp column
	if(column < 0) new_column = 0;
	else if(column >= MAX_COLUMN) new_column = MAX_COLUMN - 1;
	else new_column = column;

	int new_cursor_pos = (new_row * 40) + new_column;
	int old_cursor_pos = (__row * 40) + __column;

	int delta_cursor_pos = new_cursor_pos - old_cursor_pos;

	int8_t data = 0x10;						//Shift to the left
	if(delta_cursor_pos > 0) data |= 0x4;	//Shift to the right

	for(int i = 0; i < abs(delta_cursor_pos); i++)
	{
		__LCD_Write9Bits(0, data);
		WAIT_ChronoUs(1);
	}

	__column = new_column;
	__row = new_row;
}

void PULSE_ENABLE()
{
	//Wait for tAS
	WAIT_ChronoUs(10);

	//Enable
	LPC_GPIO1->FIOSET = (0x1 << E_POS_ID);

	//Wait for PW_EH
	WAIT_ChronoUs(10);

	//Disable
	LPC_GPIO1->FIOCLR = (0x1 << E_POS_ID);

	//Wait for tAH
	WAIT_ChronoUs(10);
}

void __LCD_Write9Bits(bool rs, int8_t data)
{
	__LCD_Write5Bits(rs, data >> 4);
	__LCD_Write5Bits(rs, data);
}

void __LCD_Write5Bits(bool rs, int8_t data)
{
	if(rs) LPC_GPIO1->FIOSET = (1 << RS_POS_ID);
	else   LPC_GPIO1->FIOCLR = (1 << RS_POS_ID);

	LPC_GPIO0->FIOCLR = (0xF << DATA_LOW_POS_ID);
	LPC_GPIO0->FIOSET = ((data & 0xF) << DATA_LOW_POS_ID);
	PULSE_ENABLE();
}

void LCDText_WriteChar(char ch)
{
	__LCD_Write9Bits(1, ch);
	__column++;

	//If we are writing to a column outside the screen range, update row and column on the LCD
	if(__column >= MAX_COLUMN)
	{
		int new_row = 0, new_column = 0;

		if(__row == MAX_ROW - 1) new_row = 0;
		else new_row = 1;

		new_column = 0;

		LCDText_SetCursor(new_row, new_column);
	}
}

void LCDText_Printf(char* fmt, ...)
{
	char str[32];
	va_list args;
	va_start( args, fmt );
	vsprintf(str, fmt, args);
	va_end( args );
	LCDText_WriteString(str);
}

void LCDText_WriteString(char* str){
	for(int i = 0; i < strlen(str); i++)
		LCDText_WriteChar(str[i]);
}

bool LCDText_GetStatus()
{
	return _onStatus;
}

void LCDText_SetStatus(bool status)
{
	_onStatus = status;
	__LCD_Write9Bits(0, 0x8 | status * 0x7);
}

void LCDText_Clear()
{
	__LCD_Write9Bits(0, 0x1); //Clear display
	__column = 0; __row = 0;

	WAIT_ChronoUs(10000);	//TODO: bung
}


void LCDText_SetCursorStatus(bool status)
{
	_onStatus = status;
	__LCD_Write9Bits(0, 0x8 | _onStatus * 0x4 | status * 0x3);
}
