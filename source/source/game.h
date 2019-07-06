#ifndef __GAME_H__
#define __GAME_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de control del juego
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion Kernel();
	void Kernel(void);
	// Nucleo principal de ejecucion del juego



	// Funcion PrepareRace();
	void PrepareRace(u8 stage, u8 player);
	// Inicializa la carrera



	// Funcion RunRace();
	extern u8 RunRace(u8 stage);
	// Ejecuta al carrera seleccionada






#ifdef __cplusplus
}
#endif


#endif