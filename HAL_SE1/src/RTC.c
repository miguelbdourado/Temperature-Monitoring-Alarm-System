/*
 * RTC.c
 *
 *  Created on: 2 Dec 2021
 *      Author: Daniel Dron
 */

#include "RTC.h"

void RTC_Init(time_t seconds)
{
	SystemInit();

	LPC_RTC->ILR = 0;
	LPC_RTC->CCR = ((1 << SBIT_CTCRST ) | (1 << SBIT_CCALEN));
	LPC_RTC->CIIR = 0;
	LPC_RTC->AMR = 0xFF;
	LPC_RTC->CALIBRATION = 0x00;
	LPC_RTC->CCR = (1 << SBIT_CLKEN);

	RTC_SetSeconds(seconds);
}

void RTC_GetValue(struct tm* dateTime)
{
	dateTime->tm_mday 	= LPC_RTC->DOM;
	dateTime->tm_mon 	= LPC_RTC->MONTH;
	dateTime->tm_year 	= LPC_RTC->YEAR;

	dateTime->tm_hour 	= LPC_RTC->HOUR;
	dateTime->tm_min 	= LPC_RTC->MIN;
	dateTime->tm_sec 	= LPC_RTC->SEC;
}

void RTC_SetValue(struct tm* dateTime)
{
	LPC_RTC->DOM    = dateTime->tm_mday;
	LPC_RTC->MONTH  = dateTime->tm_mon;
	LPC_RTC->YEAR   = dateTime->tm_year;

	LPC_RTC->HOUR   = dateTime->tm_hour;
	LPC_RTC->MIN    = dateTime->tm_min;
	LPC_RTC->SEC    = dateTime->tm_sec;
}
void RTC_SetSeconds(time_t seconds)
{
	struct tm* actual_time = localtime(&seconds);
	actual_time->tm_year += 1900;
	actual_time->tm_mon += 1;
	RTC_SetValue(actual_time);
}

time_t RTC_GetSeconds()
{
	struct tm current_time = {};
	RTC_GetValue(&current_time);
	current_time.tm_year -= 1900;
	current_time.tm_mon -= 1;

	return mktime(&current_time);
}
