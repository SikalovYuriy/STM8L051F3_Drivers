/* =================================================================================
 File name:     Driver_Timer2.cpp                                
 Autor:         Sikalov Yuriy
 Description:   The Timer 2 driver.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-05-2014 Version 1.00
-----------------------------------------------------------------------------------*/

#include "Driver_Timer2.hpp"

//-----------------------------------------------------------------------------------
DriverTimer2::DriverTimer2(u32 system_clock_frequence)
{
    CLK_PCKENR1_bit.PCKEN10 = 1; // clock enable

    sys_clock_div_1000 = system_clock_frequence / 1000;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::set_period_usec(u32 time_us, SystemClockPrescaler scp)
{
    u16 ARR;

    TIM2_PSCR = scp;

    ARR = (u16)((u32)sys_clock_div_1000 * time_us / (((u32)1 << scp) * 1000));

    TIM2_ARRH = ARR >> 8;
    TIM2_ARRL = ARR;

    TIM2_CR1_bit.UDIS = 1;
    generate_event(Generate_Update);
    TIM2_CR1_bit.UDIS = 0;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::one_pulse_mode_enable()
{
    TIM2_CR1_bit.OPM = 0;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::one_pulse_mode_disable()
{
    TIM2_CR1_bit.OPM = 1;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::counter_start()
{
    TIM2_CR1_bit.CEN = 1;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::counter_stop()
{
    TIM2_CR1_bit.CEN = 0;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::auto_reload_preload_enable()
{
    TIM2_CR1_bit.ARPE = 1;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::auto_reload_preload_disable()
{
    TIM2_CR1_bit.ARPE = 0;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::generate_event(EventGeneration eg)
{
    TIM2_EGR |= eg;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::interrupt_enable(InterruptSelection isel)
{
    TIM2_IER |= isel;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::interrupt_disable(InterruptSelection isel)
{
    TIM2_IER &= ~isel;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::interrupt_clear_pending_flag(InterruptSelection isel)
{
    TIM2_SR1 &= ~isel;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::operator =(SystemClockPrescaler scp)
{
    TIM2_PSCR = (1 << scp);
}

//-----------------------------------------------------------------------------------
void DriverTimer2::operator =(AlignedModeSelection ams)
{
    TIM2_CR1_bit.CMS = ams;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::operator =(CounterDirection cd)
{
    TIM2_CR1_bit.DIR = cd;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::operator =(MasterModeSelection mms)
{
    TIM2_CR2_bit.MMS = mms;
}

//-----------------------------------------------------------------------------------
void DriverTimer2::operator =(UpdateRequestSource urs)
{
    TIM2_CR1_bit.URS = urs;
}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------




