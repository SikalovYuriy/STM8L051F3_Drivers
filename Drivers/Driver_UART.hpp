/* =================================================================================
 File name:     Driver_UART.hpp                                
 Autor:         Sikalov Yuriy
 Description:   The UART driver.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 17-02-2014 Version 1.00
-----------------------------------------------------------------------------------*/

#ifndef _DRIVERUART_H_
#define _DRIVERUART_H_

#include "type.h"
#include "iostm8l051f3.h"
#include "Driver_DMA.hpp"
#include "Driver_Interrupt.hpp"

//#define _SEMAPHORE_

class PinconfigUART
{
public:
    // GPIO configuration for RX and TX pins
    enum TXRX_PIN_SELECT {
        TXonPC3_RXonPC2,
        TXonPA2_RXonPA3,
        TXonPC5_RXonPC6
    };

    PinconfigUART(TXRX_PIN_SELECT);

    void set_UART_pin(TXRX_PIN_SELECT);
};

class DriverUART
{
public:
    enum TRANSMITION_OR_RECEPTION;
private:
    DriverUART(const DriverUART&);               // prevent copying
    DriverUART& operator=(const DriverUART&);

    bool locker;
    u32  sys_clock;
    u8   received_size;
    u8   UartBuffer[128];

    DriverDMA<EnumDMA::DMA1,EnumDMA::CHANNEL1> DMAChannelTX;
    DriverDMA<EnumDMA::DMA1,EnumDMA::CHANNEL2> DMAChannelRX;

    static void select_direction(TRANSMITION_OR_RECEPTION);
    
    //-----------------------------------------------------------------------------------------------------
    // Interrupts
    //-----------------------------------------------------------------------------------------------------
    #pragma vector = USART_T_TC_vector
    __interrupt void static USART_transmition_complete() 
    {
        select_direction(RECEPTION);
        USART1_SR_bit.TC = 0;
    }

    // Reception
    #pragma vector = USART_R_IDLE_vector
    __interrupt void static UART1_reception();

public:

    enum SELECTUART{UART1};

    // Character Length
    enum CHARLEN{
        CHR_8, CHR_9
    };

    // Parity control
    enum PARITY_CONTROL{
        PARITY_DISABLE, PARITY_ENABLE
    };

    // Parity selection
    enum PARITY_SELECTION{
        PARITY_EVEN, PARITY_ODD
    };

    // Stop bit selection
    enum STOP_BIT{
        STOPBIT_1, STOPBIT_2 = 2, STOPBIT_1_5
    };
       
    // Enable or disable transmission
    enum TALK{
        TRASMIT_DISABLE,
        TRASMIT_ENABLE
    };

    // Enable or disable reception
    enum LISTEN{
        RECIEVE_DISABLE,
        RECIEVE_ENABLE,
    };

    // Transmite and recieve buffer type
    enum BUFFERTYPE{
        BUFF_COMMON,
        BUFF_SEPARATED
    };

    enum TRANSMITION_OR_RECEPTION{
        RECEPTION,
        TRANSMITION
    };

    DriverUART(SELECTUART, u32 baud, u32 sys_clock, PinconfigUART confPin = PinconfigUART(PinconfigUART::TXonPC3_RXonPC2));

    inline bool isopen(){return locker;};

    #ifdef _SEMAPHORE_
    inline bool open() {locker = true; Semaphore_pend(SEM_spi, BIOS_WAIT_FOREVER); return true;};
    inline bool close(){locker = false; Semaphore_post(SEM_spi); return true;};
    #else
    inline bool open() {locker = true; while(locker); return true;};
    inline bool close(){locker = false; return true;};
    #endif

    void set_sysclock(u32 sys_clk, u32 baud_rate);
    void set_baudrate(u32 baud_rate);

    u8* get_pointer_on_internal_buffer();
    u16 get_received_size();

    void reception_handshake();
    
    void operator= (CHARLEN); 
    void operator= (TALK);
    void operator= (LISTEN);
    void operator= (PARITY_CONTROL);
    void operator= (PARITY_SELECTION);
    void operator= (STOP_BIT);

     // Transmission
    void operator<<(const char ch);
    void operator<<(const char *pch);  
    void transmit(u8 *source, u16 size);
    void transmit(u16 *source, u16 size);
    void transmit(u16 size);
    
    // Reception
    static void receive_handle();
};


#endif

