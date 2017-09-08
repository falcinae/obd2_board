/*************************************************************************
Nombre del fichero: 	gprs.c
Tipo de fichero: 		Source File
Fecha de creacion: 		17-Abril-2017
Ultima modificacion: 	17-Abril-2017
Compañia:				Universidad de Cádiz
Responsable: 			Javier Alcina
 
Proposito:
En este fichero se definirán todas las funciones referentes a las 
comunicaciones por GPRS.
Lista de modificaciones:
************************************************************************/

#include "gprs.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*************************************************************************
    Nombre de la función: 	ConfigGprsPort()
    Responsable: 			Javier Alcina
    Descripción:
        Configurar el puerto de comunicaciones con el modem GPRS
    Precondiciones:
        Ninguna
    Returns
        Ninguno
    Parametros
        Ninguno
************************************************************************/
void ConfigGprsPort (void)
{
    UART3_Initialize();
}


/*************************************************************************
    Nombre de la función: 	Gprs_SendMessageReadResponse()
    Responsable: 			Javier Alcina
    Descripción:
        Función para enviar mensajes y obtener respuesta del modem GPRS
    Precondiciones:
        Haber iniciado la uart de comunicaciones
    Returns
        Ninguno
    Parametros
        char *bufferIn --------> puntero a cadena de entrada
        char *bufferOut --------> puntero a cadena de salida
 ************************************************************************/
void Gprs_SendMessageReadResponse (char *bufferIn, char *bufferOut)
{
    char *bufferTemp;
    
    UART3_WriteBuffer(bufferIn, strlen(bufferIn));
    UART3_ReadBuffer(bufferTemp, 50);
    
    strcpy(bufferOut, bufferTemp);
    
    return;
}

/*************************************************************************
    Nombre de la función: 	CheckSIM()
    Responsable: 			Javier Alcina
    Descripción:
        Función para comprobar que la tarjeta SIM está introducida
    Precondiciones:
        La tarjeta SIM introducida debe tener desactivado el PIN
    Returns
        0 -> Tarjeta SIM ok
        1 -> Error en la lectura de SIM
    Parametros
        Ninguno
************************************************************************/
unsigned char CheckSIM (void)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);
    
    sprintf(bufferToSend, "%s", AT_COMMAND_CHECK_PIN);
    Gprs_SendMessageReadResponse (bufferToSend, bufferRead);
            
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

/*************************************************************************
    Nombre de la función: 	CheckNetwork()
    Responsable: 			Javier Alcina
    Descripción:
        Función para comprobar que el modem está conectado a la red
    Precondiciones:
        La tarjeta SIM introducida debe tener desactivado el PIN
    Returns
        0 -> Conexión a red OK
        1 -> Error en conexión a red
    Parametros
        Ninguno
************************************************************************/
unsigned char CheckNetwork (void)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);

    sprintf(bufferToSend, "%s", AT_COMMAND_CHECK_NETWORK);
    Gprs_SendMessageReadResponse (bufferToSend, bufferRead);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

/*************************************************************************
    Nombre de la función: 	CheckSignalQuality()
    Responsable: 			Javier Alcina
    Descripción:
        Función para comprobar que el modem está conectado a la red
    Precondiciones:
        La tarjeta SIM introducida debe tener desactivado el PIN
    Returns
        00 -> -113dBm o menos
        01 -> -111dBm
        02..30 -> -109..-53 dBm
        31 -> -51 dBm o mayor
        99 -> Desconocido
    Parametros
        Ninguno
************************************************************************/
int CheckSignalQuality (void)
{
    char singalQualityBuffer[15];
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);

    sprintf(bufferToSend, "%s", AT_COMMAND_CHECK_SIGNAL_QUALITY);
    Gprs_SendMessageReadResponse (bufferToSend, bufferRead);
    
    if (strstr(bufferRead, SIGNAL_QUALITY_RESPONSE) != NULL)
    {
        memcpy(singalQualityBuffer,bufferRead[6], 2);
        singalQualityBuffer[2] = 0;
        return atoi(singalQualityBuffer);
    }
    else
        return GENERAL_ERROR;

}

