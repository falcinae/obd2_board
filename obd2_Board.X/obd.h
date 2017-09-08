/* 
 * File:  obd.h 
 * Author: Javier Alcina
 * Comments: Header file for OBD library
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef OBD_H
#define	OBD_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "mcc_generated_files/uart1.h" 

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
#define CONNECT_WITH_OBD_INTERPRETER    "AT SP 0"

#define OBD_nDTCs_PID           "0101"
#define OBD_READ_DTCs_PID       "03"
#define OBD_TEMP_PID            "0105"
#define OBD_RPM_PID             "010C"
#define OBD_SPEED_PID           "010D"
#define OBD_VIN_PID             "0902"
#define OBD_PRESSURE_PID        "0133"
#define OBD_THROTTLE_PID        "0111"
#define OBD_CLEAN_DTCS_PID      "05"

#define AT_OK_RESPONSE                              "OK"

   
    
typedef struct OBDData {
    char *speedOBD;
    char *rpmOBD;
    char *tempOBD;
    char *throttleOBD;
    char *milLamp;
    char *numDTCs;
    char *vinOBD;
    char *pressureOBD;
    } OBD_Data;
    
extern OBD_Data OBD_Value;

void ConfigObdPort (void);

unsigned char ConnectWithObdInterpreter (void);
void ReadSpeedFromOBD (char *speedOBD);
void ReadRpmFromOBD (char *rpmOBD);
void ReadTemperatureFromOBD (char *tempOBD);
void ReadThrottleFromOBD (char *throttleOBD);
void ReadDTCsFromOBD (char *milLamp, char *numDTCs);
void ReadVINFromOBD (char *vinOBD);
void ReadPressureFromOBD (char *pressureOBD);
void CleanDtcsFromOBD (void);

void RequestDataFromOBD (void);

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif

