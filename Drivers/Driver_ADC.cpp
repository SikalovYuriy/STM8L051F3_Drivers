/* =================================================================================
 File name:     Driver_ADC.cpp                                
 Autor:         Sikalov Yuriy
 Description:   The ADC driver.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 15-05-2014 Version 1.00
-----------------------------------------------------------------------------------*/

#include "Driver_ADC.hpp"
#include "intrinsics.h"     //Здесь описана функция __enable_interrupt()

void Delay()
{
    u16 tmp = 300;
    while(tmp--);
}

DriverADC::DriverADC()
{
    CLK_PCKENR2_bit.PCKEN20 = 1; // ADC clock enable
    ADC1_CR1_bit.ADON = 1; // ADC enable

    Delay();
    DMA_disable();
}
/*
DriverADC::~DriverADC()
{
    ADC1_CR1_bit.ADON = 0; // ADC disable
}
*/
//-----------------------------------------------------------------------------------
void DriverADC::start_conversion()
{
    ADC1_CR1_bit.START = 1;
}

//-----------------------------------------------------------------------------------
u16 DriverADC::get_conversion_value()
{
    u16 result;

    result = ADC1_DRH << 8;
    result |= ADC1_DRL;

    return result;
}

//-----------------------------------------------------------------------------------
bool DriverADC::cheak_status_flag(StatusFlag v)
{
    return v & ADC1_SR;
}

//-----------------------------------------------------------------------------------
void DriverADC::channel_enable(ChannelSelection v)
{
    switch(static_cast<u16>(v) >> 8) {
    case 0:
        ADC1_SQR1 |= v;
        break;

    case 1:
        ADC1_SQR2 |= v;
        break;

    case 2:
        ADC1_SQR3 |= v;
        break;

    case 3:
        ADC1_SQR4 |= v;
        break;
    }
}


//-----------------------------------------------------------------------------------
void DriverADC::group0_channel_enable(Group0ChannelSelection v)
{
    ADC1_SQR1 |= v;
}

//-----------------------------------------------------------------------------------
void DriverADC::group1_channel_enable(Group1ChannelSelection v)
{
    ADC1_SQR2 |= v;
}

//-----------------------------------------------------------------------------------
void DriverADC::group2_channel_enable(Group2ChannelSelection v)
{
    ADC1_SQR3 |= v;
}

//-----------------------------------------------------------------------------------
void DriverADC::group3_channel_enable(Group3ChannelSelection v)
{
    ADC1_SQR4 |= v;
}

//-----------------------------------------------------------------------------------
void DriverADC::channel_disable(ChannelSelection v)
{
    switch(static_cast<u16>(v) >> 8) {
    case 0:
        ADC1_SQR1 &= ~v;
        break;

    case 1:
        ADC1_SQR2 &= ~v;
        break;

    case 2:
        ADC1_SQR3 &= ~v;
        break;

    case 3:
        ADC1_SQR4 &= ~v;
        break;
    }
}

//-----------------------------------------------------------------------------------
void DriverADC::group0_channel_disable(Group0ChannelSelection v)
{
    ADC1_SQR1 &= ~v;
}

//-----------------------------------------------------------------------------------
void DriverADC::group1_channel_disable(Group1ChannelSelection v)
{
    ADC1_SQR2 &= ~v;
}

//-----------------------------------------------------------------------------------
void DriverADC::group2_channel_disable(Group2ChannelSelection v)
{
    ADC1_SQR3 &= ~v;
}

//-----------------------------------------------------------------------------------
void DriverADC::group3_channel_disable(Group3ChannelSelection v)
{
    ADC1_SQR4 &= ~v;
}

//-----------------------------------------------------------------------------------
void DriverADC::sampling_time_cofiguration(ChannelGroup cg, SamplingTime st)
{
    if(cg == ChannelGroup_Fast) {
        ADC1_CR3_bit.SMTP2 = st;
    }
    else{
        ADC1_CR2_bit.SMTP1 = st;
    }
}

//-----------------------------------------------------------------------------------
void DriverADC::schmitt_trigger_enable(ChannelSelection v)
{
    switch(static_cast<u16>(v) >> 8) {
    case 0:
        ADC1_TRIGR1 &= ~v;
        break;

    case 1:
        ADC1_TRIGR2 &= ~v;
        break;

    case 2:
        ADC1_TRIGR3 &= ~v;
        break;

    case 3:
        ADC1_TRIGR4 &= ~v;
        break;
    }
}

//-----------------------------------------------------------------------------------
void DriverADC::schmitt_trigger_disable(ChannelSelection v)
{

    switch(static_cast<u16>(v) >> 8) {
    case 0:
        ADC1_TRIGR1 |= v;
        break;

    case 1:
        ADC1_TRIGR2 |= v;
        break;

    case 2:
        ADC1_TRIGR3 |= v;
        break;

    case 3:
        ADC1_TRIGR4 |= v;
        break;
    }
}

//-----------------------------------------------------------------------------------
void DriverADC::interrupt_enable(InterruptSelection isel)
{
    ADC1_CR1 |= isel;
}

