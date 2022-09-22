/**
 * 	@file			RTC.h
 * 	@brief		Contains the RTC API
 * 	@version 	1.0
 * 	@date		2 Dez 2021
 * 	@author		Daniel Dron 46101, Miguel Dourado 46063, Diogo Cunha 46105
 *
 *	Copyright(C) 2021-2022, Grupo 12
 *	All rights reserved.
 *
 */
#ifndef RTC_H_
#define RTC_H_

#include "LPC17xx.h"
#include <time.h>

/**	@defgroup RTC RTC
 *  This package provides the core capabilities for RTC functions.
 *  @{
 */

#define SBIT_CLKEN     0    /* RTC Clock Enable*/
#define SBIT_CTCRST    1    /* RTC Clock Reset */
#define SBIT_CCALEN    4    /* RTC Calibration counter enable */

/** @defgroup RTC_Public_Function RTC Public Functions
 *  @{
 */

/**
 * @brief	Initialize RTC API
 * @param	seconds: time in epoch
 * @note	Call this before any other RTC API functions
 */
void RTC_Init(time_t seconds);

/**
 * @brief	Get the current time from the RTC
 * @param 	dateTime: current time from the RTC
 */
void RTC_GetValue(struct tm* dateTime);

/**
 * @brief	Set the current RTC time
 * @param	dateTime: the time to set the RTC to
 */
void RTC_SetValue(struct tm* dateTime);

/**
 * @brief 	Set the current RTC time according to the amount of given seconds since 00:00:00 UTC, January 1st 1970.
 * @param 	seconds: amount of seconds since 00:00:00 UTC, January 1st 1970
 */
void RTC_SetSeconds(time_t seconds);

/**
 * @brief 	Get the current RTC time in seconds since 00:00:00 UTC, January 1st 1970.
 * @return	amount of seconds since 00:00:00 UTC, January 1st 1970
 */
time_t RTC_GetSeconds();

/**
 * 	@}
 */

/**
 * 	@}
 */

#endif /* RTC_H_ */
