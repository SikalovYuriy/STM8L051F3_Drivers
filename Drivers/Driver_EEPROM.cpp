/* =================================================================================
 File name:     Driver_EEPROM.cpp                                
 Creation date: 23-05-2014
 Description:   The driver EEPROM.
================================================================================== */

#include "Driver_EEPROM.hpp"
#include "stm8l15x.h"

DriverEEPROM::DriverEEPROM()
{
    //FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    FLASH->CR1 &= ~FLASH_CR1_FIX;
    FLASH->CR1 |= 0x00;
    //FLASH_PowerWaitModeConfig(FLASH_Power_IDDQ);
    FLASH->CR1 &= (uint8_t)(~FLASH_CR1_WAITM);
}

//---------------------------------------------------------------
u16 DriverEEPROM::read(u32 Address)
{
    u16 tmp;
    Address *= 2;
    Address += 0x1000;

    tmp  = FLASH_ReadByte(Address) << 8;
    tmp |= FLASH_ReadByte(Address + 1);

    return tmp;
}

//---------------------------------------------------------------
void DriverEEPROM::read(u16 *dest, u32 Address, u32 size)
{
    Address *= 2;
    Address += 0x1000;

    for (u16 i = 0; i < size; i++, Address += 2, dest++) {
        *dest = FLASH_ReadByte(Address) << 8;
        *dest |= FLASH_ReadByte(Address + 1);
    }
}

//---------------------------------------------------------------
void DriverEEPROM::write(u32 Address, const u16* source, u32 size)
{
    Address *= 2;
    Address += 0x1000;

    //FLASH_Lock(FLASH_MemType_Data);
    FLASH->IAPSR &= (uint8_t)FLASH_MemType_Data;
    //FLASH_Unlock(FLASH_MemType_Data);
    FLASH->DUKR = FLASH_RASS_KEY2; 
    FLASH->DUKR = FLASH_RASS_KEY1;

    for (u16 i = 0; i < size; i++, Address += 2, source++) {
        FLASH_WaitForLastOperation(FLASH_MemType_Data);
        FLASH_ProgramByte(Address,     (u8)(*source >> 8));
        FLASH_WaitForLastOperation(FLASH_MemType_Data);
        FLASH_ProgramByte(Address + 1, (u8)(*source));
    }

    //FLASH_Lock(FLASH_MemType_Data);
    FLASH->IAPSR &= (uint8_t)FLASH_MemType_Data;
}

//---------------------------------------------------------------
void DriverEEPROM::write(u32 Address, const u16 source)
{
    Address *= 2;
    Address += 0x1000;

    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    //FLASH_Lock(FLASH_MemType_Data);
    FLASH->IAPSR &= (uint8_t)FLASH_MemType_Data;
    //FLASH_Unlock(FLASH_MemType_Data);
    FLASH->DUKR = FLASH_RASS_KEY2; 
    FLASH->DUKR = FLASH_RASS_KEY1;
    FLASH_ProgramByte(Address,     (u8)(source >> 8));
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    FLASH_ProgramByte(Address + 1, (u8)(source));
    //FLASH_Lock(FLASH_MemType_Data);
    FLASH->IAPSR &= (uint8_t)FLASH_MemType_Data;
}