//-----------------------------------------------------------------------------------
void DriverADC::interrupt_disable(InterruptSelection isel)
{
    ADC1_CR1 &= ~isel;
}

//-----------------------------------------------------------------------------------
bool DriverADC::interrupt_read_pending_flag(StatusFlag sf)
{
    return ADC1_SR & sf;
}

//-----------------------------------------------------------------------------------
void DriverADC::interrupt_clear_pending_flag(StatusFlag sf)
{
    ADC1_SR &= ~sf;
}

//-----------------------------------------------------------------------------------
void DriverADC::DMA_enable(u8 *buffer, u16 size)
{
    ADC1_SQR1_bit.DMAOFF = 0;

    DMAChannelEOC.global_disable();
    DMAChannelEOC.channel_disable();
    DMAChannelEOC.set_periph_addr(&ADC1_DRH);
    DMAChannelEOC.set_memory0_addr(buffer);
    DMAChannelEOC = EnumDMA::DATABLOCK_16bit;
    DMAChannelEOC = EnumDMA::PRIORITY_VERYHIGH;
    DMAChannelEOC = EnumDMA::MEMPNT_INCREMENT;
    DMAChannelEOC = EnumDMA::PHERIPH_TO_MEMORY;
    DMAChannelEOC = EnumDMA::CIRCULAR_ENABLE;
    DMAChannelEOC.set_number_of_transfers(size);
    DMAChannelEOC.channel_enable();
    DMAChannelEOC.global_enable();
}

//-----------------------------------------------------------------------------------
void DriverADC::DMA_enable(u16 *buffer, u16 size)
{
    ADC1_SQR1_bit.DMAOFF = 0;
    number_of_transfers = size;

    DMAChannelEOC.global_disable();
    DMAChannelEOC.channel_disable();
    DMAChannelEOC.set_periph_addr(&ADC1_DRH);
    DMAChannelEOC.set_memory0_addr(reinterpret_cast<u8*>(buffer));
    DMAChannelEOC = EnumDMA::DATABLOCK_16bit;
    DMAChannelEOC = EnumDMA::PRIORITY_VERYHIGH;
    DMAChannelEOC = EnumDMA::MEMPNT_INCREMENT;
    DMAChannelEOC = EnumDMA::PHERIPH_TO_MEMORY;
    DMAChannelEOC = EnumDMA::CIRCULAR_ENABLE;
    DMAChannelEOC.set_number_of_transfers(size);
    DMAChannelEOC.channel_enable();
    DMAChannelEOC.global_enable();
}

//-----------------------------------------------------------------------------------
void DriverADC::DMA_disable()
{
    ADC1_SQR1_bit.DMAOFF = 1;

    DMAChannelEOC.channel_disable();
}

//-----------------------------------------------------------------------------------
void DriverADC::DMA_interrupt_enable()
{
    DMAChannelEOC.interrupt_enable(EnumDMA::INTERRUPT_TRANSACTION_COMPLETE);
}

//-----------------------------------------------------------------------------------
void DriverADC::DMA_interrupt_clear_pending_flag()
{
    DMAChannelEOC.interrupt_clear_pending_flag(EnumDMA::INTERRUPT_TRANSACTION_COMPLETE);
}

//-----------------------------------------------------------------------------------
void DriverADC::DMA_priority(enum EnumDMA::ChannelPriority ch)
{
    DMAChannelEOC = ch;
}

//-----------------------------------------------------------------------------------
void DriverADC::DMA_reset()
{
    __disable_interrupt();
    DMAChannelEOC.global_disable();
    DMAChannelEOC.channel_disable();
    DMAChannelEOC.set_number_of_transfers(number_of_transfers);
    DMAChannelEOC.channel_enable();
    DMAChannelEOC.global_enable();
    __enable_interrupt();
}

//-----------------------------------------------------------------------------------
void DriverADC::operator =(ConversionMode conv_mode)
{
    ADC1_CR1_bit.CONT = conv_mode;
}

//-----------------------------------------------------------------------------------
void DriverADC::operator =(Resolution res)
{
    ADC1_CR1_bit.RES = res;
}

//-----------------------------------------------------------------------------------
void DriverADC::operator =(Prescaler prescl)
{
    ADC1_CR2_bit.PRESC = prescl;
}

//-----------------------------------------------------------------------------------
void DriverADC::operator =(StartEventSelection v)
{
    ADC1_CR2_bit.EXTSEL0 = v & 1;
    ADC1_CR2_bit.EXTSEL1 = v >> 1;
}

//-----------------------------------------------------------------------------------
void DriverADC::operator =(TriggerSensivity v)
{
    ADC1_CR2_bit.TRIG_EDGE0 = v & 1;
    ADC1_CR2_bit.TRIG_EDGE1 = v >> 1;
}

//-----------------------------------------------------------------------------------
void DriverADC::operator =(InternalReference v)
{
    ADC1_TRIGR1_bit.VREFINTON = v;
}


