/* =================================================================================
 File name:     Driver_UART.cpp                                
 Autor:         Sikalov Yuriy
 Description:   The UART driver.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 14-08-2013 Version 1.00
-----------------------------------------------------------------------------------*/

#include "Driver_UART.hpp"
#include "type.h"
#include "iostm8l051f3.h"
#include "intrinsics.h"     //Здесь описана функция __enable_interrupt()

//-----------------------------------------------------------------------------------------------------
// Main PinconfigUART Constructor 
//-----------------------------------------------------------------------------------------------------
PinconfigUART::PinconfigUART(TXRX_PIN_SELECT pinselect)
{
    set_UART_pin(pinselect);
}

void PinconfigUART::set_UART_pin(TXRX_PIN_SELECT pinselect)
{
    SYSCFG_RMPCR1_bit.USART1TR_REMAP = pinselect;
}

//-----------------------------------------------------------------------------------------------------
// Main DriverUART Constructor 
//-----------------------------------------------------------------------------------------------------
DriverUART::DriverUART(SELECTUART uart, u32 baud_rate, u32 sys_clock, PinconfigUART confPin)
{
    CLK_PCKENR1_bit.PCKEN15 = 1; // UART clock enable
    USART1_CR1_bit.USARTD = 0;
    USART1_CR5_bit.DMAT = 1;        // DMA enable transmitter
    USART1_CR5_bit.DMAR = 1;        // DMA enable receiver

    DMAChannelTX.global_disable();

    DMAChannelTX.set_periph_addr(&USART1_DR);
    DMAChannelTX.set_memory0_addr(UartBuffer);
    DMAChannelTX = EnumDMA::DATABLOCK_8bit;
    DMAChannelTX = EnumDMA::PRIORITY_MEDIUM;
    DMAChannelTX = EnumDMA::MEMPNT_INCREMENT;
    DMAChannelTX = EnumDMA::MEMORY_TO_PHERIPH;
    DMAChannelTX = EnumDMA::CIRCULAR_DISABLE;

    DMAChannelRX.set_periph_addr(&USART1_DR);
    DMAChannelRX.set_memory0_addr(UartBuffer);
    DMAChannelRX = EnumDMA::DATABLOCK_8bit;
    DMAChannelRX = EnumDMA::PRIORITY_MEDIUM;
    DMAChannelRX = EnumDMA::MEMPNT_INCREMENT;
    DMAChannelRX = EnumDMA::PHERIPH_TO_MEMORY;
    DMAChannelRX = EnumDMA::CIRCULAR_DISABLE;
    DMAChannelRX.set_number_of_transfers(sizeof(UartBuffer));
    DMAChannelRX.channel_enable();

    DMAChannelTX.global_enable();

    set_sysclock(sys_clock, baud_rate);

    USART1_CR2_bit.TCIEN = 1; // вкл. прерывания по окончанию передачи
    USART1_CR2_bit.ILIEN = 1; // вкл. прерывание по приему
    USART1_CR5_bit.EIE = 1;   // вкл. прерывание по ошибке приема
    __enable_interrupt();
   
    close();
}


//-----------------------------------------------------------------------------------------------------
// Set System Clock
//-----------------------------------------------------------------------------------------------------
void DriverUART::set_sysclock(u32 sys_clock, u32 baud_rate)
{
    this->sys_clock = sys_clock;

    this->set_baudrate(baud_rate);
}

//-----------------------------------------------------------------------------------------------------
// Baud rate
//-----------------------------------------------------------------------------------------------------
void DriverUART::set_baudrate(u32 baud_rate)
{
    u16 tmp = sys_clock / baud_rate;

    USART1_BRR2 = ((tmp >> 8) & 0xf0) + (tmp & 0x0f);
    USART1_BRR1 = (tmp >> 4) & 0xff;
}


//-----------------------------------------------------------------------------------------------------
// get_received_size
//-----------------------------------------------------------------------------------------------------
u16 DriverUART::get_received_size()
{
    return received_size;
}

//-----------------------------------------------------------------------------------------------------
// get_ponter_on_internal_buffer
//-----------------------------------------------------------------------------------------------------
u8* DriverUART::get_pointer_on_internal_buffer()
{
    return UartBuffer;
}

