/**
 * 	@file			FLASH.h
 * 	@brief		Contains the FLASH API
 * 	@version 	1.0
 * 	@date		15 Dez 2021
 * 	@author		Daniel Dron 46101, Miguel Dourado 46063, Diogo Cunha 46105
 *
 *	Copyright(C) 2021-2022, Grupo 12
 *	All rights reserved.
 *
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "LPC17xx.h"
#include <string.h>

/**	@defgroup FLASH FLASH
 *  This package provides the core capabilities for FLASH functions.
 *  @{
 */

#define IAP_LOCATION 0x1FFF1FF1

#define PREPARE_SECTOR_CODE 	50
#define COPY_RAM_TO_FLASH_CODE 	51
#define ERASE_SECTOR_CODE 		52
#define COMPARE_CODE			56

#define CMD_SUCCESS 			 0

static unsigned int command [5];
static unsigned int output  [5];

typedef void (*IAP)(unsigned int [], unsigned int []);
static IAP iap_entry;

/** @defgroup FLASH_Public_Function FLASH Public Functions
 *  @{
 */

/**
 * @brief	Initialises the FLASH API.
 * @note	This function must be called prior to any other FLASH functions.
 */
void FLASH_Init( );

/**
 * @brief	Erases a sector in flash memory.
 * @param	sector: The sector number to be erased.
 * @return	status code
 */
unsigned int FLASH_EraseSector( unsigned int sector );

/**
 * @brief	Write the data from srcAddr of size bytes to the address inside the FLASH memory pointed by dstAddr
 * @param	dstAddr: address inside a FLASH region to write the data to
 * @param	srcAddr: address containing the data
 * @param 	size: size of data
 * @return	status code
 */
unsigned int FLASH_WriteData( void* dstAddr, void* srcAddr, unsigned int size );

/**
 * @brief	Compares the content pointed by srcAddr of size bytes with the content pointed by dstAddr inside a FLASH region
 * @param	dstAddr: address inside a FLASH region to compare the data to
 * @param	srcAddr: address containing the data
 * @param 	size: size of data
 */
unsigned int FLASH_VerifyData( void* dstAddr, void* srcAddr, unsigned int size );
/**
 * 	@}
 */


/** @defgroup FLASH_Private_Function FLASH Private Functions
 *  @{
 */

/**
 * @brief	Clears the command and output buffer.
 */
void _FLASH_ClearCommandsAndOutput( );

/**
 * @brief	Get the sector number that contains the given address.
 * @param	addr: address inside a FLASH sector
 */
unsigned int _FLASH_GetSector( void* addr );

/**
 * @brief	Prepare a FLASH section to be written.
 * @param	startSector: start sector
 * @param	endSector: end sector
 */
unsigned int _FLASH_PrepareSectorForWrite( unsigned int startSector, unsigned int endSector );
/**
 * 	@}
 */

/**
 * 	@}
 */
#endif /* FLASH_H_ */
