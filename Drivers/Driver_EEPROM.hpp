/* =================================================================================
 File name:     Driver_EEPROM.hpp                                
 Creation date: 23-05-2014
 Description:   The driver EEPROM.
================================================================================== */

#ifndef _DRIVEREEPROM_H_
#define _DRIVEREEPROM_H_

#include "type.h"

class DriverEEPROM
{
private:
    struct EEPROM_struct
    {
      volatile unsigned char CR1;        /*!< Flash control register 1 */
      volatile unsigned char CR2;        /*!< Flash control register 2 */
      volatile unsigned char PUKR;       /*!< Flash program memory unprotection register */
      volatile unsigned char DUKR;       /*!< Data EEPROM unprotection register */
      volatile unsigned char IAPSR;      /*!< Flash in-application programming status register */
    };

    u32 address;
    
public:
    DriverEEPROM();

    u16 read(u32 Address);
    void read(u16 *dest, u32 Address, u32 size);
    void write(u32 Address, const u16* source, u32 size);
    void write(u32 Address, const u16 source);

    DriverEEPROM & operator[] (u32 address) 
    {
        this->address = address; 
        return *this;
    }

    DriverEEPROM & operator= (const u16 &val)
    {
        write(address, val); 
        return *this;
    }
};

#endif

