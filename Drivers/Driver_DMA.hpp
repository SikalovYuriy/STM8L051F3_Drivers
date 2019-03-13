/* =================================================================================
 File name:     Driver_DMA.hpp                                
 Creation date: 26-05-2014
 Description:   The timer driver.
================================================================================== */

#ifndef _DRIVERDMA_H_
#define _DRIVERDMA_H_

#include "type.h"
#include "iostm8l051f3.h"

#define __DMA ((DMA_struct*) DMA)
#define __DMACHANNEL ((DMA_Channel_struct*) ((u32)DMA + (u32)DMAChannel))

struct EnumDMA
{
    enum DMASel{
        DMA1        = 0x5070
    };

    enum ChannelSel{
        CHANNEL0    = 0x05, //0x5075,
        CHANNEL1    = 0x0F, //0x507F,
        CHANNEL2    = 0x19, //0x5089,
        CHANNEL3    = 0x23  //0x5093
    };

    enum DataBlock{
        DATABLOCK_8bit, DATABLOCK_16bit
    };

    enum ChannelPriority{
        PRIORITY_LOW,   
        PRIORITY_MEDIUM,
        PRIORITY_HIGH,
        PRIORITY_VERYHIGH
    };

    enum MemoryPointerMode{
        MEMPNT_DECREMENT,
        MEMPNT_INCREMENT
    };

    enum CircularBufferMode{
        CIRCULAR_DISABLE,
        CIRCULAR_ENABLE
    };

    enum TransferType{ // только для канала 3
        TRANS_TYPE_PHERIPH_TO_MEMORY,
        TRANS_TYPE_MEMORY0_TO_MEMORY1
    };

    enum TransferDirection{
        PHERIPH_TO_MEMORY,
        MEMORY_TO_PHERIPH
    };

    enum InterruptSelection{
        INTERRUPT_HALF_TRANSACTION_COMPLETE,
        INTERRUPT_TRANSACTION_COMPLETE
    };

    enum InterruptVectors{
        VECTOR_DMA1_CHANNEL0_HALF_TRANSACTION_COMPLETE   = DMA1_CH0_HT_vector,
        VECTOR_DMA1_CHANNEL0_TRANSACTION_COMPLETE        = DMA1_CH0_TC_vector,
        VECTOR_DMA1_CHANNEL1_HALF_TRANSACTION_COMPLETE   = DMA1_CH1_HT_vector,
        VECTOR_DMA1_CHANNEL1_TRANSACTION_COMPLETE        = DMA1_CH1_TC_vector,
        VECTOR_DMA1_CHANNEL2_HALF_TRANSACTION_COMPLETE   = DMA1_CH2_HT_vector,
        VECTOR_DMA1_CHANNEL2_TRANSACTION_COMPLETE        = DMA1_CH2_TC_vector,
        VECTOR_DMA1_CHANNEL3_HALF_TRANSACTION_COMPLETE   = DMA1_CH3_HT_vector,
        VECTOR_DMA1_CHANNEL3_TRANSACTION_COMPLETE        = DMA1_CH3_TC_vector,
    };
};

template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
class DriverDMA
{
private:

    //----------------------------------------------------------------------------------------------
    struct  GCSR_BITS {        // bits   description
        unsigned char GEN         : 1;
        unsigned char GB          : 1;
        unsigned char TO          : 6;
    };
    
    union GCSR_REG {
       unsigned char     all;
       struct GCSR_BITS  bit;
    };

    struct  GIR_BITS {        // bits   description
        unsigned char IFC0        : 1;
        unsigned char IFC1        : 1;
        unsigned char IFC2        : 1;
        unsigned char IFC3        : 1;
    };
    
    union GIR_REG {
       unsigned char     all;
       struct GIR_BITS   bit;
    };

    struct DMA_struct
    {
        volatile GCSR_REG  GCSR;     /*!<  Global configuration and status register  */
        volatile GIR_REG   GIR1;     /*!<  Global interrupt register 1  */
    };

