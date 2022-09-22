/*
===============================================================================
 Name        : LAB_2.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

// TODO: insert other include files here
#include "LCDText.h"

#include "LED.h"
#include "Wait.h"
#include "KEYPAD.h"
#include "RTC.h"
// TODO: insert other definitions and declarations here

#define NO_KEY -2
int layoutTest[] = {0x30, 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , NO_KEY, 0x37 , 0x38 , 0x39 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 };
int lastPressedKey = 0x0;

int main(void)
{
	LCDText_Init();
	KEYPAD_Init(layoutTest);
    LED_Init(false);
    WAIT_ChronosInit();
    WAIT_init();

    //time(NULL) -> epoch
    RTC_Init(1638481117);

    while (1)
	{
		if(changeTimeKeyPressed) enterCalibrationMode = true;

		if(enterCalibrationMode)
		{
			if(changeTimeKeyPressed) { enterCalibrationMode = false; continue; }

			LCDText_SetCursor(0, HOUR_COLUMN);

			if(upPressed) incrementNumberAt(HOUR_COLUMN);
			...etc;
		} else {
			/* Read Time */
			uint8_t hour = LPC_RTC->HOUR;
			uint8_t min  = LPC_RTC->MIN;
			uint8_t sec  = LPC_RTC->SEC;

			/* Read Date */
			uint8_t date  = LPC_RTC->DOM;
			uint8_t month = LPC_RTC->MONTH;
			uint16_t year  = LPC_RTC->YEAR;

			/* Display date and time on LCD */
			LCDText_SetCursor(0, 0);
			LCDText_Printf("Time: %02hu:%02hu:%02hu", hour, min, sec);
			LCDText_SetCursor(1, 0);
			LCDText_Printf("Date: %02hu:%02hu:%04hu", date, month, year);
		}
	}
    return 0 ;
}
