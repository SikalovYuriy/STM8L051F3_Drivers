/* =================================================================================
 File name:     Driver_Timer.hpp                                
 Creation date: 26-05-2014
 Description:   The timer driver.
================================================================================== */

#ifndef _DRIVERTIMER_H_
#define _DRIVERTIMER_H_

#include "type.h"
#include "iostm8l051f3.h"

struct EnumTimer
{
    enum TimerSelection{
        TIMER2 = 0x5250,
        TIMER3 = 0x5280
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
        VECTOR_TIMER2_UPDATE            = TIM2_OVR_UIF_vector,   
        VECTOR_TIMER2_BREAK             = TIM2_CAPCOM_BIF_vector,
        VECTOR_TIMER2_TRIGGER           = TIM2_CAPCOM_TIF_vector,
        VECTOR_TIMER2_CAPTURE_COMPARE_1 = TIM2_CAPCOM_CC1IF_vector, 
        VECTOR_TIMER2_CAPTURE_COMPARE_2 = TIM2_CAPCOM_CC2IF_vector,

        VECTOR_TIMER3_UPDATE            = TIM3_OVR_UIF_vector,   
        VECTOR_TIMER3_BREAK             = TIM3_CAPCOM_BIF_vector,
        VECTOR_TIMER3_TRIGGER           = TIM3_CAPCOM_TIF_vector,
        VECTOR_TIMER3_CAPTURE_COMPARE_1 = TIM3_CAPCOM_CC1IF_vector, 
        VECTOR_TIMER3_CAPTURE_COMPARE_2 = TIM3_CAPCOM_CC2IF_vector 
    };                                                               
};

template <EnumTimer::TimerSelection TimerSel>
class DriverTimer
{
private:
    
    struct  CR1_BITS {        // bits   description
        unsigned char CEN         : 1;
        unsigned char UDIS        : 1;
        unsigned char URS         : 1;
        unsigned char OPM         : 1;
        unsigned char DIR         : 1;
        unsigned char CMS         : 2;
        unsigned char ARPE        : 1;
    };
    
    union CR1_REG {
       unsigned char    all;
       struct CR1_BITS  bit;
    };

    struct  CR2_BITS {        // bits   description
        unsigned char             : 3;
        unsigned char CCDS        : 1;
        unsigned char MMS         : 3;
        unsigned char TI1S        : 1;
    };
    
    union CR2_REG {
       unsigned char    all;
       struct CR2_BITS  bit;
    };


    /**
      * @brief 16 bit timer :TIM2, TIM3 & TIM5
      */
    struct TIM_struct
    {
      volatile CR1_REG          CR1;    /*!< control register 1   */
      volatile CR2_REG          CR2;    /*!< control register 2   */
      volatile unsigned char    SMCR;   /*!< Synchro mode control register */
      volatile unsigned char    ETR;    /*!< external trigger register */
      volatile unsigned char    DER;    /*!< DMA requests enable register */
      volatile unsigned char    IER;    /*!< interrupt enable register*/
      volatile unsigned char    SR1;    /*!< status register 1   */
      volatile unsigned char    SR2;    /*!< status register 2   */
      volatile unsigned char    EGR;    /*!< event generation register */
      volatile unsigned char    CCMR1;  /*!< CC mode register 1      */
      volatile unsigned char    CCMR2;  /*!< CC mode register 2      */
      volatile unsigned char    CCER1;  /*!< CC enable register 1     */
      volatile unsigned char    CNTRH;  /*!< counterregister  high  */
      volatile unsigned char    CNTRL;  /*!< counterregister  low   */
      volatile unsigned char    PSCR;   /*!< prescaler  register   */
      volatile unsigned char    ARRH;   /*!< auto-reload register high  */
      volatile unsigned char    ARRL;   /*!< auto-reload register low    */
      volatile unsigned char    CCR1H;  /*!< capture/compare register 1 high   */
      volatile unsigned char    CCR1L;  /*!< capture/compare register 1 low     */
      volatile unsigned char    CCR2H;  /*!< capture/compare register 2 high   */
      volatile unsigned char    CCR2L;  /*!< capture/compare register 2 low     */
      volatile unsigned char    BKR;    /*!< Break Register */
      volatile unsigned char    OISR;   /*!< Output idle register */
    };                          

    static u32  sys_clock_div_1000;

public:
    DriverTimer(u32 system_clock_frequence);

    u16 InterruptVector;

    void set_period_usec(u32 time_us, EnumTimer::SystemClockPrescaler scp);
    void set_period_msec(u32 time_ms, EnumTimer::SystemClockPrescaler scp);
    void set_counter(u16 counter_val, EnumTimer::SystemClockPrescaler scp);
    u16  calculate_counter_value(u32 time_us, EnumTimer::SystemClockPrescaler scp);
    void one_pulse_mode_enable();
    void one_pulse_mode_disable();
    void counter_start();
    void counter_stop();
    void auto_reload_preload_enable();
    void auto_reload_preload_disable();
    void generate_event(EnumTimer::EventGeneration);

    void interrupt_enable(EnumTimer::InterruptSelection isel);
    void interrupt_disable(EnumTimer::InterruptSelection isel);
    void interrupt_clear_pending_flag(EnumTimer::InterruptSelection);

    void operator= (EnumTimer::SystemClockPrescaler);
    void operator= (EnumTimer::AlignedModeSelection);
    void operator= (EnumTimer::CounterDirection);
    void operator= (EnumTimer::MasterModeSelection);
    void operator= (EnumTimer::UpdateRequestSource);
};


