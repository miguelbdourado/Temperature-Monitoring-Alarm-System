/*
 * BMP.c
 *
 *  Created on: 13 Jan 2022
 *      Author: Daniel Dron		46101
 *      		Miguel Dourado	46063
 *      		Diogo Cunha		46105
 */


#include "BMP.h"

double getTempCelsius()
{
	unsigned short digT1 = GetReg16(0x88);
	short digT2 = GetReg16(0x8A);
	short digT3 = GetReg16(0x8C);

	short FA = GetReg8(0xFA);
	short FB = GetReg8(0xFB);
	short FC = GetReg8(0xFC);

	unsigned long temperature = (FA << 12) | (FB << 4) | (FC >> 4);

	double var1 = (((double)temperature) / 16384.0 - ((double)digT1) / 1024) *
			((double) digT2);

	double var2 = ((((double)temperature) / 131072.0 - ((double) digT1) / 8192.0) *
			 (((double)temperature) / 131072.0 - ((double) digT1) / 8192.0)) *
			((double) digT3);

	double final_temperature = (var1 + var2) / 5120.0;

	return final_temperature;
}

double getTempFahrenheit()
{
	return (getTempCelsius() * 9 / 5) + 32;
}


int SetReg8(unsigned short registerName, unsigned short value)
{
	// Enable CSB
	LPC_GPIO2->FIOCLR |= (0x1 << 11);

	//read
	registerName &= ~(1 << 7);


	unsigned short txBuffer[2] = {registerName, value};
	unsigned short rxBuffer[2];
	int status = SPI_Transfer(&txBuffer, &rxBuffer, 2);

	// Disable CSB
	LPC_GPIO2->FIOSET |= (0x1 << 11);

	return status;
}

unsigned short GetReg16(unsigned short registerName)
{
	// Enable CSB
	LPC_GPIO2->FIOCLR |= (0x1 << 11);

	//read
	registerName |= (1 << 7);

	unsigned short valueRead[3] = {};
	unsigned short txBuffer[3] = { registerName, 0xff, 0xff };

	SPI_Transfer(txBuffer, &valueRead, 3);

	// Disable CSB
	LPC_GPIO2->FIOSET |= (0x1 << 11);

	return valueRead[1] | valueRead[2] << 8;
}


unsigned char GetReg8(unsigned char registerName)
{
	// Enable CSB
	LPC_GPIO2->FIOCLR |= (0x1 << 11);

	//read
	registerName |= (1 << 7);

	unsigned short valueRead[2] = {};
	unsigned short txBuffer[2] = { registerName, 0xff };

	SPI_Transfer(txBuffer, &valueRead, 2);

	// Disable CSB
	LPC_GPIO2->FIOSET |= (0x1 << 11);

	return valueRead[1];
}

void BMP_Init()
{
	// Setup P2[11] for output
	// P2[11] -> GPIO
	LPC_PINCON->PINSEL4 &= ~(0x3 << 22);
	// P2[11] -> OUTPUT
	LPC_GPIO2->FIODIR |= (0x1 << 11);
	// Set to high because active low CSB
	LPC_GPIO2->FIOSET |= (0x1 << 11);

	SPI_Init();
	SPI_ConfigTransfer(0, 8, 3);

	unsigned char ctrl_meas = GetReg8(0xF4) | 0x3 | 0x3 << 5;
	SetReg8(0xF4, ctrl_meas);
}
