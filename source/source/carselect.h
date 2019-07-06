#ifndef __CARSELECT_H__
#define __CARSELECT_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones del menu de seleccion de los coches
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>



	// Funcion CarSelect();
	extern u8 CarSelect(void);
	// Seleccion de circuito



	// Funcion LoadCarSelectFiles();
	void LoadCarSelectFiles(void);
	// Carga los archivos para el menu de seleccion de circuito



	// Funcion CreateCarSelectMenu();
	void CreateCarSelectMenu(void);
	// Crea el menu de seleccion del circuito



	// Funcion CarSelectSprites();
	void CarSelectSprites(u8 id);
	// Crea los sprites del menu



	// Funcion CarSelectMoveMenu();
	extern bool CarSelectMoveMenu(s16 move_x);
	// Mueve los sprites del menu



	// Funcion SetStatusBar();
	void SetStatusBar(u8 bar, u8 value);
	// Cambia el valor de una barra de estado





#ifdef __cplusplus
}
#endif


#endif