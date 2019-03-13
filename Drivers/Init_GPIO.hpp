/* =================================================================================
 File name:     Init_GPIO.hpp                                
 Creation date: 23-05-2014
 Description:   The GPIO initialization.
================================================================================== */

#ifndef _INITGPIO_H_
#define _INITGPIO_H_

#include "Driver_GPIO_pin.hpp"

extern DriverGPIOpin<EnumGPIO::PORTB, EnumGPIO::PIN3> Open_collector_pin1;
extern DriverGPIOpin<EnumGPIO::PORTB, EnumGPIO::PIN4> Open_collector_pin0;

void Init_GPIO();

#endif