/*************************************************************************
    Nombre de la función: 	RetrieveNetworkOperator()
    Responsable: 			Javier Alcina
    Descripción:
        Función para comprobar que el modem está conectado al operador
        que corresponde
    Precondiciones:
        Debe ejectarse un CheckSIM();
    Returns
        0 -> Ha encontrado la red definida en OPERATOR_NAME
        1 -> Error en conexión con operador o conexión a otro operador
  
    Parametros
        Ninguno
************************************************************************/
unsigned char RetrieveNetworkOperator (void)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);

    sprintf(bufferToSend, "%s", AT_COMMAND_RETRIEVE_OPERATOR);
    Gprs_SendMessageReadResponse (bufferToSend, bufferRead);
    
    if (strstr(bufferRead, OPERATOR_NAME) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

/*************************************************************************
    Nombre de la función: 	EstablishGprsConnection()
    Responsable: 			Javier Alcina
    Descripción:
        Función para establecer conectividad GPRS
    Precondiciones:
        Debe ejectarse un CheckSIM();
    Returns
        0 -> Se ha establecido conexión
        1 -> Error en el establecimiento de conexión
  
    Parametros
        Ninguno
************************************************************************/
unsigned char EstablishGprsConnection (void)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);

    sprintf(bufferToSend, "%s", AT_COMMAND_ESTABLISH_GPRS_CONNECTION);
    Gprs_SendMessageReadResponse (bufferToSend, bufferRead);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

/*************************************************************************
    Nombre de la función: 	ConnectWithOurServer()
    Responsable: 			Javier Alcina
    Descripción:
        Función para conectar con servidor establecido
    Precondiciones:
        Debe haber una conexión GPRS activa
    Returns
        0 -> Se ha establecido conexión
        1 -> Error en el establecimiento de conexión
  
    Parametros
        Ninguno
************************************************************************/
unsigned char ConnectWithOurServer (void)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);

    sprintf(bufferToSend, "%s", AT_COMMAND_CONNECT_WITH_OUR_SERVER);
    Gprs_SendMessageReadResponse (bufferToSend, bufferRead);
    
    if (strstr(bufferRead, SERVER_NAME) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

/*************************************************************************
    Nombre de la función: 	CheckServerConnection()
    Responsable: 			Javier Alcina
    Descripción:
        Función para verificar que se ha conectado al servidor
    Precondiciones:
        Debe haberse una conexión activa 
    Returns
        0 -> Se ha establecido conexión
        1 -> Error en el establecimiento de conexión
  
    Parametros
        Ninguno
************************************************************************/
unsigned char CheckServerConnection (void)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);

    sprintf(bufferToSend, "%s", AT_COMMAND_CHECK_SERVER_CONNECTION);
    Gprs_SendMessageReadResponse (bufferToSend, bufferRead);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

/*************************************************************************
    Nombre de la función: 	OpenGprsSession()
    Responsable: 			Javier Alcina
    Descripción:
        Función para establecer conectividad GPRS
    Precondiciones:
 Abre el contexto PDP o sesión 
    Returns
        0 -> Se ha establecido conexión
        1 -> Error en el establecimiento de conexión
  
    Parametros
        Ninguno
************************************************************************/
unsigned char OpenGprsSession (void)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);

    sprintf(bufferToSend, "%s", AT_COMMAND_OPEN_SESSION);
    Gprs_SendMessageReadResponse (bufferToSend, bufferRead);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

/*************************************************************************
    Nombre de la función: 	CloseGprsSession()
    Responsable: 			Javier Alcina
    Descripción:
        Función para establecer conectividad GPRS
    Precondiciones:
        Debe ejectarse un CheckSIM();
    Returns
        0 -> Se ha establecido conexión
        1 -> Error en el establecimiento de conexión
  
    Parametros
        Ninguno
************************************************************************/
unsigned char CloseGprsSession (void)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);

    sprintf(bufferToSend, "%s", AT_COMMAND_CLOSE_SESSION);
    Gprs_SendMessageReadResponse (bufferToSend, bufferRead);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

