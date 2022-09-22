/**
 * 	@file			SPI.h
 * 	@brief		Contains the FLASH API
 * 	@version 	1.0
 * 	@date		06 Jan 2022
 * 	@author		Daniel Dron 46101, Miguel Dourado 46063, Diogo Cunha 46105
 *
 *	Copyright(C) 2021-2022, Grupo 12
 *	All rights reserved.
 *
 */


#ifndef SPI_H_
#define SPI_H_

#include "LPC17xx.h"
#include <string.h>

/**	@defgroup SPI SPI
 *  This package provides the core capabilities for SPI functions.
 *  @{
 */

#define ENABLE_SPI (0x1 << 8)

#define BIT_ENABLE 4
#define MASTER_MODE 0x20

#define CPHA_MASK 3
#define CPOL_MASK 4
#define MSTR_MASK 5
#define BITS_MASK 8

#define SPIF_MASK 0x80
#define STATUS_MASK 0x78

/** @defgroup SPI_Public_Function SPI Public Functions
 *  @{
 */

/**
 * @brief	Initialises the SPI API.
 * @note	This function must be called prior to any other SPI functions.
 */
void SPI_Init();

/**
 * @brief	Used to configure the transfer.
 * @param	frequency: The frequency at which the transfer operates.
 * @param	bitData: The length in bits to transfer.
 * @param	mode: Configure the SPI Data To Clock Phase Relationship.
 */
void SPI_ConfigTransfer(int frequency, int bitData, int mode);

/**
 * @brief	Used to transfer in and out.
 * @param	txBuffer: Pointer to a buffer of shorts that will be transfered out.
 * @param	rxBuffer: Pointer to the destination buffer where the contents will be read to.
 * @param	length: length in number of shorts to be read or written.
 */
int SPI_Transfer(unsigned short* txBuffer, unsigned short* rxBuffer, int length);

/**
 * 	@}
 */

/**
 * 	@}
 */

#endif /* SPI_H_ */
