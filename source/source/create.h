#ifndef __CREATE_H__
#define __CREATE_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de creacion de contenidos
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion CreateStage();
	void CreateStage(u8 stage, u8 player);
	// Crea el stage



	// Funcion CreateTrack();
	void CreateTrack(u8 stage);
	// Crea los decorados de la fase indicada



	// Funcion CreateCars
	void CreateCars(u8 stage, u8 player);
	// Crea los coches



	// Funcion CreateCarSprite();
	void CreateCarSprite(u8 car, u8 id, s32 x, s32 y);
	// Crea un coche en concreto



	// Funcion DeleteCarSprite();
	void DeleteCarSprite(u8 id);
	// Borra un coche en concreto



	// Funcion CreateMessageSprites();
	void CreateMessageSprites(void);
	// Crea los sprites para mostrar mensages



	// Funcion CreateSpeedometer();
	void CreateSpeedometer(void);
	// Crea el velocimetro


	// Funcion CreateSemaphore();
	void CreateSemaphore(void);
	// Crea el semaforo



	// Funcion DeleteSemaphore();
	void DeleteSemaphore(void);
	// Borra el semaforo



	// Funcion GetRandomCar();
	extern u8 GetRandomCar(void);
	// Obten un ID de coche para la IA






#ifdef __cplusplus
}
#endif


#endif