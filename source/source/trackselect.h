#ifndef __TRACKSELECT_H__
#define __TRACKSELECT_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones del menu de seleccion del circuito
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>



	// Funcion TrackSelect();
	extern u8 TrackSelect(void);
	// Seleccion de circuito



	// Funcion LoadTrackSelectFiles();
	void LoadTrackSelectFiles(void);
	// Carga los archivos para el menu de seleccion de circuito



	// Funcion CreateTrackSelectMenu();
	void CreateTrackSelectMenu(void);
	// Crea el menu de seleccion del circuito



	// Funcion TrackSelectSprites();
	void TrackSelectSprites(u8 id);
	// Crea los sprites del menu



	// Funcion TrackSelectMoveMenu();
	extern bool TrackSelectMoveMenu(s16 move_x);
	// Mueve los sprites del menu





#ifdef __cplusplus
}
#endif


#endif