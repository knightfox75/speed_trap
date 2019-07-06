#ifndef __AUTOPLAY_H__
#define __AUTOPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones Autoplay (modo demo)
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>




	// Funcion AutoPlay();
	void AutoPlay(void);
	// Autoplay del juego (modo demo)


	// Funcion AutoPlayKernel();
	void AutoPlayKernel(void);
	// Nucleo del modo demo



	// Funcion AutoPlayEngine();
	void AutoPlayEngine(void);
	// Engine del modo demo





#ifdef __cplusplus
}
#endif


#endif