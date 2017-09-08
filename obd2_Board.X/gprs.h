/* 
 * File:  gprs.h 
 * Author: Javier Alcina
 * Comments: Header file for GPRS library
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef GPRS_H
#define	GPRS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "mcc_generated_files/uart3.h" 

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
  
#define AT_COMMAND_CHECK_PIN                        "AT+CPIN?\r\n"
#define AT_COMMAND_CHECK_NETWORK                    "AT+CGREG?\r\n"
#define AT_COMMAND_RETRIEVE_OPERATOR                "AT+COPS?\r\n"
#define AT_COMMAND_CHECK_SIGNAL_QUALITY             "AT+CSQ\r\n"
#define AT_COMMAND_ESTABLISH_GPRS_CONNECTION        "AT+CGATT=1\r\n"
#define AT_COMMAND_CONNECT_WITH_OUR_SERVER          "AT+CGDCONT=1,?IP?,?servidor.com?\r\n"
#define AT_COMMAND_CHECK_SERVER_CONNECTION          "AT+CGDCONT?\r\n"
#define AT_COMMAND_OPEN_SESSION                     "AT+CGACT=1,1\r\n"
#define AT_COMMAND_CLOSE_SESSION                    "AT+CGDCONT?\r\n"
#define AT_COMMAND_CLOSE_GPRS_CONNECTION            "AT+CGATT=0\r\n"
    
#define AT_OK_RESPONSE                              "OK"

#define ERROR_CODE_CHECK_PIN                        2
#define ERROR_CODE_CHECK_NETWORK                    3
#define ERROR_CODE_RETRIEVE_OPERATOR                4
#define ERROR_CODE_CHECK_SIGNAL_QUALITY             5
#define ERROR_CODE_ESTABLISH_GPRS_CONNECTION        6
#define ERROR_CODE_CONNECT_WITH_OUR_SERVER          7
#define ERROR_CODE_CHECK_SERVER_CONNECTION          8
#define ERROR_CODE_OPEN_SESSION                     9
#define ERROR_CODE_CLOSE_SESSION                    10
#define ERROR_CODE_CLOSE_GPRS_CONNECTION            11

void ConfigGprsPort (void);
unsigned char CheckSIM (void);
unsigned char CheckNetwork (void);
unsigned char RetrieveNetworkOperator (void);
unsigned char CheckSignalQuality (void);
unsigned char EstablishGprsConnection (void);
unsigned char CheckServerConnection (void);
unsigned char ConnectWithOurServer (void);
unsigned char OpenGprsSession (void);
unsigned char CloseGprsSession (void);
unsigned char CloseGprsConnection (void);
unsigned char GprsReconnectSequence (void);

unsigned char GprsStartSequence (void);
unsigned char GprsCloseSequence (void);
void SendDataToGprs (char *gprsData);
void Gprs_SendMessageReadResponse (char *bufferIn, char *bufferOut);


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif

