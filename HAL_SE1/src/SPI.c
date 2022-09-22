/*
 * SPI.c
 *
 *  Created on: 16 Dec 2021
 *      Author: Daniel Dron		46101
 *      		Miguel Dourado	46063
 *      		Diogo Cunha		46105
 */


#include "SPI.h"

static uint32_t spi_GetPclk(void)
{
    uint32_t v_spiPclk_u32,v_Pclk_u32;
    /**
       PCLKSELx registers contains the PCLK info for all the clock dependent peripherals.
       Bit16,Bit17 contains the SPI Clock(ie.SPI_PCLK) information.
       The SPI_PCLK and the actual Peripheral Clock(PCLK) is calculated as below.
       (Refer data sheet for more info)
       SPI_PCLK    PCLK
         0x00       SystemFreq/4
         0x01       SystemFreq
         0x02       SystemFreq/2
         0x03       SystemFreq/8
     **/

    v_spiPclk_u32 = (LPC_SC->PCLKSEL0 >> 16) & 0x03;

    switch( v_spiPclk_u32 )
    {
    case 0x00:
        v_Pclk_u32 = SystemCoreClock/4;           //SystemFrequency or  SystemCoreClock
        break;
    case 0x01:
        v_Pclk_u32 = SystemCoreClock;
        break;
    case 0x02:
        v_Pclk_u32 = SystemCoreClock/2;
        break;
    case 0x03:
        v_Pclk_u32 = SystemCoreClock/8;
        break;
    }

    return (v_Pclk_u32);
}


void SPI_Init()
{
	/*
	 * P0.15 -> MODE 11 (SCK)   OUTPUT
	 * P0.16 -> MODE 11 (SSEL)  OUTPUT
	 * P0.17 -> MODE 11 (MISO)   INPUT sdo
	 * P0.18 -> MODE 11 (MOSI)  OUTPUT sdi
	 */
	LPC_PINCON->PINSEL0 |= (0x3 << 30);
	LPC_PINCON->PINSEL1 |= 0x3F;

	LPC_GPIO0->FIODIR |= (1 << 15);	//OUTPUT
	LPC_GPIO0->FIODIR |= (1 << 16);	//OUTPUT
	LPC_GPIO0->FIODIR &= ~(1 << 17); // INPUT
	LPC_GPIO0->FIODIR |= (1 << 18); //OUTPUT

	LPC_GPIO0->FIOSET |= (1 << 16); //Disable SSEL

	LPC_SC->PCONP |= ENABLE_SPI;

	LPC_SPI->SPCCR = spi_GetPclk() / 4000000;	//freq 25

	LPC_SPI->SPCR = (1 << CPHA_MASK) | (1 << CPOL_MASK) | (1 << MSTR_MASK);
}

void SPI_ConfigTransfer(int frequency, int bitData, int mode)
{
	uint32_t freqCalc = (SystemCoreClock / 4) / frequency;

	if(freqCalc % 2 == 0 && freqCalc >= 8)
		LPC_SPI->SPCCR = freqCalc;
	else
		LPC_SPI->SPCCR = 8;

	// Bit enable and set number of bits per transmission
	LPC_SPI->SPCR |= MASTER_MODE | BIT_ENABLE | (bitData << BITS_MASK);

	// Set CPHA and CPOL
	LPC_SPI->SPCR |= (mode << CPHA_MASK);
}


uint8_t dummy_u8;
//								 send				   	   read
int SPI_Transfer(unsigned short* txBuffer, unsigned short* rxBuffer, int length)
{
	for( unsigned int i = 0; i < length; i++)
	{
		LPC_SPI->SPDR = txBuffer[i];	// write
		while(!(LPC_SPI->SPSR & SPIF_MASK)); //wait for SPI transfer complete flag

		rxBuffer[i] = LPC_SPI->SPDR;	// read
	}

	return LPC_SPI->SPSR & STATUS_MASK;
}


uint8_t SPI_Write (uint8_t spiData_u8)
{
    LPC_SPI->SPDR = spiData_u8;
    while(!(LPC_SPI->SPSR & SPIF_MASK)); /* wait until data is sent */
    dummy_u8 = LPC_SPI->SPSR;
    dummy_u8 = LPC_SPI->SPDR;

    return spiData_u8;
}

uint8_t SPI_Read(void)
{
    uint8_t spiData_u8;

    LPC_SPI->SPDR = 0xff;

    while(!(LPC_SPI->SPSR & SPIF_MASK)); /* wait until data is received */
    dummy_u8 = LPC_SPI->SPSR;                               /* Dummy read to clear the flags */
    spiData_u8 = (uint8_t)LPC_SPI->SPDR;

    return spiData_u8;
}
