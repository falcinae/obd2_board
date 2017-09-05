/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : v1.35
        Device            :  PIC24FJ128GA306
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.31
        MPLAB             :  MPLAB X 3.60
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

#include "mcc_generated_files/mcc.h"
#include "FatFS/diskio.h"
#include "FatFS/ff.h"
#include <stdio.h>

#define _1S_TIMER1_TICK             1

extern volatile bit Timer1_1s_Tick;


//FIL fil;       /* File object */
//char line[82]; /* Line buffer */
//FRESULT fr;    /* FatFs return code */
//FATFS FatFs;
//unsigned int br;

/*
                         Main application
 */

void GoToSleep (void)
{
    return;
}

void ReStartSystem (void)
{
    return;
}

unsigned char IsOnTrip (void)
{
    return 0;
}



// Rutinas GPS
void RequestDataFromGPS (void)
{
    return;
}
void ReadPositionData (void)
{
    return;
}
// Rutinas GPS

//Rutina OBD
void ReadStartTripData (void)
{
    return;
}


int main(void)
{
    unsigned char resetTrip = 0;
    
    // initialize the device
    SYSTEM_Initialize();

    /* 
    Register work area to the default drive
    f_mount(&FatFs, "", 0);
      
    fr = f_open(&fil, "prueba.txt", FA_WRITE | FA_CREATE_NEW | FA_OPEN_APPEND);   
    sprintf(line,"Hola mundo\r\n");   
    fr = f_write(&fil, line, 12, &br);   
    sprintf(line,"Hola mundo\r\n");
    fr = f_write(&fil, line, 16, &br);           
    f_close(&fil);
    */

    while (1)
    {
        if (IsOnTrip())
        {
            if (Timer1_1s_Tick)
            {
                Timer1_1s_Tick = 0;
                
                if (!resetTrip)
                {
                    ReStartSystem();
                    ReadStartTripData(); //PIDs Supported??
                    resetTrip = 1;
                }

                //GetTripData();
                //SendTripData();
                //StoreTripData();
               
                /*
                //ToDo Something
                switch (TMR1_SoftwareCounterGet())
                {
                    //case _250MS:
                    //    break;
                    case _1S_TIMER1_TICK:
                        TMR1_SoftwareCounterClear();

                        break;
                    //case _10S:
                    //    break;
                }
                */ 
            }
        }
        else
        {
            resetTrip = 0;
            GoToSleep();
        }
        
    }

    return -1;
}
/**
 End of File
*/