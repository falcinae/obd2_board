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
#include "obd.h"
#include "gps.h"
#include "gprs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _1S_TIMER1_TICK                 1

#define VREF                            3.30
#define ADC_BIT_RESOLUTION              10
#define STEPS_ADC_RESOLUTION            1024

#define STOPPED_CAR_MINIMUM_VBAT_VALUE  11.8
#define STOPPED_CAR_MAXIMUM_VBAT_VALUE  12.3

#define ONTRIP_CAR_MINIMUM_VBAT_VALUE   13.8
#define ONTRIP_CAR_MAXIMUM_VBAT_VALUE   14.4


unsigned char Timer1_1s_Tick = 0;

OBD_Data OBD_Value;

void GoToSleep (void);

unsigned char ReStartSystem (void);
unsigned char IsOnTrip (void);
void MeasureIgnitionLevel (float *ignValue);

void StoreTripData (void);
void SendTripData (char *dataToSend);
void RequestConfigurationFromHost (void);

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
    //Deactivate 3V3 Peripheral Power and 3V8 Modem Power
    _3V3_PERIPH_CTRL_SetLow();
    _3V8_MODEM_CTRL_SetLow();
     
    //Deactivate battery charge
    ENABLE_BAT_CHG_SetLow();
    
    //Enter microcontroller in Sleep mode
    Sleep();

    return;
}

unsigned char ReStartSystem (void)
{
    //Re-Activate 3V3 Peripheral Power and 3V8 Modem Power
    _3V3_PERIPH_CTRL_SetHigh();
    _3V8_MODEM_CTRL_SetHigh();
     
    //Re-Activate battery charge
    ENABLE_BAT_CHG_SetHigh();
    
    if (!ConnectWithObdInterpreter ())
        return 0;
    else
        return 1;
}

unsigned char IsOnTrip (void)
{
    float ignValue = 0;
    
    MeasureIgnitionLevel (&ignValue);
    
    if (ignValue > ONTRIP_CAR_MINIMUM_VBAT_VALUE) 
        return 0;
    else
        return 1;
}

void MeasureIgnitionLevel (float *ignValue)
{
    unsigned int adcValue = 0;

    CAR_BAT_LEVEL_SetHigh();
    ADC1_ChannelSelect(ADC1_CAR_BAT_LEVEL);
    ADC1_Start();
    while(ADC1_IsConversionComplete());
    adcValue = ADC1_ConversionResultGet();
    ADC1_Stop();
    CAR_BAT_LEVEL_SetLow();
    
    *ignValue = (float) (VREF/STEPS_ADC_RESOLUTION) * adcValue;
}

void StoreTripData (void)
{
    Nop();
}

void SendTripData (char *dataToSend)
{
    SendDataToGprs(dataToSend);
}
    
void RequestConfigurationFromHost (void)
{
    Nop();
}

int main(void)
{
    //Iniciar variables de datos
    char gpsDate[50] = {};
    char gpsTime[50] = {};
    char northSouth[50] = {};
    char gpsLongitude[50] = {};
    char gpsLatitude[50] = {};
    char eastWest[50] = {};
    char messageToSend[50] = {};
    
    //Iniciar periféricos
    SYSTEM_Initialize();
 
    //Bucle principal
    while (1)
    {
        //Comprobar si hay alguna configuración o lectura a la espera
        RequestConfigurationFromHost();
        
        //Comprobamos si estamos en viaje
        if (!IsOnTrip())
        {
            //Si ha pasado 1 seg..
            if (Timer1_1s_Tick)
            {
                Timer1_1s_Tick = 0;
                RequestDataFromOBD(); //Obtenemos los datos de la ECU del vehiculo
                ReadGpsGprmcCommand (gpsDate, gpsTime, gpsLatitude, northSouth, gpsLongitude, eastWest); //Obtenemos información GPS
                sprintf (messageToSend, "%s", "HOLA);");
                SendTripData(messageToSend); //Enviamos la información
                //StoreTripData(); //Almacenamos la información en la microSD 
            }
        }
        else
        {
            //Poner el dispositivo en modo de ultra bajo consumo
            GoToSleep();
        }
    }

    return 1;
}

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
