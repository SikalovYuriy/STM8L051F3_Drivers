#include "type.h"
#include "Init_UART.hpp"
#include "Driver_UART.hpp"
#include "iostm8l051f3.h"
#include "Global_defines.hpp"

volatile bool flag_recieve = false;

DriverUART Uart1(DriverUART::UART1, 
                 baud_rate, 
                 system_clock_frequence, 
                 PinconfigUART(PinconfigUART::TXonPC5_RXonPC6));

//-----------------------------------------------------------------------------------------------------
// Прерывание по приему
//-----------------------------------------------------------------------------------------------------
void DriverUART::receive_handle()
{
    flag_recieve = true;

    Uart1.reception_handshake();
}

//-----------------------------------------------------------------------------------------------------
// Инициализация пина управления приема или передачи
//-----------------------------------------------------------------------------------------------------
void Init_direction_gpio()
{
    PC_DDR_bit.DDR4 = 1; // output
    PC_CR1_bit.C14 = 1;  // push-pull mode
    PC_CR2_bit.C24 = 0;  // output speed up to 2 MHz
    PC_ODR_bit.ODR4 = 0;
}

//-----------------------------------------------------------------------------------------------------
// Инициализация UART
//-----------------------------------------------------------------------------------------------------
void Init_UART()
{
    //Uart1 = DriverUART::BUFF_COMMON;
    Uart1 = DriverUART::CHR_8;
    Uart1 = DriverUART::PARITY_DISABLE;
    Uart1 = DriverUART::STOPBIT_1;
    Uart1 = DriverUART::TRASMIT_ENABLE;
    Uart1 = DriverUART::RECIEVE_ENABLE;
    Uart1.set_baudrate(baud_rate);
    Uart1.get_pointer_on_internal_buffer();

    Init_direction_gpio();
}

//-----------------------------------------------------------------------------------------------------
// Выбор аппаратного режима приема или передачи
//-----------------------------------------------------------------------------------------------------
void DriverUART::select_direction(DriverUART::TRANSMITION_OR_RECEPTION dir)
{
    PC_ODR_bit.ODR4 = dir;
}