/*************************************************************************
    Nombre de la función: 	CloseGprsConnection()
    Responsable: 			Javier Alcina
    Descripción:
        Función para cerrar la conexión GPRS
    Precondiciones:
        Debe haber una conexión abierta
    Returns
        0 -> Se ha cerrado la conexión
        1 -> Error en el establecimiento de conexión
  
    Parametros
        Ninguno
************************************************************************/
unsigned char CloseGprsConnection (void)
{
    char bufferToSend[50] = {};
    char bufferRead[50] = {};
    
    memset(bufferToSend, 0, 50);
    memset(bufferRead, 0, 50);

    sprintf(bufferToSend, "%s", AT_COMMAND_CLOSE_GPRS_CONNECTION);
    Gprs_SendMessageReadResponse (bufferToSend, bufferRead);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

/*************************************************************************
    Nombre de la función: 	GprsStartSequence()
    Responsable: 			Javier Alcina
    Descripción:
        Función con la secuencia de llamadas para establecer conectividad GPRS
        Si se utiliza esta función no se necesita usar las anteriores de manera 
        individual
    Precondiciones:
        Debe ejectarse un CheckSIM();
    Returns
        0 -> Se ha establecido conexión
        1 -> Error en el establecimiento de conexión
  
    Parametros
        Ninguno
************************************************************************/
unsigned char GprsStartSequence (void)
{
    if(CheckSIM ())
    {
        return ERROR_CODE_CHECK_PIN;
    }
    if(CheckNetwork ())
    {
        return ERROR_CODE_CHECK_NETWORK;
    }
    if(RetrieveNetworkOperator())
    {
        return ERROR_CODE_RETRIEVE_OPERATOR;
    }
    if(CheckSignalQuality ())
    {
        return ERROR_CODE_CHECK_SIGNAL_QUALITY;
    }
    if(EstablishGprsConnection ())
    {
        return ERROR_CODE_ESTABLISH_GPRS_CONNECTION;
    }
    if(CheckServerConnection ())
    {
        return ERROR_CODE_CONNECT_WITH_OUR_SERVER;
    }
    if(CheckServerConnection ())
    {
        return ERROR_CODE_CHECK_SERVER_CONNECTION;
    }
    if(OpenGprsSession ())
    {
        return ERROR_CODE_OPEN_SESSION;
    }
    
    return 0;  
}

/*************************************************************************
    Nombre de la función: 	GprsReconnectSequence()
    Responsable: 			Javier Alcina
    Descripción:
        Secuencia para reestablecer conectividad GPRS (esta función 
        no abre socket).
    Precondiciones:
        Debe ejectarse un CheckSIM();
    Returns
        0 -> Se ha establecido conexión
        1 -> Error en el establecimiento de conexión
  
    Parametros
        Ninguno
************************************************************************/
unsigned char GprsReconnectSequence (void)
{
    if(CheckSignalQuality ())
    {
        return ERROR_CODE_CHECK_SIGNAL_QUALITY;
    }
    if(EstablishGprsConnection ())
    {
        return ERROR_CODE_ESTABLISH_GPRS_CONNECTION;
    }
    if(CheckServerConnection ())
    {
        return ERROR_CODE_CONNECT_WITH_OUR_SERVER;
    }
    if(CheckServerConnection ())
    {
        return ERROR_CODE_CHECK_SERVER_CONNECTION;
    }
    if(OpenGprsSession ())
    {
        return ERROR_CODE_OPEN_SESSION;
    }
    
    return 0;  
}

/*************************************************************************
    Nombre de la función: 	GprsCloseSequence()
    Responsable: 			Javier Alcina
    Descripción:
        Secuencia de cierre de conexión
    Precondiciones:
        Debe haber una conexión activa
    Returns
        0 -> Se ha establecido conexión
        1 -> Error en el establecimiento de conexión
  
    Parametros
        Ninguno
************************************************************************/
unsigned char GprsCloseSequence (void)
{    
    if(CloseGprsSession ())
    {
        return ERROR_CODE_CLOSE_SESSION;
    }
    if(CloseGprsConnection ())
    {
        return ERROR_CODE_CLOSE_GPRS_CONNECTION;
    }
    
    return 0;  
}

/*************************************************************************
    Nombre de la función: 	GprsCloseSequence()
    Responsable: 			Javier Alcina
    Descripción:
        Secuencia de cierre de conexión
    Precondiciones:
        Debe haber una conexión activa
    Returns
        0 -> Se ha establecido conexión
        1 -> Error en el establecimiento de conexión
  
    Parametros
        char *gprsData -> Puntero a cadena de salida
************************************************************************/
void SendDataToGprs (char *gprsData)
{
    char bufferToSend[50] = {};
    
    memset(bufferToSend, 0, 50);
    strcpy(bufferToSend, gprsData);
    UART3_WriteBuffer(bufferToSend, strlen(bufferToSend));

    return;
}