/*************************************************************************
Nombre del fichero: 	obd.c
Tipo de fichero: 		Source File
Fecha de creacion: 		17-Abril-2017
Ultima modificacion: 	17-Abril-2017
Compañia:				Universidad de Cádiz
Responsable: Javier Alcina
 
Proposito:
En este fichero se definirán todas las funciones referentes a las 
comunicaciones OBD.
Lista de modificaciones:
************************************************************************/

#include "obd.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*************************************************************************
    Nombre de la función: 	ConfigObdPort()
    Responsable: Javier Alcina
    Descripción:
        Configurar el puerto de comunicaciones con el chip OBD
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        Ninguno
************************************************************************/
void ConfigObdPort (void)
{
    UART1_Initialize();
}

/*************************************************************************
    Nombre de la función: 	ReadGpsGprmcCommand()
    Responsable: Javier Alcina
    Descripción:
        Función para enviar mensajes y obtener respuesta del chip OBD
    Precondiciones:
        Haber iniciado la uart de comunicaciones
    Returns
        Ninguno
    Parametros
        char *bufferIn --------> puntero a cadena de entrada
        char *bufferOut --------> puntero a cadena de salida
 ************************************************************************/
void Obd_SendMessageReadResponse (char *bufferIn, char *bufferOut)
{
    char *bufferTemp;
    
    UART1_WriteBuffer(bufferIn, strlen(bufferIn));
    UART1_ReadBuffer(bufferTemp, 50);
    
    strcpy(bufferOut, bufferTemp);
    
    return;
}


/*************************************************************************
    Nombre de la función: 	RequestDataFromOBD()
    Responsable: Javier Alcina
    Descripción:
        Función con secuencia de peticiones de datos al chip OBD
    Precondiciones:
        Haber conectado con chip OBD
    Returns
        Ninguno
    Parametros
        Ninguno
 ************************************************************************/
void RequestDataFromOBD (void)
{
    ReadSpeedFromOBD (OBD_Value.speedOBD);
    ReadRpmFromOBD (OBD_Value.rpmOBD);
    ReadTemperatureFromOBD (OBD_Value.tempOBD);
    ReadThrottleFromOBD (OBD_Value.throttleOBD);
    ReadDTCsFromOBD (OBD_Value.milLamp, OBD_Value.numDTCs);
    ReadVINFromOBD (OBD_Value.vinOBD);
    ReadPressureFromOBD (OBD_Value.pressureOBD);
    
    return;
}
 
/*************************************************************************
    Nombre de la función: 	ConnectWithObdInterpreter()
    Responsable: Javier Alcina
    Descripción:
        Función para conectar con el chip OBD
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        Ninguno
 ************************************************************************/
unsigned char ConnectWithObdInterpreter (void)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);
    
    sprintf(bufferToSend, "%s\r\n", CONNECT_WITH_OBD_INTERPRETER);
    Obd_SendMessageReadResponse(bufferToSend, bufferRead);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;
}

/*************************************************************************
    Nombre de la función: 	ReadSpeedFromOBD()
    Responsable: Javier Alcina
    Descripción:
        Función para solicitar la velocidad del vehículo al chip OBD
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        char *speedOBD --------> puntero a cadena de salida (velocidad)
 ************************************************************************/
void ReadSpeedFromOBD (char *speedOBD)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);
   
    sprintf(bufferToSend, "%s\r\n", OBD_SPEED_PID);
    Obd_SendMessageReadResponse(bufferToSend, bufferRead);
    
    memcpy(speedOBD, &bufferRead[2], 1);
}

/*************************************************************************
    Nombre de la función: 	ReadRpmFromOBD()
    Responsable: Javier Alcina
    Descripción:
        Función para solicitar las revoluciones por minuto del motor
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        char *rpmOBD --------> puntero a cadena de salida (rpms)
*************************************************************************/
void ReadRpmFromOBD (char *rpmOBD)
{
    char bufferToSend[10];
    char bufferRead[10];
    
    sprintf(bufferToSend, "%s\r\n", OBD_RPM_PID);
    Obd_SendMessageReadResponse(bufferToSend, bufferRead);
    
    memcpy(rpmOBD, &bufferRead[2], 2);
}

