/* =================================================================================
 File name:     Driver_ADC.hpp                                
 Autor:         Sikalov Yuriy
 Description:   The ADC driver.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 15-05-2014 Version 1.00
-----------------------------------------------------------------------------------*/

#ifndef _DRIVERADC_H_
#define _DRIVERADC_H_

#include "type.h"
#include "iostm8l051f3.h"
#include "Driver_DMA.hpp"
#include "Driver_Interrupt.hpp"

class DriverADC 
{
private:
    DriverDMA<EnumDMA::DMA1,EnumDMA::CHANNEL0> DMAChannelEOC;
    u8 number_of_transfers;
public:
    enum ConversionMode{
        CONVMODE_SINGLE,
        CONVMODE_CONTINUOUS
    };

    enum Resolution{
        RESOLUTION_12BIT,
        RESOLUTION_10BIT,
        RESOLUTION_8BIT,
        RESOLUTION_6BIT
    };

    enum Prescaler{
        SYSCLOCK_DIVIDEDby1,
        SYSCLOCK_DIVIDEDby2
    };

    enum StartEventSelection{
        START_BY_SOFTWARE,
        START_BY_PIN_PA6_OR_PD0,
        START_BY_TIMER1,
        START_BY_TIMER2,
    };

    enum TriggerSensivity{
        TRIGGER_Rising_edge = 1,
        TRIGGER_Falling_edge,
        TRIGGER_Both_edge
    };

    enum InternalReference{
        Internal_reference_disable,
        Internal_reference_enable
        
    };

    enum ChannelSelection
    {
        Channel_0           = ((u16)0x0301), /*!< Channel 00 */
        Channel_1           = ((u16)0x0302), /*!< Channel 01 */
        Channel_2           = ((u16)0x0304), /*!< Channel 02 */
        Channel_3           = ((u16)0x0308), /*!< Channel 03 */
        Channel_4           = ((u16)0x0310), /*!< Channel 04 */
        Channel_5           = ((u16)0x0320), /*!< Channel 05 */
        Channel_6           = ((u16)0x0340), /*!< Channel 06 */
        Channel_7           = ((u16)0x0380), /*!< Channel 07 */
        
        Channel_8           = ((u16)0x0201), /*!< Channel 08 */
        Channel_9           = ((u16)0x0202), /*!< Channel 09 */
        Channel_10          = ((u16)0x0204), /*!< Channel 10 */
        Channel_11          = ((u16)0x0208), /*!< Channel 11 */
        Channel_12          = ((u16)0x0210), /*!< Channel 12 */
        Channel_13          = ((u16)0x0220), /*!< Channel 13 */
        Channel_14          = ((u16)0x0240), /*!< Channel 14 */
        Channel_15          = ((u16)0x0280), /*!< Channel 15 */
        
        Channel_16          = ((u16)0x0101), /*!< Channel 16 */
        Channel_17          = ((u16)0x0102), /*!< Channel 17 */
        Channel_18          = ((u16)0x0104), /*!< Channel 18 */
        Channel_19          = ((u16)0x0108), /*!< Channel 19 */
        Channel_20          = ((u16)0x0110), /*!< Channel 20 */
        Channel_21          = ((u16)0x0120), /*!< Channel 21 */
        Channel_22          = ((u16)0x0140), /*!< Channel 22 */
        Channel_23          = ((u16)0x0180), /*!< Channel 23 */
        
        Channel_24          = ((u16)0x0001), /*!< Channel 24 */
        Channel_25          = ((u16)0x0002), /*!< Channel 25 */
        Channel_26          = ((u16)0x0004), /*!< Channel 26 */
        Channel_27          = ((u16)0x0008), /*!< Channel 27 */
        
        Channel_Vrefint     = ((u16)0x0010), /*!< Vrefint Channel  */
        //Channel_TempSensor  = ((u16)0x0020), /*!< Temperature sensor Channel  */
        
        /* combination*/
        Channel_00To07      = ((u16)0x03FF), /*!<select from channel00 to channel07 */
        Channel_08To15      = ((u16)0x02FF), /*!<select from channel08 to channel15 */
        Channel_16To23      = ((u16)0x01FF), /*!<select from channel16 to channel23 */
        Channel_24To27      = ((u16)0x00FF)  /*!<select from channel24 to channel27 */
    };

    enum Group0ChannelSelection{
        Group0_Channel_24          = ((u16)0x0001), /*!< Channel 24 */
        Group0_Channel_25          = ((u16)0x0002), /*!< Channel 25 */
        Group0_Channel_26          = ((u16)0x0004), /*!< Channel 26 */
        Group0_Channel_27          = ((u16)0x0008), /*!< Channel 27 */
        
        Group0_Channel_Vrefint     = ((u16)0x0010), /*!< Vrefint Channel  */
        //Channel_group0_TempSensor  = ((u16)0x0020), /*!< Temperature sensor Channel  */
    };

    enum Group1ChannelSelection{
        Group1_Channel_16          = ((u16)0x0101), /*!< Channel 16 */
        Group1_Channel_17          = ((u16)0x0102), /*!< Channel 17 */
        Group1_Channel_18          = ((u16)0x0104), /*!< Channel 18 */
        Group1_Channel_19          = ((u16)0x0108), /*!< Channel 19 */
        Group1_Channel_20          = ((u16)0x0110), /*!< Channel 20 */
        Group1_Channel_21          = ((u16)0x0120), /*!< Channel 21 */
        Group1_Channel_22          = ((u16)0x0140), /*!< Channel 22 */
        Group1_Channel_23          = ((u16)0x0180), /*!< Channel 23 */
    };

