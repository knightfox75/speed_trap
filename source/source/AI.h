#ifndef __AI_H__
#define __AI_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de inteligencia artificial
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion CpuCarsAI();
	void CpuCarsAI(u8 first);
	// Decide quedebe hacer cada coche controlado por la CPU



	// Funcion AINormalDrive();
	void AINormalDrive(u8 first);
	// Comportamiento de la IA en una conduccion normal.



	// Funcion AITakeOverDrive();
	void AITakeOverDrive(u8 first);
	// Comportamiento de la IA adelantando.



	// Funcion GetCarWeels();
	void GetCarWeels(u8 id);
	// Obten las coordenadas de las ruedas de cada coche





#ifdef __cplusplus
}
#endif


#endif