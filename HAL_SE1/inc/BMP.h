/**
 * 	@file			BMP.h
 * 	@brief		Contains the BMP API
 * 	@version 	1.0
 * 	@date		20 Jan 2022
 * 	@author		Daniel Dron 46101, Miguel Dourado 46063, Diogo Cunha 46105
 *
 *	Copyright(C) 2021-2022, Grupo 12
 *	All rights reserved.
 *
 */

#ifndef BMP_H_
#define BMP_H_

#include "SPI.h"

/**	@defgroup BMP BMP
 *  This package provides the core capabilities for BMP functions.
 *  @{
 */

/** @defgroup BMP_Public_Function BMP Public Functions
 *  @{
 */

/**
 * @brief 	Initialises communication with the BMP sensor
 * @note	This function <b>must</b> be called prior to any other BMP functions
 */
void BMP_Init();

/**
 * @brief 	Sets a register in the BMP sensor with the given value
 * @param	registerName: the register to write the data to
 * @param 	value: the value to write to the register
 * @return	Status
 */
int SetReg8(unsigned short registerName, unsigned short value);

/**
 * @brief 	Get the value of a 16bit register from the BMP sensor
 * @param 	registerName: the register to get the value from
 */
unsigned short GetReg16(unsigned short registerName);

/**
 * @brief 	Get the value of a 8bit register from the BMP sensor
 * @param 	registerName: the register to get the value from
 */
unsigned char GetReg8(unsigned char registerName);

/**
 * @brief 	Get the current temperature in Celsius
 * @return	the current temperature in Celsius
 */
double getTempCelsius();

/**
 * @brief 	Get the current temperature in Fahrenheit
 * @return	the current temperature in Fahrenheit
 */
double getTempFahrenheit();
/**
 * 	@}
 */


/**
 * 	@}
 */

#endif
