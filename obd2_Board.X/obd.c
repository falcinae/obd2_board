
#include "obd.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ConfigObdPort (void)
{
    UART1_Initialize();
}

void RequestDataFromOBD (void)
{
    char *buffeTemp;
    
    struct obd {
    char *speedOBD;
    char *rpmOBD;
    char *tempOBD;
    char *throttleOBD;
    char *milLamp;
    char *numDTCs;
    char *vinOBD;
    char *pressureOBD;
    } OBD_Data;    
    
    ReadSpeedFromOBD (OBD_Data.speedOBD);
    ReadRpmFromOBD (OBD_Data.rpmOBD);
    ReadTemperatureFromOBD (OBD_Data.tempOBD);
    ReadThrottleFromOBD (OBD_Data.throttleOBD);
    ReadDTCsFromOBD (OBD_Data.milLamp, OBD_Data.numDTCs);
    ReadVINFromOBD (OBD_Data.vinOBD);
    ReadPressureFromOBD (OBD_Data.pressureOBD);
    
    return;
}
 
unsigned char ConnectWithObdInterpreter (void)
{
    char bufferToSend[10];
    char bufferRead[10];
    
    sprintf(bufferToSend, "%s\r\n", CONNECT_WITH_OBD_INTERPRETER);
    UART1_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART1_ReadBuffer(bufferRead, 5);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;
}

void ReadSpeedFromOBD (char *speedOBD)
{
    char bufferToSend[10];
    char bufferRead[10];
    
    sprintf(bufferToSend, "%s\r\n", OBD_SPEED_PID);
    UART1_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART1_ReadBuffer(bufferRead, 5);
    
    memcpy(speedOBD, &bufferRead[2], 1);
}

void ReadRpmFromOBD (char *rpmOBD)
{
    char bufferToSend[10];
    char bufferRead[10];
    
    sprintf(bufferToSend, "%s\r\n", OBD_RPM_PID);
    UART1_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART1_ReadBuffer(bufferRead, 6);
    
    memcpy(rpmOBD, &bufferRead[2], 2);
}

void ReadTemperatureFromOBD (char *tempOBD)
{
    char bufferToSend[10];
    char bufferRead[10];
    
    sprintf(bufferToSend, "%s\r\n", OBD_TEMP_PID);
    UART1_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART1_ReadBuffer(bufferRead, 5);
    
    memcpy(tempOBD, &bufferRead[2], 1);
}

void ReadThrottleFromOBD (char *throttleOBD)
{
    char bufferToSend[10];
    char bufferRead[10];
    
    sprintf(bufferToSend, "%s\r\n", OBD_THROTTLE_PID);
    UART1_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART1_ReadBuffer(bufferRead, 5);
    
    memcpy(throttleOBD, &bufferRead[2], 1);
}

void ReadDTCsFromOBD (char *milLamp, char *numDTCs)
{
    char bufferToSend[10];
    char bufferRead[10];
    char numDTCsBuffer[3];
    int dtcs;
    
    sprintf(bufferToSend, "%s\r\n", OBD_nDTCs_PID);
    UART1_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART1_ReadBuffer(bufferRead, 8);
    
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
    char bufferToSend[10];
    char bufferRead[30];
    
    sprintf(bufferToSend, "%s\r\n", OBD_VIN_PID);
    UART1_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART1_ReadBuffer(bufferRead, 22);
    
    memcpy(vinOBD, &bufferRead[2], 18);
}

void ReadPressureFromOBD (char *pressureOBD)
{
    char bufferToSend[10];
    char bufferRead[10];
    
    sprintf(bufferToSend, "%s\r\n", OBD_PRESSURE_PID);
    UART1_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART1_ReadBuffer(bufferRead, 6);
    
    memcpy(pressureOBD, &bufferRead[2], 2);
}