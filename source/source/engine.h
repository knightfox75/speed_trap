#ifndef __ENGINE_H__
#define __ENGINE_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones del motor del juego
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion MainEngine();
	void MainEngine(bool run);
	// Rutina principal del juego



	// Funcion ControlPlayerCar();
	void ControlPlayerCar(void);
	// Controla la posicion del jugador



	// Funcion MoveCars();
	void MoveCars(void);
	// Mueve los coches de la CPU



	// Funcion TrackLoop();
	void TrackLoop(void);
	// Controla el LOOP del circuito



	// Funcion ControlTakeOver();
	void ControlTakeOver(void);
	// Verifica si se tiene que tomar el control sobre la IA de algun coche



	// Funcion ReplacePuppetCar();
	void ReplacePuppetCar(void);
	// Si es necesario, recoloca los coches marionetas



	// Funcion DrawTrack();
	void DrawTrack(u8 id);
	// Calcula la posicion de los fondos segun la posicion del coche principal



	// Funcion DrawTrack();
	void DrawCars(u8 id);
	// Calcula la posicion de los coches, segun los fondos



	// Funcion BackupCarPosition(void);
	void BackupCarPosition(void);
	// Guarda la posicion de los coches en el ultimo frame



	// Funcion ReplacePlayerCar();
	void ReplacePlayerCar(void);
	// Recoloca el coche del jugador en la pista





#ifdef __cplusplus
}
#endif


#endif