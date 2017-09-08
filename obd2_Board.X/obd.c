
#include "obd.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ConfigObdPort (void)
{
    UART1_Initialize();
}

void Obd_SendMessageReadResponse (char *bufferIn, char *bufferOut)
{
    char *bufferTemp;
    
    UART1_WriteBuffer(bufferIn, strlen(bufferIn));
    UART1_ReadBuffer(bufferTemp, 50);
    
    strcpy(bufferOut, bufferTemp);
    
    return;
}

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

void ReadRpmFromOBD (char *rpmOBD)
{
    char bufferToSend[10];
    char bufferRead[10];
    
    sprintf(bufferToSend, "%s\r\n", OBD_RPM_PID);
    Obd_SendMessageReadResponse(bufferToSend, bufferRead);
    
    memcpy(rpmOBD, &bufferRead[2], 2);
}

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

void CleanDtcsFromOBD (void)
{
    char bufferToSend[10];
    
    sprintf(bufferToSend, "%s\r\n", OBD_CLEAN_DTCS_PID);
    UART1_WriteBuffer(bufferToSend, strlen(bufferToSend));
}


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