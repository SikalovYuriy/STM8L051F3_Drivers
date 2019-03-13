/* =================================================================================
 File name:     Driver_Timer2.hpp                                
 Autor:         Sikalov Yuriy
 Description:   The Timer 2 driver.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 13-05-2014 Version 1.00
-----------------------------------------------------------------------------------*/

#ifndef _DRIVERTIMER2_H_
#define _DRIVERTIMER2_H_

#include "type.h"
#include "iostm8l051f3.h"

class DriverTimer2 
{
private:
    //enum TimerSelection;
    u32  sys_clock_div_1000;
    //TimerSelection CurrentTimer;

public:
    enum TimerSelection{
        TIMER2,
        TIMER3,
        TIMER5
    };

    enum SystemClockPrescaler{
        Clock_prescaler_1,
        Clock_prescaler_2,
        Clock_prescaler_4,
        Clock_prescaler_8,
        Clock_prescaler_16,
        Clock_prescaler_32,
        Clock_prescaler_64,
        Clock_prescaler_128,
    };

    enum AlignedModeSelection{
        Aligned_mode_Edge,
        Aligned_mode_Center1,
        Aligned_mode_Center2,
        Aligned_mode_Center3,
    };

    enum CounterDirection{
        Counter_direction_Up,
        Counter_direction_Down
    };

    enum MasterModeSelection{
        MasterMode_Reset,
        MasterMode_Enable,
        MasterMode_Update
    };

    enum EventGeneration{
        Generate_Update             = 0x01,
        Generate_Capture_compare_1  = 0x02,
        Generate_Capture_compare_2  = 0x04,
        Generate_Trigger            = 0x40,
        Generate_Break              = 0x80
    };

    enum UpdateRequestSource{
        UpdateSource_All,
        UpdateSource_Only_overflow_or_underflow
    };

    enum InterruptSelection{
        Interrupt_Update            = 0x01,
        Interrupt_Capture_compare_1 = 0x02,
        Interrupt_Capture_compare_2 = 0x04,
        Interrupt_Trigger           = 0x40,
        Interrupt_Break             = 0x80
    };

    enum InterruptVectors{
        VECTOR_Update               = 0x15,           
        VECTOR_Capture_compare_1    = 0x16,
        VECTOR_Capture_compare_2    = 0x16,
        VECTOR_Trigger              = 0x16,          
    };        

    DriverTimer2(u32 system_clock_frequence);

    void set_period_usec(u32 time_us, SystemClockPrescaler scp);
    void one_pulse_mode_enable();
    void one_pulse_mode_disable();
    void counter_start();
    void counter_stop();
    void auto_reload_preload_enable();
    void auto_reload_preload_disable();
    void generate_event(EventGeneration);

    void interrupt_enable(InterruptSelection isel);
    void interrupt_disable(InterruptSelection isel);
    void interrupt_clear_pending_flag(InterruptSelection);

    void operator= (SystemClockPrescaler);
    void operator= (AlignedModeSelection);
    void operator= (CounterDirection);
    void operator= (MasterModeSelection);
    void operator= (UpdateRequestSource);
};

#endif

