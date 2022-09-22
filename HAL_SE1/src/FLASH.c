/*
 * FLASH.c
 *
 *  Created on: 9 Dec 2021
 *      Author: Daniel Dron
 */

#include "FLASH.h"

void FLASH_Init( )
{
	iap_entry = (IAP)IAP_LOCATION;
}

unsigned int FLASH_EraseSector( unsigned int sector )
{
	_FLASH_PrepareSectorForWrite(sector, sector);

	_FLASH_ClearCommandsAndOutput();

	command[0] = ERASE_SECTOR_CODE;
	command[1] = sector;	//Param0
	command[2] = sector;	//Param1
	command[3] = SystemCoreClock / 1000;

	iap_entry(command, output);

	//Return Code
	return output[0];
}


unsigned int FLASH_WriteData( void* dstAddr, void* srcAddr, unsigned int size )
{
	//We might need to write to more than one sector depending on the size
	unsigned int startSector = _FLASH_GetSector(dstAddr);
	unsigned int endSector   = _FLASH_GetSector(dstAddr + size);

	if ( _FLASH_PrepareSectorForWrite( startSector, endSector ) != CMD_SUCCESS )
		return output[0];

	//Clear the command and output array from previous usage
	_FLASH_ClearCommandsAndOutput( );

	command[0] = COPY_RAM_TO_FLASH_CODE;
	command[1] = (unsigned int)dstAddr;
	command[2] = (unsigned int)srcAddr;
	command[3] = size;
	command[4] = SystemCoreClock / 1000;

	iap_entry(command, output);
	return output[0];
}

unsigned int _FLASH_PrepareSectorForWrite( unsigned int startSector, unsigned int endSector )
{
	_FLASH_ClearCommandsAndOutput( );

	command[0] = PREPARE_SECTOR_CODE;
	command[1] = startSector;
	command[2] = endSector;

	iap_entry( command, output );
	return output[0];
}



void _FLASH_ClearCommandsAndOutput( )
{
	memset(command, 0, sizeof(unsigned int) * 5);
	memset(output, 0, sizeof(unsigned int) * 5);
}

unsigned int _FLASH_GetSector( void* addr )
{
	unsigned int uiAddr = (unsigned int)addr;

	if ( uiAddr >= 0x00000000 && uiAddr <= 0x0000FFFF )
		return uiAddr >> 0xC;

	else if ( uiAddr >= 0x00010000 && uiAddr <= 0x0007FFFF )
		return ((uiAddr - 0x00010000) >> 0xF) | 0x10;

	//Address does not belong to any sector
	return -1;
}

unsigned int FLASH_VerifyData( void* dstAddr, void* srcAddr, unsigned int size )
{
	_FLASH_ClearCommandsAndOutput( );

	command[0] = COMPARE_CODE;
	command[1] = (unsigned int)dstAddr;
	command[2] = (unsigned int)srcAddr;
	command[3] = size;

	iap_entry( command, output );
	return output[0];
}





