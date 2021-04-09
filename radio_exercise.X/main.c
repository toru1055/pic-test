/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F1778
        Driver Version    :  2.00
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

#include "mcc_generated_files/mcc.h"
void CS_SetLow() {
    RB1 = 0;
}
void CS_SetHigh() {
    RB1 = 1;
}
void SendAdrs(uint32_t address) {
    SPI_ExchangeByte((uint8_t)(address>>16));
    SPI_ExchangeByte((uint8_t)(address>>8));
    SPI_ExchangeByte((uint8_t)(address));
}
uint8_t SPIFlashByteRead(uint32_t address) {
    uint8_t data;
    CS_SetLow();
    SPI_ExchangeByte(0x03);
    SendAdrs(address);
    data = SPI_ExchangeByte(0xAA);
    CS_SetHigh();
    return data;
}
void SPIFlashReadOpen(uint32_t address) {
    CS_SetLow();
    SPI_ExchangeByte(0x03);
    SendAdrs(address);
}
void SPIFlashReadClose(void) {
    CS_SetHigh();
}
uint8_t SPIFlashByteRead2(void) {
    return SPI_ExchangeByte(0xAA);
}
void SPIFlashByteWrite(uint32_t address, uint8_t data) {
    CS_SetLow();
    SPI_ExchangeByte(0x06); // Write Enable
    CS_SetHigh();
    CS_SetLow();
    SPI_ExchangeByte(0x02);
    SendAdrs(address);
    SPI_ExchangeByte(data);
    CS_SetHigh();
    __delay_ms(2);
}

void SPIFlashWriteOpen(uint32_t address) {
    CS_SetLow();
    SPI_ExchangeByte(0x06); // Write Enable
    CS_SetHigh();
    CS_SetLow();
    SPI_ExchangeByte(0x02);
    SendAdrs(address);
}
void SPIFlashWriteClose(void) {
    CS_SetHigh();
}
void SPIFlashWrite2(uint8_t data) {
    SPI_ExchangeByte(data);
}

void SPIFlashErase(void) {
    CS_SetLow();
    SPI_ExchangeByte(0x06);
    CS_SetHigh();
    __delay_ms(50);
    CS_SetLow();
    SPI_ExchangeByte(0xC7);
    CS_SetHigh();
    __delay_ms(50);
}

void SPIFlashUnprotect(void) {
    uint8_t i;
    CS_SetLow();
    SPI_ExchangeByte(0x06);
    CS_SetHigh();
    __delay_ms(50);
    CS_SetLow();
    SPI_ExchangeByte(0x42);
    for(i=0; i < 18; i++) {
        SPI_ExchangeByte(0x00);
    }
    CS_SetHigh();
    __delay_ms(50);
}

void save_music(void) {
    uint8_t data;
    uint32_t address = 0;
    SPI_Open(SPI_DEFAULT);
    CS_SetHigh();
    SPIFlashUnprotect();
    SPIFlashErase();
    SPIFlashWriteOpen(0);
    while(1) {
        RB5 = 1;
        data = getch();
        SPIFlashWrite2(data);
//        if (address++ % 1000 == 0) {
//            printf("%u\r\n", address);
//        }
    }
    SPI_Close();
}
void save_music_old(void) {
    uint8_t data;
    uint32_t address = 0;
    uint32_t counter = 0;
    SPI_Open(SPI_DEFAULT);
    CS_SetHigh();
    SPIFlashUnprotect();
    SPIFlashErase();
    
    SPIFlashReadOpen(0);
    while(1) {
        while(PIR1bits.TMR2IF == 0);
        PIR1bits.TMR2IF = 0;
        data = SPIFlashByteRead2();
        DAC1_Load10bitInputData(data);
        if (counter++ >= 10000) {
            SPIFlashReadClose();
            RB5 = 1;
            __delay_ms(3000);
            RB5 = 0;
            __delay_ms(3000);
            break;
        }
        //printf("%d\r\n", data);
    }
    
    while(1) {
        RB5 = 1;
        data = getch();
        SPIFlashByteWrite(address++, data);
//        if (address % 1000 == 0) {
//            printf("address=%u, data=%x\r\n", address, data);
//        }
    }
    SPI_Close();
}

void play_music(void) {
    uint8_t data;
    uint32_t counter = 0;
    SPI_Open(SPI_DEFAULT);
    CS_SetHigh();
    SPIFlashReadOpen(80);
    while(1) {
        while(PIR1bits.TMR2IF == 0);
        PIR1bits.TMR2IF = 0;
        data = SPIFlashByteRead2();
        DAC1_Load10bitInputData(data);
        if (counter++ >= 240000) {
            counter = 0;
            SPIFlashReadClose();
            RB5 = 1;
            __delay_ms(3000);
            RB5 = 0;
            SPIFlashReadOpen(80);
        }
        //printf("%d\r\n", data);
    }
    SPI_Close();
}

/*
                         Main application
 */
void main(void)
{
    SYSTEM_Initialize();
    if (RC0) {
        __delay_ms(100);
        save_music_old();
    } else {
        play_music();
    }
}
/**
 End of File
*/