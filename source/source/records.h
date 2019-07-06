#ifndef __RECORDS_H__
#define __RECORDS_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de control de "Records"
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion CheckTrackRecord();
	extern u8 CheckTrackRecord(u8 stage);
	// Verifica si hay algun record



	// Funcion DisplayResults();
	void DisplayResults(u8 stage);
	// Pantalla de resultados de la carrera





#ifdef __cplusplus
}
#endif


#endif