/*************************************************************************
Nombre del fichero: 	gps.h
Tipo de fichero: 		Header File
Fecha de creacion: 		17-Abril-2017
Ultima modificacion: 	17-Abril-2017
Compañia:				Universidad de Cádiz
Responsable: 			Javier Alcina
 
Proposito:
En este fichero se encuentran las definiciones de la libreria GPS
Lista de modificaciones:
************************************************************************/

 
#ifndef GPS_H
#define	GPS_H

#include <xc.h> 
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

