/*************************************************************************
Nombre del fichero: 	gps.c
Tipo de fichero: 		Source File
Fecha de creacion: 		17-Abril-2017
Ultima modificacion: 	17-Abril-2017
Compañia:				Universidad de Cádiz
Responsable: Javier Alcina
 
Proposito:
En este fichero se definirán todas las funciones referentes a las 
comunicaciones GPS.
Lista de modificaciones:
************************************************************************/

#include "gps.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*************************************************************************
    Nombre de la función: 	ConfigGpsPort()
    Responsable: Javier Alcina
    Descripción:
        Configurar el puerto de comunicaciones con el modulo GPS
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        Ninguno
************************************************************************/
void ConfigGpsPort (void)
{
    UART2_Initialize();
}



/*************************************************************************
    Nombre de la función: 	ReadGpsGprmcCommand()
    Responsable: Javier Alcina
    Descripción:
        Función para obtener los datos de GPS y devolverlos
    Precondiciones:
        Haber iniciado la uart de comunicaciones
    Returns
        Ninguno
    Parametros
        char *gpsDate --------> puntero a cadena gpsDate (Fecha)
        char *gpsTime --------> puntero a cadena gpsTime (Hora)
        char *gpsLatitude ----> puntero a cadena gpsLatitude (Latitud)
        char *northSouth -----> puntero a cadena northSouth (Norte/Sur)
        char *gpsLongitude ---> puntero a cadena gpsLongitude (Longitud)
        char *eastWest -------> puntero a cadena eastWest (Este/Oeste)
 ************************************************************************/
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