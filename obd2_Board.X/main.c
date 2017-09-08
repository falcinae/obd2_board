/*************************************************************************
Nombre del fichero: 	gprs.c
Tipo de fichero: 		Source File
Fecha de creacion: 		17-Abril-2017
Ultima modificacion: 	17-Abril-2017
Compa�ia:				Universidad de C�diz
Responsable: 			Javier Alcina
 
Proposito:
En este fichero se encuentra la aplicaci�n principal del programa.

Lista de modificaciones:
************************************************************************/


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

/*************************************************************************
    Nombre de la funci�n: 	GoToSleep()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para introducir el microcontrolador en el modo
        de m�s bajo consumno bajo consumo.
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        Ninguno
 ************************************************************************/
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

/*************************************************************************
    Nombre de la funci�n: 	ReStartSystem()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n que reestablece los perifericos y las alimentaciones
        tras un Sleep.
    Precondiciones:
        Ninguna
    Returns
        0 -> Si no hubo error en la conexi�n al chip OBD
        1 -> Si hubo error en la conexi�n al chip OBD
    Parametros
        Ninguno
 ************************************************************************/
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

/*************************************************************************
    Nombre de la funci�n: 	IsOnTrip()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para conocer si el vehiculo est� o no en viaje (VBat>13.8V)
    Precondiciones:
        Ninguna
    Returns
        0 -> Si el coche est� en viaje (VBAT > 13.8)
        1 -> Si el coche no est� en viaje (VBAT < 13.8)
    Parametros
        Ninguno
 ************************************************************************/
unsigned char IsOnTrip (void)
{
    float ignValue = 0;
    
    MeasureIgnitionLevel (&ignValue);
    
    if (ignValue > ONTRIP_CAR_MINIMUM_VBAT_VALUE) 
        return 0;
    else
        return 1;
}

/*************************************************************************
    Nombre de la funci�n: 	ReadPressureFromOBD()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para medir la tensi�n de bater�a del veh�culo
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        float *ignValue --------> puntero a valor de V de bater�a medida
 ************************************************************************/
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

/*************************************************************************
    Nombre de la funci�n: 	ReadPressureFromOBD()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para solicitar la presi�n atmosf�rica al chip OBD
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        char *pressureOBD --------> puntero a cadena de salida (presi�n)
 ************************************************************************/
void StoreTripData (void)
{
    Nop();
}

/*************************************************************************
    Nombre de la funci�n: 	ReadPressureFromOBD()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para solicitar la presi�n atmosf�rica al chip OBD
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        char *pressureOBD --------> puntero a cadena de salida (presi�n)
 ************************************************************************/
void SendTripData (char *dataToSend)
{
    SendDataToGprs(dataToSend);
}
    
/*************************************************************************
    Nombre de la funci�n: 	ReadPressureFromOBD()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para solicitar la presi�n atmosf�rica al chip OBD
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        char *pressureOBD --------> puntero a cadena de salida (presi�n)
 ************************************************************************/
void RequestConfigurationFromHost (void)
{
    Nop();
}

/*************************************************************************
    Nombre de la funci�n: 	main()
    Responsable: 			Javier Alcina
    Descripci�n:
        Aplicaci�n principal
     Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        Ninguno
 ************************************************************************/
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
    
    //Iniciar perif�ricos
    SYSTEM_Initialize();
 
    //Bucle principal
    while (1)
    {
        //Comprobar si hay alguna configuraci�n o lectura a la espera
        RequestConfigurationFromHost();
        
        //Comprobamos si estamos en viaje
        if (!IsOnTrip())
        {
            //Si ha pasado 1 seg..
            if (Timer1_1s_Tick)
            {
                Timer1_1s_Tick = 0;
                RequestDataFromOBD(); //Obtenemos los datos de la ECU del vehiculo
                ReadGpsGprmcCommand (gpsDate, gpsTime, gpsLatitude, northSouth, gpsLongitude, eastWest); //Obtenemos informaci�n GPS
                sprintf (messageToSend, "%s", "HOLA);");
                SendTripData(messageToSend); //Enviamos la informaci�n
                //StoreTripData(); //Almacenamos la informaci�n en la microSD 
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