    //----------------------------------------------------------------------------------------------
    struct  CCR_BITS {        // bits   description
        unsigned char EN          : 1;
        unsigned char TCIE        : 1;
        unsigned char HTIE        : 1;
        unsigned char DIR         : 1;
        unsigned char CIRC        : 1;
        unsigned char MINCDEC     : 1;
        unsigned char MEM         : 1;
    };

    union CCR_REG {
       unsigned char     all;
       struct CCR_BITS   bit;
    };

    //----------------------------------------------------------------------------------------------
    struct  CSPR_BITS {        // bits   description
        unsigned char             : 1;
        unsigned char TCIF        : 1;
        unsigned char HTIF        : 1;
        unsigned char TSIZE       : 1;
        unsigned char PL          : 2;
        unsigned char PEND        : 1;
        unsigned char BUSY        : 1;
    };
    
    union CSPR_REG {
       unsigned char     all;
       struct CSPR_BITS  bit;
    };


    struct DMA_Channel_struct
    {
        volatile CCR_REG        CCR;       /*!<  CHx Control register  */
        volatile CSPR_REG       CSPR;      /*!<  CHx Status & Priority register  */
        volatile unsigned char  CNDTR;     /*!<  CHx Number of Bytes to Tranfer register  */
        volatile unsigned char  CPARH;     /*!<  Peripheral Address High register  */
        volatile unsigned char  CPARL;     /*!<  Peripheral Address Low register  */
        volatile unsigned char  CM0EAR;    /*!<  Memory 0 Extended Address register  (for channel3)*/
        volatile unsigned char  CM0ARH;    /*!<  Memory 0 Address High register  */
        volatile unsigned char  CM0ARL;    /*!<  Memory 0 Address Low register  */
    };

    u8 number_of_transfers;
    u8 NumChannel;

public:
    DriverDMA();

    void operator= (EnumDMA::DataBlock);
    void operator= (EnumDMA::ChannelPriority);
    void operator= (EnumDMA::MemoryPointerMode);
    void operator= (EnumDMA::CircularBufferMode);
    void operator= (EnumDMA::TransferDirection);
    void operator= (EnumDMA::TransferType); // только для канала 3

    void global_enable();
    void global_disable();

    void channel_enable();
    void channel_disable();

    void set_number_of_transfers(const u16 trans_num);
    void set_periph_addr(u8 volatile* addr);
    void set_memory0_addr(u8* addr);

    // ДОСТУПНИЕ АДРЕСА ОТ 0х0000 до 0х1FFF
    void set_memory1_addr(u8* addr); // только для канала 3

    bool is_busy();

    u16 get_amount_of_last_transation();