    enum Group2ChannelSelection{
        Group2_Channel_8           = ((u16)0x0201), /*!< Channel 08 */
        Group2_Channel_9           = ((u16)0x0202), /*!< Channel 09 */
        Group2_Channel_10          = ((u16)0x0204), /*!< Channel 10 */
        Group2_Channel_11          = ((u16)0x0208), /*!< Channel 11 */
        Group2_Channel_12          = ((u16)0x0210), /*!< Channel 12 */
        Group2_Channel_13          = ((u16)0x0220), /*!< Channel 13 */
        Group2_Channel_14          = ((u16)0x0240), /*!< Channel 14 */
        Group2_Channel_15          = ((u16)0x0280), /*!< Channel 15 */
    };  

    enum Group3ChannelSelection{
        Group3_Channel_0           = ((u16)0x0301), /*!< Channel 00 */
        Group3_Channel_1           = ((u16)0x0302), /*!< Channel 01 */
        Group3_Channel_2           = ((u16)0x0304), /*!< Channel 02 */
        Group3_Channel_3           = ((u16)0x0308), /*!< Channel 03 */
        Group3_Channel_4           = ((u16)0x0310), /*!< Channel 04 */
        Group3_Channel_5           = ((u16)0x0320), /*!< Channel 05 */
        Group3_Channel_6           = ((u16)0x0340), /*!< Channel 06 */
        Group3_Channel_7           = ((u16)0x0380), /*!< Channel 07 */
    };

    enum SamplingTime
    {
        SamplingTime_4Cycles   = ((u16)0x00), /*!< Sampling Time Cycles is 4 */
        SamplingTime_9Cycles   = ((u16)0x01), /*!< Sampling Time Cycles is 9 */
        SamplingTime_16Cycles  = ((u16)0x02), /*!< Sampling Time Cycles is 16 */
        SamplingTime_24Cycles  = ((u16)0x03), /*!< Sampling Time Cycles is 24 */
        SamplingTime_48Cycles  = ((u16)0x04), /*!< Sampling Time Cycles is 48 */
        SamplingTime_96Cycles  = ((u16)0x05), /*!< Sampling Time Cycles is 96 */
        SamplingTime_192Cycles = ((u16)0x06), /*!< Sampling Time Cycles is 192 */
        SamplingTime_384Cycles = ((u16)0x07)  /*!< Sampling Time Cycles is 384 */
    };

    enum ChannelGroup
    {
        ChannelGroup_Slow,
        ChannelGroup_Fast
    };

    enum StatusFlag
    {
        StatusFlag_End_of_conversion    = 0x01,
        StatusFlag_Analog_watchdog      = 0x02,
        StatusFlag_Overrun              = 0x04
    };

    enum InterruptSelection{
        Interrupt_End_of_conversion     = 0x08,
        Interrupt_Analog_watchdog       = 0x10,
        Interrupt_Overrun               = 0x80
    };

    enum InterruptVectors{
        VECTOR_OVERRUN                  = 0x14,
        VECTOR_ANALOG_WATCHDOG          = 0x14,
        VECTOR_END_OF_CONVERSATION      = 0x14,
        VECTOR_DMA_TRANSACTION_COMPLETE = DMA1_CH0_TC_vector
    };

    DriverADC();
    //~DriverADC();

    void start_conversion();
    u16  get_conversion_value();
    bool cheak_status_flag(StatusFlag);
    void channel_enable(ChannelSelection);
    void group0_channel_enable(Group0ChannelSelection);
    void group1_channel_enable(Group1ChannelSelection);
    void group2_channel_enable(Group2ChannelSelection);
    void group3_channel_enable(Group3ChannelSelection);
    void channel_disable(ChannelSelection);
    void group0_channel_disable(Group0ChannelSelection);
    void group1_channel_disable(Group1ChannelSelection);
    void group2_channel_disable(Group2ChannelSelection);
    void group3_channel_disable(Group3ChannelSelection);
    void sampling_time_cofiguration(ChannelGroup, SamplingTime);
    void schmitt_trigger_enable(ChannelSelection);
    void schmitt_trigger_disable(ChannelSelection);

    void interrupt_enable(InterruptSelection);
    void interrupt_disable(InterruptSelection);
    bool interrupt_read_pending_flag(StatusFlag);
    void interrupt_clear_pending_flag(StatusFlag);

    void DMA_enable(u8 *buffer, u16 size);
    void DMA_enable(u16 *buffer, u16 size);
    void DMA_disable();
    void DMA_interrupt_enable();
    void DMA_interrupt_disable();
    void DMA_interrupt_clear_pending_flag();
    void DMA_priority(enum EnumDMA::ChannelPriority);
    void DMA_reset();

    void operator= (ConversionMode);
    void operator= (Resolution);
    void operator= (Prescaler);
    void operator= (StartEventSelection);
    void operator= (TriggerSensivity);
    void operator= (InternalReference);
    
};

#endif

