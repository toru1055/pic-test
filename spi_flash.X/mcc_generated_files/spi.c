/**
  SPI Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    spi.c

  @Summary
    This is the generated driver implementation file for the SPI driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for SPI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F1778
        Driver Version    :  1.0.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above or later
        MPLAB             :  MPLAB X 5.45
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "spi.h"
#include <xc.h>

typedef struct { 
    uint8_t con1; 
    uint8_t stat;
    uint8_t add;
    uint8_t operation;
} spi_configuration_t;

//con1 == SSPxCON1, stat == SSPxSTAT, add == SSPxADD, operation == Master/Slave
static const spi_configuration_t spi_configuration[] = {   
    { 0x0, 0x40, 0x1, 0 }
};

void SPI_Initialize(void)
{
    //Setup PPS Pins
    SSPCLKPPS = 10;
    SSPDATPPS = 11;
    RB2PPS    = 33;
    RB4PPS    = 35;
    //SPI setup
    SSP1STAT = 0x40;
    SSP1CON1 = 0x00;
    SSP1ADD = 0x01;
    TRISBbits.TRISB2 = 0;
    SSP1CON1bits.SSPEN = 0;
}

bool SPI_Open(spi_modes_t spiUniqueConfiguration)
{
    if(!SSP1CON1bits.SSPEN)
    {
        SSP1STAT = spi_configuration[spiUniqueConfiguration].stat;
        SSP1CON1 = spi_configuration[spiUniqueConfiguration].con1;
        SSP1CON2 = 0x00;
        SSP1ADD  = spi_configuration[spiUniqueConfiguration].add;
        TRISBbits.TRISB2 = spi_configuration[spiUniqueConfiguration].operation;
        SSP1CON1bits.SSPEN = 1;
        return true;
    }
    return false;
}

void SPI_Close(void)
{
    SSP1CON1bits.SSPEN = 0;
}

uint8_t SPI_ExchangeByte(uint8_t data)
{
    SSP1BUF = data;
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF = 0;
    return SSP1BUF;
}

void SPI_ExchangeBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        SSP1BUF = *data;
        while(!PIR1bits.SSP1IF);
        PIR1bits.SSP1IF = 0;
        *data++ = SSP1BUF;
    }
}

// Half Duplex SPI Functions
void SPI_WriteBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        SPI_ExchangeByte(*data++);
    }
}

void SPI_ReadBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        *data++ = SPI_ExchangeByte(0);
    }
}

void SPI_WriteByte(uint8_t byte)
{
    SSP1BUF = byte;
}

uint8_t SPI_ReadByte(void)
{
    return SSP1BUF;
}