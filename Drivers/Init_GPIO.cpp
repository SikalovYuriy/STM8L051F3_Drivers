/* =================================================================================
 File name:     Init_GPIO.cpp                                
 Creation date: 23-05-2014
 Description:   The GPIO initialization.
================================================================================== */

#include "Init_GPIO.hpp"

DriverGPIOpin<EnumGPIO::PORTB, EnumGPIO::PIN3> Open_collector_pin1;
DriverGPIOpin<EnumGPIO::PORTB, EnumGPIO::PIN4> Open_collector_pin0;

void Init_GPIO()
{
    Open_collector_pin1.set();
    Open_collector_pin1.direction(EnumGPIO::Direction_OUTPUT);
    Open_collector_pin1.output_config(EnumGPIO::OutType_OPEN_DRAIN, EnumGPIO::OutSpeed_2MHz);

    Open_collector_pin0.set();
    Open_collector_pin0.direction(EnumGPIO::Direction_OUTPUT);
    Open_collector_pin0.output_config(EnumGPIO::OutType_OPEN_DRAIN, EnumGPIO::OutSpeed_2MHz);
}


