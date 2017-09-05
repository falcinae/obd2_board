
#include "gprs.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ConfigGprsPort (void)
{
    UART3_Initialize();
}

unsigned char CheckSIM (void)
{
    char *bufferToSend;
    char *bufferRead;
    
    sprintf(bufferToSend, "%s", AT_COMMAND_CHECK_PIN);
    UART3_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART3_ReadBuffer(bufferRead, 4);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

unsigned char CheckNetwork (void)
{
    char bufferToSend[50];
    char *bufferRead;
    
    sprintf(bufferToSend, "%s", AT_COMMAND_CHECK_NETWORK);
    UART3_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART3_ReadBuffer(bufferRead, 4);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

unsigned char CheckSignalQuality (void)
{
    char bufferToSend[50];
    char *bufferRead;
    
    sprintf(bufferToSend, "%s", AT_COMMAND_CHECK_SIGNAL_QUALITY);
    UART3_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART3_ReadBuffer(bufferRead, 4);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

unsigned char RetrieveNetworkOperator (void)
{
    char bufferToSend[50];
    char *bufferRead;
    
    sprintf(bufferToSend, "%s", AT_COMMAND_RETRIEVE_OPERATOR);
    UART3_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART3_ReadBuffer(bufferRead, 4);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

unsigned char EstablishGprsConnection (void)
{
    char bufferToSend[50];
    char *bufferRead;
    
    sprintf(bufferToSend, "%s", AT_COMMAND_ESTABLISH_GPRS_CONNECTION);
    UART3_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART3_ReadBuffer(bufferRead, 4);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

unsigned char ConnectWithOurServer (void)
{
    char bufferToSend[50];
    char *bufferRead;
    
    sprintf(bufferToSend, "%s", AT_COMMAND_CONNECT_WITH_OUR_SERVER);
    UART3_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART3_ReadBuffer(bufferRead, 4);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

unsigned char CheckServerConnection (void)
{
    char bufferToSend[50];
    char *bufferRead;
    
    sprintf(bufferToSend, "%s", AT_COMMAND_CHECK_SERVER_CONNECTION);
    UART3_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART3_ReadBuffer(bufferRead, 4);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

unsigned char OpenGprsSession (void)
{
    char bufferToSend[50];
    char *bufferRead;
    
    sprintf(bufferToSend, "%s", AT_COMMAND_OPEN_SESSION);
    UART3_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART3_ReadBuffer(bufferRead, 4);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

unsigned char CloseGprsSession (void)
{
    char bufferToSend[50];
    char *bufferRead;
    
    sprintf(bufferToSend, "%s", AT_COMMAND_CLOSE_SESSION);
    UART3_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART3_ReadBuffer(bufferRead, 4);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

unsigned char CloseGprsConnection (void)
{
    char *bufferToSend;
    char *bufferRead;
    
    sprintf(bufferToSend, "%s", AT_COMMAND_CLOSE_GPRS_CONNECTION);
    UART3_WriteBuffer(bufferToSend, strlen(bufferToSend));
    UART3_ReadBuffer(bufferRead, 4);
    
    if (strstr(bufferRead, AT_OK_RESPONSE) != NULL)
    {
        return 0;  
    }
    else
        return 1;

}

unsigned char GprsStartSequence (void)
{
    char bufferToSend[50];
    char *bufferRead;

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

unsigned char GprsReconnectSequence (void)
{
    char bufferToSend[50];
    char *bufferRead;

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