//-----------------------------------------------------------------------------------------------------
// Char length
//-----------------------------------------------------------------------------------------------------
void DriverUART::operator =(CHARLEN v)
{
    USART1_CR1_bit.M = v;
}

//-----------------------------------------------------------------------------------------------------
// Transmission selection
//-----------------------------------------------------------------------------------------------------
void DriverUART::operator =(TALK v)
{
    USART1_CR2_bit.TEN = v;
}

//-----------------------------------------------------------------------------------------------------
// Reception selection
//-----------------------------------------------------------------------------------------------------
void DriverUART::operator =(LISTEN v)
{
    USART1_CR2_bit.REN = v;
}


//-----------------------------------------------------------------------------------------------------
// Parity control
//-----------------------------------------------------------------------------------------------------
void DriverUART::operator =(PARITY_CONTROL v)
{
    USART1_CR1_bit.PCEN = v; 
}

//-----------------------------------------------------------------------------------------------------
// Parity selection
//-----------------------------------------------------------------------------------------------------
void DriverUART::operator =(PARITY_SELECTION v)
{
    USART1_CR1_bit.PS = v; 
}

//-----------------------------------------------------------------------------------------------------
// Stop bit selection
//-----------------------------------------------------------------------------------------------------
void DriverUART::operator =(STOP_BIT v)
{
    USART1_CR3_bit.STOP0 = v; 
    USART1_CR3_bit.STOP1 = v>>1; 
}

//-----------------------------------------------------------------------------------------------------
// Transmission
//-----------------------------------------------------------------------------------------------------
void DriverUART::operator <<(const char ch)
{
    while(!(USART1_SR_bit.TXE));
    USART1_DR = ch;
}

void DriverUART::operator <<(const char *pch)
{
    select_direction(TRANSMITION);

    while(*pch) {
        while(!(USART1_SR_bit.TXE));
        USART1_DR = *pch++;
    }
}

void DriverUART::transmit(u8 * source, u16 size)
{
    while(DMAChannelTX.is_busy()) ;

    select_direction(TRANSMITION);

    __disable_interrupt();
    DMAChannelTX.global_disable();
    DMAChannelTX.channel_disable();

    DMAChannelTX.set_number_of_transfers(size);
    DMAChannelTX.set_memory0_addr(source);

    DMAChannelTX.global_enable();
    DMAChannelTX.channel_enable();
    __enable_interrupt();
}

void DriverUART::transmit(u16 * source, u16 size)
{
    //select_direction(TRANSMITION);
}

void DriverUART::transmit(u16 size)
{
    while(DMAChannelTX.is_busy()) ;

    select_direction(TRANSMITION);

    __disable_interrupt();
    DMAChannelTX.global_disable();
    DMAChannelTX.channel_disable();

    DMAChannelTX.set_number_of_transfers(size);
    DMAChannelTX.set_memory0_addr(UartBuffer);

    DMAChannelTX.global_enable();
    DMAChannelTX.channel_enable();
    __enable_interrupt();
}

void DriverUART::reception_handshake()
{
    __disable_interrupt();
    DMAChannelRX.global_disable();
    DMAChannelRX.channel_disable();

    received_size = DMAChannelRX.get_amount_of_last_transation();
    DMAChannelRX.set_number_of_transfers(sizeof(UartBuffer));

    DMAChannelRX.global_enable();
    DMAChannelRX.channel_enable();
    __enable_interrupt();
}

//-----------------------------------------------------------------------------------------------------
// Reception
//-----------------------------------------------------------------------------------------------------
__interrupt void DriverUART::UART1_reception()
{
    u8 tmp = USART1_DR;

    if(USART1_SR & 0x0F){ // errors
        USART1_SR_bit.FE = 0;
        USART1_SR_bit.OR = 0;
        USART1_SR_bit.PE = 0;
        USART1_SR_bit.NF = 0;
        USART1_SR_bit.IDLE = 0;
    }
    else if (USART1_SR_bit.IDLE){
        receive_handle();
        USART1_SR_bit.IDLE = 0;
    }
}




