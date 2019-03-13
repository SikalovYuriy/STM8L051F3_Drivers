/* =================================================================================
 File name:     Driver_GPIO.hpp                                
 Creation date: 21-05-2014
 Description:   The GPIO driver.
================================================================================== */

#ifndef _DRIVERGPIO_H_
#define _DRIVERGPIO_H_

#include "iostm8l051f3.h"
#include "type.h"

class DriverGPIO
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

    GPIO_struct *GPIO_port;
    bool dir_pin;
public:
    
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

    DriverGPIO(PORT port_loc) 
    {
        GPIO_port = (GPIO_struct *) port_loc;
    }

    void output_config(PIN pin_num, OutputPinType pt, OutputPinSpeed ps)
    {
        if(pt) 
            GPIO_port->CR1 |= pin_num;
        else 
            GPIO_port->CR1 &= ~pin_num;

        if(ps) 
            GPIO_port->CR2 |= pin_num;
        else 
            GPIO_port->CR2 &= ~pin_num;
    }

    void input_config(PIN pin_num, InputPinType pt, InputInterrupt ps)
    {
        if(pt) 
            GPIO_port->CR1 |= pin_num;
        else 
            GPIO_port->CR1 &= ~pin_num;

        if(ps) 
            GPIO_port->CR2 |= pin_num;
        else 
            GPIO_port->CR2 &= ~pin_num;
    }

    void direction(PIN pin_num, Direction dir)
    {
        dir_pin = dir;
        if(dir) {
            GPIO_port->DDR |= pin_num;
        }
        else{
            GPIO_port->DDR &= ~pin_num;
        }
    }

    void set_pin(PIN pin_num)
    {
        GPIO_port->ODR |= pin_num;
    }

    void clear_pin(PIN pin_num)
    {
        GPIO_port->ODR &= ~pin_num;
    }

    void toggle(PIN pin_num)
    {
        GPIO_port->ODR ^= pin_num;
    }

    bool read_pin(PIN pin_num)
    {
        if(dir_pin){
            return GPIO_port->ODR & pin_num;
        }
        else{
            return GPIO_port->IDR & pin_num;
        }

    }
};


#endif
