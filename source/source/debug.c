#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de debug
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010



	// Includes c/c++
	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>

	// Includes propietarios NDS
	#include <nds.h>

	// Includes NightFox's Lib
	#include <nf_lib.h>

	// Includes del juego
	#include "debug.h"


	// Init debug screen
	void InitDebugScreen(u8 screen) {

		NF_Set2D(screen, 0);
		NF_InitTiledBgSys(screen);
		NF_InitTextSys(screen);

		NF_LoadTextFont("font/default", "debug", 256, 256, 0);
		NF_CreateTextLayer(screen, 0, 0, "debug");
		
	}



#ifdef __cplusplus
}
#endif