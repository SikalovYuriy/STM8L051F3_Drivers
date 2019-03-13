/* =================================================================================
 File name:     Driver_GPIO_pin.hpp                                
 Creation date: 23-05-2014
 Description:   The GPIO driver.
================================================================================== */

#ifndef _DRIVERGPIOPIN_H_
#define _DRIVERGPIOPIN_H_

#include "iostm8l051f3.h"
#include "type.h"

#define __PORT ((GPIO_struct*)PortSel)

struct EnumGPIO
{
    enum PORT{
        PORTA = 0x5000,
        PORTB = 0x5005,
        PORTC = 0x500A,
        PORTD = 0x500F
    };
    
    enum PIN{
        PIN0 = 0x01, 
        PIN1 = 0x02, 
        PIN2 = 0x04, 
        PIN3 = 0x08, 
        PIN4 = 0x10, 
        PIN5 = 0x20, 
        PIN6 = 0x40, 
        PIN7 = 0x80
    };
    
    enum Direction{
        Direction_INPUT,
        Direction_OUTPUT
    };

    enum OutputPinType{
        OutType_OPEN_DRAIN,
        OutType_PUSH_PULL
    };

    enum OutputPinSpeed{
        OutSpeed_2MHz,
        OutSpeed_10MHz
    };

    enum InputPinType{
        InType_FLOATING,
        InType_PULL_UP
    };

    enum InputInterrupt{
        InInterrupt_DISABLE,
        InInterrupt_ENABLE
    };
};

template<EnumGPIO::PORT PortSel, EnumGPIO::PIN PinSel>
class DriverGPIOpin
{   
private:
    struct GPIO_struct
    {
      volatile u8 ODR; /*!< Output Data Register */
      volatile u8 IDR; /*!< Input Data Register */
      volatile u8 DDR; /*!< Data Direction Register */
      volatile u8 CR1; /*!< Configuration Register 1 */
      volatile u8 CR2; /*!< Configuration Register 2 */
    };
    bool dir_pin;

public:
    void output_config(EnumGPIO::OutputPinType pt, EnumGPIO::OutputPinSpeed ps)
    {
        if(pt) 
            __PORT->CR1 |= PinSel;
        else 
            __PORT->CR1 &= ~PinSel;

        if(ps) 
            __PORT->CR2 |= PinSel;
        else 
            __PORT->CR2 &= ~PinSel;
    }

    void input_config(EnumGPIO::InputPinType pt, EnumGPIO::InputInterrupt ps)
    {
        if(pt) 
            __PORT->CR1 |= PinSel;
        else 
            __PORT->CR1 &= ~PinSel;

        if(ps) 
            __PORT->CR2 |= PinSel;
        else 
            __PORT->CR2 &= ~PinSel;
    }

    void direction(EnumGPIO::Direction dir)
    {
        dir_pin = dir;
        if(dir) {
            __PORT->DDR |= PinSel;
        }
        else{
            __PORT->DDR &= ~PinSel;
        }
    }

    void set()
    {
        __PORT->ODR |= PinSel;
    }

    void clear()
    {
        __PORT->ODR &= ~PinSel;
    }

    void toggle()
    {
        __PORT->ODR ^= PinSel;
    }

    bool read()
    {
        if(dir_pin){
            return __PORT->ODR & PinSel;
        }
        else{
            return __PORT->IDR & PinSel;
        }

    }
};


#endif

