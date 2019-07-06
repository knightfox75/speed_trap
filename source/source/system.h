#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de sistema
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>
	




	// Funcion BootGame();
	void BootGame(void);
	// Inicializa el engine del juego



	// Funcion ReadKeypad();
	void ReadKeypad(void);
	// Lee el keypad y actualiza el array de datos del mismo



	// Funcion ReadTouchpad();
	void ReadTouchpad(void);
	// Lee el TOUCHPAD y actualiza el array de datos del mismo



	// Funcion FadeIn();
	void FadeIn(u8 screen, u16 speed);
	// Efecto FadeIn en ambas pantallas
	// Toma el control del programa hasta finalizar la ejecuccion



	// Funcion FadeOut();
	void FadeOut(u8 screen, u16 speed);
	// Efecto FadeOut en ambas pantallas
	// Toma el control del programa hasta finalizar la ejecuccion



	// Funcion WaitTime();
	void WaitTime(u16 wait);
	// Espera los frames indicados
	// Toma el control del programa hasta finalizar la ejecucion



	// Funcion ResetAll();
	void ResetAll(void);
	// Vacia todos los buffers



	// Funcion UpdateBothScreens();
	void UpdateBothScreens(void);
	// Actualiza todo el contenido de ambas pantallas, incluida
	// la espera del sincronismo vertical



	// Funcion GetAngle();
	extern s16 GetAngle(s16 x1, s16 y1, s16 x2, s16 y2);
	// Devuelve el angulo respecto a la horizontal de dos puntos (-512 / 512)





#ifdef __cplusplus
}
#endif


#endif