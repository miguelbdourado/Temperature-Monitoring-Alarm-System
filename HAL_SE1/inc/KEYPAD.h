/**
 * 	@file			KEYPAD.h
 * 	@brief		Contains the KEYPAD API
 * 	@version 	1.0
 * 	@date		29 Nov 2021
 * 	@author		Daniel Dron 46101, Miguel Dourado 46063, Diogo Cunha 46105
 *
 *	Copyright(C) 2021-2022, Grupo 12
 *	All rights reserved.
 *
 */


#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "LPC17xx.h"

/**	@defgroup KEYPAD KeyPad
 *  This package provides the core capabilities for KEYPAD functions.
 *  @{
 */

static volatile int __layout[16];
static volatile uint16_t lastTickKeys = 0x0;

/** @defgroup KEYPAD_Public_Function KeyPad Public Functions
 *  @{
 */

/**
 * @brief	Initialises the KeyPad API.
 * @return	0
 * @note	This function must be called prior to any other KEYPAD function.
 */
int KEYPAD_Init(int layout[16]);

/**
 * @brief Returns true or false whether a key is being pressed at the moment
 * @return True if a key is being pressed. False otherwise.
 */
bool KEYPAD_Hit();

/**
 * @brief Returns the code of the key being pressed.
 * @return Key code being pressed. -1 If no key is being pressed.
 */
uint16_t KEYPAD_Read();

/**
 * @brief 	Returns whether the given key is being pressed or not.
 * @param	keyCode: code key from layout
 * @return 	True or false.
 */
bool isKeyPressed(int keyCode);

/**
 * @brief 	Returns whether the given key started being pressed at this moment.
 * @param	keyCode: code key from layout
 * @return 	True or false.
 */
bool isKeyDown(int keyCode);

/**
 * @brief 	Returns whether the given key stopped being pressed at this moment.
 * @param	keyCode: code key from layout
 * @return 	True or false.
 */
bool isKeyUp(int keyCode);

void updateKeys();


/**
 * 	@}
 */

/**
 * 	@}
 */
