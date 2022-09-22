/**
 * 	@file			LED.h
 * 	@brief		Contains the LED2 API
 * 	@version 	1.0
 * 	@date		28 Out 2021
 * 	@author		Daniel Dron 46101, Miguel Dourado 46063, Diogo Cunha 46105
 *
 *	Copyright(C) 2021-2022, Grupo 12
 *	All rights reserved.
 *
 */

#ifndef LED_H_
#define LED_H_

#include <stdbool.h>
#include "LPC17xx.h"

/**	@defgroup LED LED
 *  This package provides the core capabilities for LED2 functions.
 *  @{
 */

static volatile bool __curr_state;


/** @defgroup LED_Public_Function LED2 Public Functions
 *  @{
 */

/**
 * @brief	Initialises the LED2 with a given state.
 * @param	state: Indicates the initial state of the LED.
 * @note	This function must be called prior to any other LED2 functions.
 */
void LED_Init(bool state);
/**
 * @brief	Get the current LED2 state.
 * @return	the current LED2 state.
 */
bool LED_GetState();
/**
 * @brief	Turns the LED2 on.
 */
void LED_On();
/**
 * @brief	Turns the LED2 off.
 */
void LED_Off();
/**
 * @brief	Toggles the LED2 on or off depending on the current state.
 */
void LED_Toggle();

/**
 * 	@}
 */

/**
 * 	@}
 */

#endif
