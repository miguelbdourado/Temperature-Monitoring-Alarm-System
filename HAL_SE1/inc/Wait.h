/**
 * 	@file			WAIT.h
 * 	@brief		Contains the WAIT API
 * 	@version 	1.0
 * 	@date		28 Out 2021
 * 	@author		Daniel Dron 46101, Miguel Dourado 46063, Diogo Cunha 46105
 *
 *	Copyright(C) 2021-2022, Grupo 12
 *	All rights reserved.
 *
 */


#ifndef _WAIT_H
#define _WAIT_H

#include <stdint.h>
#include "LPC17xx.h"

/**	@defgroup WAIT Wait
 *  This package provides the core capabilities for WAIT functions.
 *  @{
 */

#define SYSTICK_FREQ (SystemCoreClock / 1000)
#define TIMER0_BIT  0x2
//TCR
#define COUNTER_ON  0x1
#define COUNTER_OFF 0x0
#define RESET_ON    0x2
#define RESET_OFF   0x0
//MCR
#define MR0I		0x1
#define MR0S		0x4
//IR
#define CLEAR_MR0I	0x1

static volatile uint32_t __ms;
static volatile uint32_t __CHEGUEI;

/** @defgroup WAIT_Public_Function Wait Public Functions
 *  @{
 */

/**
 * @brief	Initialises the Wait API with 1 millisecond resolution.
 * @return	0 if initialisation succeeded; -1 if fails.
 * @note	This function must be called prior to any other WAIT functions,
 * 				and makes use of SYSTICK resource.
 */
int WAIT_init();
/**
 * @brief	Initialises the Timer 0 with 1 microsecond resolution.
 */
void WAIT_ChronosInit();
/**
 * @brief	Waits in microseconds the given amount.
 */
int WAIT_ChronoUs(uint32_t waitUs);
/**
 * @brief 	Puts the CPU in low power mode for a given amount of milliseconds.
 * @param	milliseconds: The amount of milliseconds to wait.
 */
void WAIT_Milliseconds(uint32_t milliseconds);
/**
 * @brief	Get difference in milliseconds from parameter.
 * @param	start: if 0 get current milliseconds.
 */
uint32_t WAIT_GetElapsedMillis(uint32_t start);
/**
 * 	@}
 */

/**
 * 	@}
 */
#endif
