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

#define INNER_FLASH_ADDRESS 0x3F80
#define SONG_DATA_ADDRESS 0x0600
#define SONG_SETTING_ADDRESS 0x0500 // TODO: 0x0000に変更

uint8_t playing_song = 0;
uint32_t saving_address = SONG_DATA_ADDRESS;

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

uint8_t SPIFlashByteReadOnce(uint32_t address) {
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
uint8_t SPIFlashByteRead(void) {
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

void save_end_address(uint16_t song_num, uint32_t end_address) {
    uint16_t Buf[ERASE_FLASH_BLOCKSIZE];
    FLASH_WriteWord((song_num*0x010) + INNER_FLASH_ADDRESS + 0x000, Buf, (uint8_t)(end_address>>16));
    FLASH_WriteWord((song_num*0x010) + INNER_FLASH_ADDRESS + 0x001, Buf, (uint8_t)(end_address>>8));
    FLASH_WriteWord((song_num*0x010) + INNER_FLASH_ADDRESS + 0x002, Buf, (uint8_t)(end_address>>0));
}

void save_end_address_spi(uint32_t song_num, uint32_t end_address) {
    SPIFlashByteWrite(SONG_SETTING_ADDRESS + (song_num*0x010) + 0x000, (uint8_t)(end_address>>16));
    SPIFlashByteWrite(SONG_SETTING_ADDRESS + (song_num*0x010) + 0x001, (uint8_t)(end_address>>8));
    SPIFlashByteWrite(SONG_SETTING_ADDRESS + (song_num*0x010) + 0x002, (uint8_t)(end_address>>0));
}

uint32_t load_end_address_spi(uint32_t song_num) {
    uint32_t end_address = SPIFlashByteReadOnce(SONG_SETTING_ADDRESS + (song_num*0x010) + 0x000);
    end_address = (end_address<<8) + (uint32_t)SPIFlashByteReadOnce(SONG_SETTING_ADDRESS + (song_num*0x010) + 0x001);
    end_address = (end_address<<8) + (uint32_t)SPIFlashByteReadOnce(SONG_SETTING_ADDRESS + (song_num*0x010) + 0x002);
    return end_address;
}

void save_1st_music(void) {
    save_end_address_spi(1, saving_address);
}

void save_2nd_music(void) {
    save_end_address_spi(2, saving_address);
}

void save_music(void) {
    uint8_t data;
    saving_address = SONG_DATA_ADDRESS;
    SPI_Open(SPI_DEFAULT);
    //SPIFlashUnprotect();
    SPIFlashErase();
    while(1) {
        data = getch();
        SPIFlashByteWrite(saving_address++, data);
    }
    SPI_Close();
}

void save_music_dummy(void) {
    SPI_Open(SPI_DEFAULT);
    SPIFlashUnprotect();
    CS_SetLow();
    SPI_ExchangeByte(0x20);
    SendAdrs(SONG_SETTING_ADDRESS);
    CS_SetHigh();
    save_end_address_spi(1, 120000);
    save_end_address_spi(2, 240000);
    SPI_Close();
}

uint32_t load_end_address(uint16_t song_num) {
    uint32_t end_address = FLASH_ReadWord((song_num*0x010) + INNER_FLASH_ADDRESS + 0x000);
    end_address = (end_address<<8) + (uint32_t)FLASH_ReadWord((song_num*0x010) + INNER_FLASH_ADDRESS + 0x001);
    end_address = (end_address<<8) + (uint32_t)FLASH_ReadWord((song_num*0x010) + INNER_FLASH_ADDRESS + 0x002);
    return end_address;
}

void play_music(void) {
    uint8_t data;
    uint32_t counter = 0;
    SPI_Open(SPI_DEFAULT);
    SPIFlashReadOpen(80);
    while(1) {
        while(PIR1bits.TMR2IF == 0);
        PIR1bits.TMR2IF = 0;
        data = SPIFlashByteRead();
        DAC1_Load10bitInputData(data);
        if (counter++ >= 240000) {
            counter = 0;
            SPIFlashReadClose();
            RB5 = 1;
            __delay_ms(3000);
            RB5 = 0;
            SPIFlashReadOpen(80);
        }
    }
    SPI_Close();
}

void play_music_once(uint32_t song_num) {
    uint32_t from_address;
    uint32_t to_address;
    uint32_t i;
    uint8_t data;
    RC3 = 0;
    CS_SetHigh();
    SPI_Open(SPI_DEFAULT);
    if (song_num < 2) {
        from_address = SONG_DATA_ADDRESS;
    } else {
        from_address = load_end_address_spi(song_num - 1) + 1;
    }
    to_address = load_end_address_spi(song_num);
    SPIFlashReadOpen(from_address);
    for (i = from_address; i < to_address; i++) {
        while(PIR1bits.TMR2IF == 0);
        PIR1bits.TMR2IF = 0;
        data = SPIFlashByteRead();
        DAC1_Load10bitInputData(data);
    }
    SPIFlashReadClose();
    SPI_Close();
    RC3 = 1;
}

void play_1st_music(void) {
    play_music_once(1);
}

void play_2nd_music(void) {
    play_music_once(2);
}

/*
                         Main application
 */
void main(void)
{
    SYSTEM_Initialize();
    RC3 = 1;
    CS_SetHigh();
    ei();
    if (RC0) {
        RB5 = 1;
        __delay_ms(100);
        IOCCF1_SetInterruptHandler(save_1st_music);
        IOCCF2_SetInterruptHandler(save_2nd_music);
        save_music();
//        save_music_dummy();
    } else {
        IOCCF1_SetInterruptHandler(play_1st_music);
        IOCCF2_SetInterruptHandler(play_2nd_music);
        while(1) {
            SLEEP();
        }
    } 
}
/**
 End of File
*/