/*************************************************************************
    Nombre de la función: 	ReadTemperatureFromOBD()
    Responsable: Javier Alcina
    Descripción:
        Función para solicitar la temperatura del refrigerante del motor
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        char *tempOBD --------> puntero a cadena de salida (temperatura)
 ************************************************************************/
void ReadTemperatureFromOBD (char *tempOBD)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);
    
    sprintf(bufferToSend, "%s\r\n", OBD_TEMP_PID);
    Obd_SendMessageReadResponse(bufferToSend, bufferRead);
    
    memcpy(tempOBD, &bufferRead[2], 1);
}

/*************************************************************************
    Nombre de la función: 	ReadThrottleFromOBD()
    Responsable: Javier Alcina
    Descripción:
        Función para solicitar el % de presión en el pedal de aceleración
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        char *throttleOBD --------> puntero a cadena de salida (% acelerador)
 ************************************************************************/
void ReadThrottleFromOBD (char *throttleOBD)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);
    
    sprintf(bufferToSend, "%s\r\n", OBD_THROTTLE_PID);
    Obd_SendMessageReadResponse(bufferToSend, bufferRead);
    
    memcpy(throttleOBD, &bufferRead[2], 1);
}

/*************************************************************************
    Nombre de la función: 	ReadDTCsFromOBD()
    Responsable: Javier Alcina
    Descripción:
        Función para solicitar el nº de DTCs en el vehículo al chip OBD
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        char *milLamp --------> puntero a cadena de salida (Indicador MIL)
        char *numDTCs --------> puntero a cadena de salida (nº de DTCs)
 ************************************************************************/
void ReadDTCsFromOBD (char *milLamp, char *numDTCs)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    char numDTCsBuffer[3];
    int dtcs;
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);
    
    sprintf(bufferToSend, "%s\r\n", OBD_nDTCs_PID);
    Obd_SendMessageReadResponse(bufferToSend, bufferRead);
    
    dtcs = atoi(bufferRead[2]);
    
    if (dtcs & 0x80)
    {
        memcpy(milLamp, "1", 1);
        sprintf(numDTCsBuffer, "%02d", dtcs & 0x7F);
        memcpy(numDTCs, numDTCsBuffer, 1);
    }
    else
    {
        memcpy(milLamp, "0", 1);
        memcpy(numDTCs, "0", 1);
    }
    
}

/*************************************************************************
    Nombre de la función: 	CleanDtcsFromOBD()
    Responsable: Javier Alcina
    Descripción:
        Función para limpiar DTCs del vehículo mediante el chip OBD
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        Ninguno
 ************************************************************************/
void CleanDtcsFromOBD (void)
{
    char bufferToSend[10];
    
    sprintf(bufferToSend, "%s\r\n", OBD_CLEAN_DTCS_PID);
    UART1_WriteBuffer(bufferToSend, strlen(bufferToSend));
}


/*************************************************************************
    Nombre de la función: 	ReadVINFromOBD()
    Responsable: Javier Alcina
    Descripción:
        Función para solicitar el nº de bastidor del vehículo al chip OBD
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        char *vinOBD --------> puntero a cadena de salida (nº bastidor)
 ************************************************************************/
void ReadVINFromOBD (char *vinOBD)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);
    
    sprintf(bufferToSend, "%s\r\n", OBD_VIN_PID);
    Obd_SendMessageReadResponse(bufferToSend, bufferRead);
    
    memcpy(vinOBD, &bufferRead[2], 18);
}

/*************************************************************************
    Nombre de la función: 	ReadPressureFromOBD()
    Responsable: Javier Alcina
    Descripción:
        Función para solicitar la presión atmosférica al chip OBD
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        char *pressureOBD --------> puntero a cadena de salida (presión)
 ************************************************************************/
void ReadPressureFromOBD (char *pressureOBD)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);
    
    sprintf(bufferToSend, "%s\r\n", OBD_PRESSURE_PID);
    Obd_SendMessageReadResponse(bufferToSend, bufferRead);
    
    memcpy(pressureOBD, &bufferRead[2], 2);
}