/*************************************************************************
Nombre del fichero: 	gprs.c
Tipo de fichero: 		Source File
Fecha de creacion: 		17-Abril-2017
Ultima modificacion: 	17-Abril-2017
Compa�ia:				Universidad de C�diz
Responsable: 			Javier Alcina
 
Proposito:
En este fichero se definir�n todas las funciones referentes a las 
comunicaciones por GPRS.
Lista de modificaciones:
************************************************************************/

#include "gprs.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*************************************************************************
    Nombre de la funci�n: 	ConfigGprsPort()
    Responsable: 			Javier Alcina
    Descripci�n:
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
    Nombre de la funci�n: 	Gprs_SendMessageReadResponse()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para enviar mensajes y obtener respuesta del modem GPRS
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
    Nombre de la funci�n: 	CheckSIM()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para comprobar que la tarjeta SIM est� introducida
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
    Nombre de la funci�n: 	CheckNetwork()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para comprobar que el modem est� conectado a la red
    Precondiciones:
        La tarjeta SIM introducida debe tener desactivado el PIN
    Returns
        0 -> Conexi�n a red OK
        1 -> Error en conexi�n a red
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
    Nombre de la funci�n: 	CheckSignalQuality()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para comprobar que el modem est� conectado a la red
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
    Nombre de la funci�n: 	RetrieveNetworkOperator()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para comprobar que el modem est� conectado al operador
        que corresponde
    Precondiciones:
        Debe ejectarse un CheckSIM();
    Returns
        0 -> Ha encontrado la red definida en OPERATOR_NAME
        1 -> Error en conexi�n con operador o conexi�n a otro operador
  
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
    Nombre de la funci�n: 	EstablishGprsConnection()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para establecer conectividad GPRS
    Precondiciones:
        Debe ejectarse un CheckSIM();
    Returns
        0 -> Se ha establecido conexi�n
        1 -> Error en el establecimiento de conexi�n
  
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
    Nombre de la funci�n: 	ConnectWithOurServer()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para conectar con servidor establecido
    Precondiciones:
        Debe haber una conexi�n GPRS activa
    Returns
        0 -> Se ha establecido conexi�n
        1 -> Error en el establecimiento de conexi�n
  
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
    Nombre de la funci�n: 	CheckServerConnection()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para verificar que se ha conectado al servidor
    Precondiciones:
        Debe haberse una conexi�n activa 
    Returns
        0 -> Se ha establecido conexi�n
        1 -> Error en el establecimiento de conexi�n
  
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
    Nombre de la funci�n: 	OpenGprsSession()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para establecer conectividad GPRS
    Precondiciones:
 Abre el contexto PDP o sesi�n 
    Returns
        0 -> Se ha establecido conexi�n
        1 -> Error en el establecimiento de conexi�n
  
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
    Nombre de la funci�n: 	CloseGprsSession()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para establecer conectividad GPRS
    Precondiciones:
        Debe ejectarse un CheckSIM();
    Returns
        0 -> Se ha establecido conexi�n
        1 -> Error en el establecimiento de conexi�n
  
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
    Nombre de la funci�n: 	CloseGprsConnection()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n para cerrar la conexi�n GPRS
    Precondiciones:
        Debe haber una conexi�n abierta
    Returns
        0 -> Se ha cerrado la conexi�n
        1 -> Error en el establecimiento de conexi�n
  
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
    Nombre de la funci�n: 	GprsStartSequence()
    Responsable: 			Javier Alcina
    Descripci�n:
        Funci�n con la secuencia de llamadas para establecer conectividad GPRS
        Si se utiliza esta funci�n no se necesita usar las anteriores de manera 
        individual
    Precondiciones:
        Debe ejectarse un CheckSIM();
    Returns
        0 -> Se ha establecido conexi�n
        1 -> Error en el establecimiento de conexi�n
  
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
    Nombre de la funci�n: 	GprsReconnectSequence()
    Responsable: 			Javier Alcina
    Descripci�n:
        Secuencia para reestablecer conectividad GPRS (esta funci�n 
        no abre socket).
    Precondiciones:
        Debe ejectarse un CheckSIM();
    Returns
        0 -> Se ha establecido conexi�n
        1 -> Error en el establecimiento de conexi�n
  
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
    Nombre de la funci�n: 	GprsCloseSequence()
    Responsable: 			Javier Alcina
    Descripci�n:
        Secuencia de cierre de conexi�n
    Precondiciones:
        Debe haber una conexi�n activa
    Returns
        0 -> Se ha establecido conexi�n
        1 -> Error en el establecimiento de conexi�n
  
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
    Nombre de la funci�n: 	GprsCloseSequence()
    Responsable: 			Javier Alcina
    Descripci�n:
        Secuencia de cierre de conexi�n
    Precondiciones:
        Debe haber una conexi�n activa
    Returns
        0 -> Se ha establecido conexi�n
        1 -> Error en el establecimiento de conexi�n
  
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