    void interrupt_enable(EnumDMA::InterruptSelection);
    void interrupt_disable(EnumDMA::InterruptSelection);
    void interrupt_clear_pending_flag(EnumDMA::InterruptSelection);

};

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
DriverDMA<DMA,DMAChannel>::DriverDMA()
{
    CLK_PCKENR2_bit.PCKEN24 = 1; // DMA clock enable
    __DMA->GCSR.bit.TO = 31;
    NumChannel = (u8)DMAChannel;                                        
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::operator= (EnumDMA::DataBlock db)
{
    __DMACHANNEL->CSPR.bit.TSIZE = db;
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::operator= (EnumDMA::ChannelPriority ch_prior)
{
    __DMACHANNEL->CSPR.bit.PL =  ch_prior;
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::operator= (EnumDMA::MemoryPointerMode pnt_mode)
{
    __DMACHANNEL->CCR.bit.MINCDEC = pnt_mode;
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::operator= (EnumDMA::CircularBufferMode circular_mode)
{
    __DMACHANNEL->CCR.bit.CIRC = circular_mode;
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::operator= (EnumDMA::TransferDirection trans_direction)
{
    __DMACHANNEL->CCR.bit.DIR = trans_direction;
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::operator= (EnumDMA::TransferType trans_type)
{
    __DMACHANNEL->CCR.bit.MEM = trans_type;
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::global_enable()
{
    __DMA->GCSR.bit.GEN = 1; // Global enable of DMA1
}

template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::global_disable()
{
    __DMA->GCSR.bit.GEN = 0; // Global enable of DMA1
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::channel_enable()
{
    __DMACHANNEL->CCR.bit.EN = 1; //Enable channel 1 of DMA1
}

template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::channel_disable()
{
    __DMACHANNEL->CCR.bit.EN = 0; 
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::set_number_of_transfers(const u16 trans_num)
{
    __DMACHANNEL->CNDTR = trans_num;
    number_of_transfers = trans_num;
}

template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::set_periph_addr(u8 volatile* addr)
{
    __DMACHANNEL->CPARH = reinterpret_cast<u16> (addr) >> 8;
    __DMACHANNEL->CPARL = reinterpret_cast<u16> (addr);
}

template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::set_memory0_addr(u8* addr)
{
    __DMACHANNEL->CM0EAR = reinterpret_cast<u32>(addr) >> 16;
    __DMACHANNEL->CM0ARH = reinterpret_cast<u16>(addr) >> 8;
    __DMACHANNEL->CM0ARL = reinterpret_cast<u16>(addr);
}

template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::set_memory1_addr(u8* addr)
{
    __DMACHANNEL->CPARH = reinterpret_cast<u16> (addr) >> 8;
    __DMACHANNEL->CPARL = reinterpret_cast<u16> (addr);
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
bool DriverDMA<DMA,DMAChannel>::is_busy()
{
    if(__DMACHANNEL->CCR.bit.EN) {
        if(__DMACHANNEL->CSPR.bit.TCIF) {
            return false;
        }
        else{
            return true;
        }
    }
    else{
        return false;
    }
    
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
u16 DriverDMA<DMA,DMAChannel>::get_amount_of_last_transation()
{
    return number_of_transfers - __DMACHANNEL->CNDTR;
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::interrupt_enable(EnumDMA::InterruptSelection isel)
{
    if(isel == EnumDMA::INTERRUPT_HALF_TRANSACTION_COMPLETE) {
        __DMACHANNEL->CCR.bit.HTIE = 1;
    }
    else{
        __DMACHANNEL->CCR.bit.TCIE = 1;
    }
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::interrupt_disable(EnumDMA::InterruptSelection isel)
{
    if(isel == EnumDMA::INTERRUPT_HALF_TRANSACTION_COMPLETE) {
        __DMACHANNEL->CCR.bit.HTIE = 0;
    }
    else{
        __DMACHANNEL->CCR.bit.TCIE = 0;
    }
}

//----------------------------------------------------------------------------------------------
template <EnumDMA::DMASel DMA, EnumDMA::ChannelSel DMAChannel>
void DriverDMA<DMA,DMAChannel>::interrupt_clear_pending_flag(EnumDMA::InterruptSelection isel)
{
    if(isel == EnumDMA::INTERRUPT_HALF_TRANSACTION_COMPLETE) {
        __DMACHANNEL->CSPR.bit.HTIF = 0;
    }
    else{
        __DMACHANNEL->CSPR.bit.TCIF = 0;
    }

    switch(NumChannel) {
    case EnumDMA::CHANNEL0:
        __DMA->GIR1.bit.IFC0 = 0;
        break;

    case EnumDMA::CHANNEL1:
        __DMA->GIR1.bit.IFC1 = 0;
        break;

    case EnumDMA::CHANNEL2:
        __DMA->GIR1.bit.IFC2 = 0;
        break;

    case EnumDMA::CHANNEL3:
        __DMA->GIR1.bit.IFC3 = 0;
        break;
    }
}

#endif

