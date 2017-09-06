
#include "gps.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ConfigGpsPort (void)
{
    UART2_Initialize();
}

void ReadGpsGprmcCommand (char *gpsDate, char *gpsTime,
                            char *gpsLatitude, char *northSouth,
                            char *gpsLongitude, char *eastWest)
{
    char bufferRead[100];
    char *token;
    char bufferToken[12][20];
    const char s[2] = ",";
    unsigned int i = 0;
    
    UART2_ReadBuffer(bufferRead, 70);
    
    token = strtok(bufferRead, s);
    memcpy(bufferToken[0], token, strlen(token));
    i = 1;
    while( bufferToken[i] != NULL ) 
    {
        token = strtok(NULL, ',');
        memcpy(bufferToken[i++], token, strlen(token));
    }
   
    memcpy(gpsDate, bufferToken[9], strlen(bufferToken[9]));
    memcpy(gpsTime, bufferToken[1], strlen(bufferToken[1]));
    memcpy(gpsLatitude, bufferToken[2], strlen(bufferToken[2]));
    memcpy(northSouth, bufferToken[4], strlen(bufferToken[4]));
    memcpy(gpsLongitude, bufferToken[5], strlen(bufferToken[5]));
    memcpy(eastWest, bufferToken[6], strlen(bufferToken[6]));
}