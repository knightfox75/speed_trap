#ifndef __MENU_H__
#define __MENU_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de menu y splashscreens
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>




	// Funcion SplashScreens();
	void SplashScreens(void);
	// Muestra las splashscreens



	// Funcion CrossDisolve();
	void CrossDisolve(u8 screen, u8 origin, u8 destination);
	// Efecto cross disolve entre pantallas



	// Funcion Main Menu
	extern u8 MainMenu(void);
	// Menu principal del juego



	// Funcion MenuFlag();
	void MenuFlag(void);
	// Efecto Modo 7 en la bandera del menu
	




#ifdef __cplusplus
}
#endif


#endif