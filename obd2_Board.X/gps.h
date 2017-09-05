/* 
 * File:  gps.h 
 * Author: Javier Alcina
 * Comments: Header file for GPS library
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef GPS_H
#define	GPS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "mcc_generated_files/uart2.h" 

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
void ConfigGpsPort (void);
void ReadGpsGprmcCommand (char *gpsDate, char *gpsTime,
                            char *gpsLatitude, char *northSouth,
                            char *gpsLongitude, char *eastWest);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif

