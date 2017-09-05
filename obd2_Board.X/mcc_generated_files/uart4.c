/**
  UART4 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    uart4.c

  @Summary
    This is the generated source file for the UART4 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for UART4. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : v1.35
        Device            :  PIC24FJ128GA306
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.31
        MPLAB 	          :  MPLAB X 3.60
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include "uart4.h"

/**
  Section: Data Type Definitions
*/

/** UART Driver Queue Status

  @Summary
    Defines the object required for the status of the queue.
*/

typedef union
{
    struct
    {
            uint8_t full:1;
            uint8_t empty:1;
            uint8_t reserved:6;
    }s;
    uint8_t status;
}

UART_BYTEQ_STATUS;

/** UART Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintenance of the hardware instance.

*/
typedef struct
{
    /* RX Byte Q */
    uint8_t                                      *rxTail ;

    uint8_t                                      *rxHead ;

    /* TX Byte Q */
    uint8_t                                      *txTail ;

    uint8_t                                      *txHead ;

    UART_BYTEQ_STATUS                        rxStatus ;

    UART_BYTEQ_STATUS                        txStatus ;

} UART_OBJECT ;

static UART_OBJECT uart4_obj ;

/** UART Driver Queue Length

  @Summary
    Defines the length of the Transmit and Receive Buffers

*/

#define UART4_CONFIG_TX_BYTEQ_LENGTH 8
#define UART4_CONFIG_RX_BYTEQ_LENGTH 8


/** UART Driver Queue

  @Summary
    Defines the Transmit and Receive Buffers

*/

static uint8_t uart4_txByteQ[UART4_CONFIG_TX_BYTEQ_LENGTH] ;
static uint8_t uart4_rxByteQ[UART4_CONFIG_RX_BYTEQ_LENGTH] ;


/**
  Section: Driver Interface
*/


void UART4_Initialize (void)
{
   // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; RXINV disabled; UEN TX_RX; 
   U4MODE = (0x8008 & ~(1<<15));  // disabling UARTEN bit   
   // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
   U4STA = 0x0000;
   // BaudRate = 9600; Frequency = 14745600 Hz; BRG 383; 
   U4BRG = 0x017F;

   IEC5bits.U4RXIE = 1;

    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
   U4MODEbits.UARTEN = 1;  // enabling UART ON bit
   U4STAbits.UTXEN = 1;
   
   

   uart4_obj.txHead = uart4_txByteQ;
   uart4_obj.txTail = uart4_txByteQ;
   uart4_obj.rxHead = uart4_rxByteQ;
   uart4_obj.rxTail = uart4_rxByteQ;
   uart4_obj.rxStatus.s.empty = true;
   uart4_obj.txStatus.s.empty = true;
   uart4_obj.txStatus.s.full = false;
   uart4_obj.rxStatus.s.full = false;
}




/**
    Maintains the driver's transmitter state machine and implements its ISR
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _U4TXInterrupt ( void )
{ 
    if(uart4_obj.txStatus.s.empty)
    {
        IEC5bits.U4TXIE = false;
        return;
    }

    IFS5bits.U4TXIF = false;

    while(!(U4STAbits.UTXBF == 1))
    {

        U4TXREG = *uart4_obj.txHead;

        uart4_obj.txHead++;

        if(uart4_obj.txHead == (uart4_txByteQ + UART4_CONFIG_TX_BYTEQ_LENGTH))
        {
            uart4_obj.txHead = uart4_txByteQ;
        }

        uart4_obj.txStatus.s.full = false;

        if(uart4_obj.txHead == uart4_obj.txTail)
        {
            uart4_obj.txStatus.s.empty = true;
            break;
        }
    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U4RXInterrupt( void )
{


    while((U4STAbits.URXDA == 1))
    {

        *uart4_obj.rxTail = U4RXREG;

        uart4_obj.rxTail++;

        if(uart4_obj.rxTail == (uart4_rxByteQ + UART4_CONFIG_RX_BYTEQ_LENGTH))
        {
            uart4_obj.rxTail = uart4_rxByteQ;
        }

        uart4_obj.rxStatus.s.empty = false;
        
        if(uart4_obj.rxTail == uart4_obj.rxHead)
        {
            //Sets the flag RX full
            uart4_obj.rxStatus.s.full = true;
            break;
        }
        
    }

    IFS5bits.U4RXIF = false;
   
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _U4ErrInterrupt ( void )
{
    if ((U4STAbits.OERR == 1))
    {
        U4STAbits.OERR = 0;
    }

    IFS5bits.U4ERIF = false;
}

/**
  Section: UART Driver Client Routines
*/

uint8_t UART4_Read( void)
{
    uint8_t data = 0;

    data = *uart4_obj.rxHead;

    uart4_obj.rxHead++;

    if (uart4_obj.rxHead == (uart4_rxByteQ + UART4_CONFIG_RX_BYTEQ_LENGTH))
    {
        uart4_obj.rxHead = uart4_rxByteQ;
    }

    if (uart4_obj.rxHead == uart4_obj.rxTail)
    {
        uart4_obj.rxStatus.s.empty = true;
    }

    uart4_obj.rxStatus.s.full = false;

    return data;
}


unsigned int UART4_ReadBuffer( uint8_t *buffer, const unsigned int bufLen)
{
    unsigned int numBytesRead = 0 ;
    while ( numBytesRead < ( bufLen ))
    {
        if( uart4_obj.rxStatus.s.empty)
        {
            break;
        }
        else
        {
            buffer[numBytesRead++] = UART4_Read () ;
        }
    }

    return numBytesRead ;
}



void UART4_Write( const uint8_t byte)
{
    IEC5bits.U4TXIE = false;
    
    *uart4_obj.txTail = byte;

    uart4_obj.txTail++;
    
    if (uart4_obj.txTail == (uart4_txByteQ + UART4_CONFIG_TX_BYTEQ_LENGTH))
    {
        uart4_obj.txTail = uart4_txByteQ;
    }

    uart4_obj.txStatus.s.empty = false;

    if (uart4_obj.txHead == uart4_obj.txTail)
    {
        uart4_obj.txStatus.s.full = true;
    }

    IEC5bits.U4TXIE = true ;
	
}


unsigned int UART4_WriteBuffer( const uint8_t *buffer , const unsigned int bufLen )
{
    unsigned int numBytesWritten = 0 ;

    while ( numBytesWritten < ( bufLen ))
    {
        if((uart4_obj.txStatus.s.full))
        {
            break;
        }
        else
        {
            UART4_Write (buffer[numBytesWritten++] ) ;
        }
    }

    return numBytesWritten ;

}


UART4_TRANSFER_STATUS UART4_TransferStatusGet (void )
{
    UART4_TRANSFER_STATUS status = 0;

    if(uart4_obj.txStatus.s.full)
    {
        status |= UART4_TRANSFER_STATUS_TX_FULL;
    }

    if(uart4_obj.txStatus.s.empty)
    {
        status |= UART4_TRANSFER_STATUS_TX_EMPTY;
    }

    if(uart4_obj.rxStatus.s.full)
    {
        status |= UART4_TRANSFER_STATUS_RX_FULL;
    }

    if(uart4_obj.rxStatus.s.empty)
    {
        status |= UART4_TRANSFER_STATUS_RX_EMPTY;
    }
    else
    {
        status |= UART4_TRANSFER_STATUS_RX_DATA_PRESENT;
    }
    return status;
}


uint8_t UART4_Peek(uint16_t offset)
{
    if( (uart4_obj.rxHead + offset) > (uart4_rxByteQ + UART4_CONFIG_RX_BYTEQ_LENGTH))
    {
      return uart4_rxByteQ[offset - (uart4_rxByteQ + UART4_CONFIG_RX_BYTEQ_LENGTH - uart4_obj.rxHead)];
    }
    else
    {
      return *(uart4_obj.rxHead + offset);
    }
}


unsigned int UART4_ReceiveBufferSizeGet(void)
{
    if(!uart4_obj.rxStatus.s.full)
    {
        if(uart4_obj.rxHead > uart4_obj.rxTail)
        {
            return(uart4_obj.rxHead - uart4_obj.rxTail);
        }
        else
        {
            return(UART4_CONFIG_RX_BYTEQ_LENGTH - (uart4_obj.rxTail - uart4_obj.rxHead));
        } 
    }
    return 0;
}


unsigned int UART4_TransmitBufferSizeGet(void)
{
    if(!uart4_obj.txStatus.s.full)
    { 
        if(uart4_obj.txHead > uart4_obj.txTail)
        {
            return(uart4_obj.txHead - uart4_obj.txTail);
        }
        else
        {
            return(UART4_CONFIG_TX_BYTEQ_LENGTH - (uart4_obj.txTail - uart4_obj.txHead));
        }
    }
    return 0;
}


bool UART4_ReceiveBufferIsEmpty (void)
{
    return(uart4_obj.rxStatus.s.empty);
}


bool UART4_TransmitBufferIsFull(void)
{
    return(uart4_obj.txStatus.s.full);
}


UART4_STATUS UART4_StatusGet (void)
{
    return U4STA;
}



/**
  End of File
*/