#define __TIMER ((TIM_struct*)TimerSel)

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
u32  DriverTimer<TimerSel>::sys_clock_div_1000 = 1000;

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
DriverTimer<TimerSel>::DriverTimer(u32 system_clock_frequence)
{
    switch(TimerSel) {
    case EnumTimer::TIMER2:
        CLK_PCKENR1_bit.PCKEN10 = 1; // clock enable
        break;

    case EnumTimer::TIMER3:
        CLK_PCKENR1_bit.PCKEN11 = 1; // clock enable
        break;
    }

    sys_clock_div_1000 = system_clock_frequence / 1000;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::set_period_usec(u32 time_us, EnumTimer::SystemClockPrescaler scp)
{
    u16 ARR;

    __TIMER->PSCR = scp;

    if(time_us > 1000000UL) {
        time_us /= 10000;
        ARR = (u16)((((u32)sys_clock_div_1000 * time_us) >> scp) *  10);
    }
    else if(time_us > 100000UL) {
        time_us /= 1000;
        ARR = (u16)((((u32)sys_clock_div_1000 * time_us) >> scp) /  1);
    }
    else if(time_us > 10000UL) {
        time_us /= 100;
        ARR = (u16)((((u32)sys_clock_div_1000 * time_us) >> scp) /  10);
    }
    else if(time_us > 1000UL) {
        time_us /= 10;
        ARR = (u16)((((u32)sys_clock_div_1000 * time_us) >> scp) /  100);
    }
    else{
        ARR = (u16)((((u32)sys_clock_div_1000 * time_us) >> scp) /  1000);
    }

    __TIMER->ARRH = ARR >> 8;
    __TIMER->ARRL = ARR;

    __TIMER->CR1.all |= MASK_TIM2_CR1_UDIS;
    generate_event(EnumTimer::Generate_Update);
    __TIMER->CR1.all &= ~MASK_TIM2_CR1_UDIS;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::set_period_msec(u32 time_ms, EnumTimer::SystemClockPrescaler scp)
{
    u16 ARR;

    __TIMER->PSCR = scp;

    ARR = (u16)(((u32)sys_clock_div_1000 * time_ms) >> scp);

    __TIMER->ARRH = ARR >> 8;
    __TIMER->ARRL = ARR;

    __TIMER->CR1.all |= MASK_TIM2_CR1_UDIS;
    generate_event(EnumTimer::Generate_Update);
    __TIMER->CR1.all &= ~MASK_TIM2_CR1_UDIS;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::set_counter(u16 counter_val, EnumTimer::SystemClockPrescaler scp)
{
    __TIMER->PSCR = scp;
    __TIMER->ARRH = counter_val >> 8;
    __TIMER->ARRL = counter_val;

    __TIMER->CR1.all |= MASK_TIM2_CR1_UDIS;
    __TIMER->EGR |= EnumTimer::Generate_Update;
    __TIMER->CR1.all &= ~MASK_TIM2_CR1_UDIS;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
u16 DriverTimer<TimerSel>::calculate_counter_value(u32 time_us,  EnumTimer::SystemClockPrescaler scp)
{
    u16 ARR;

    if(time_us > 1000000UL) {
        time_us /= 10000;
        ARR = (u16)((((u32)sys_clock_div_1000 * time_us) >> scp) *  10);
    }
    else if(time_us > 100000UL) {
        time_us /= 1000;
        ARR = (u16)((((u32)sys_clock_div_1000 * time_us) >> scp) /  1);
    }
    else if(time_us > 10000UL) {
        time_us /= 100;
        ARR = (u16)((((u32)sys_clock_div_1000 * time_us) >> scp) /  10);
    }
    else if(time_us > 1000UL) {
        time_us /= 10;
        ARR = (u16)((((u32)sys_clock_div_1000 * time_us) >> scp) /  100);
    }
    else{
        ARR = (u16)((((u32)sys_clock_div_1000 * time_us) >> scp) /  1000);
    }

    return ARR;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::one_pulse_mode_enable()
{
    __TIMER->CR1.bit.OPM = 0;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::one_pulse_mode_disable()
{
    __TIMER->CR1.bit.OPM = 1;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::counter_start()
{
    __TIMER->CR1.bit.CEN = 1;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::counter_stop()
{
    __TIMER->CR1.bit.CEN = 0;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::auto_reload_preload_enable()
{
    __TIMER->CR1.bit.ARPE = 1;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::auto_reload_preload_disable()
{
    __TIMER->CR1.bit.ARPE = 0;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::generate_event(EnumTimer::EventGeneration eg)
{
    __TIMER->EGR |= eg;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::interrupt_enable(EnumTimer::InterruptSelection isel)
{
    __TIMER->IER |= isel;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::interrupt_disable(EnumTimer::InterruptSelection isel)
{
    __TIMER->IER &= ~isel;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::interrupt_clear_pending_flag(EnumTimer::InterruptSelection isel)
{
    __TIMER->SR1 &= ~isel;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::operator =(EnumTimer::SystemClockPrescaler scp)
{
    __TIMER->PSCR = (1 << scp);
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::operator =(EnumTimer::AlignedModeSelection ams)
{
    __TIMER->CR1.bit.CMS = ams;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::operator =(EnumTimer::CounterDirection cd)
{
    __TIMER->CR1.bit.DIR = cd;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::operator =(EnumTimer::MasterModeSelection mms)
{
    __TIMER->CR2.bit.MMS = mms;
}

//-----------------------------------------------------------------------------------
template <EnumTimer::TimerSelection TimerSel>
void DriverTimer<TimerSel>::operator =(EnumTimer::UpdateRequestSource urs)
{
    __TIMER->CR1.bit.URS = urs;
}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------


#endif

