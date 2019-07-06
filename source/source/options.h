#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de menu de opciones
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>



	// Funcion OptionsMenu();
	extern u8 OptionsMenu(void);
	// Kernel del menu de opciones



	// Funcion LoadOptionsMenuFiles();
	void LoadOptionsMenuFiles(void);
	// Carga los archivos del menu de opciones



	// Funcion CreateOptionsMenu();
	void CreateOptionsMenu(void);
	// Crea el menu de opciones





#ifdef __cplusplus
}
#endif


#endif