#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de calculo de fisicas
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>





	// CAR 1	[2][1][0] [2][1][0] [2][1][0]
	// BITS		[8][7][6] [5][4][3] [2][1][0]
	// CAR 0	   [2]		 [1]	   [0]





	// Funcion CheckCarsColisions();
	extern u16 CheckCarsColisions(u8 car1, u8 car2);
	// Verifica si dos coches han colisionado entre ellos, devolviendo que partes
	// lo han hecho
	// [000][000][000] 9 ultimos bits
	// Se ponen a 1 lo bits correspondientes a los circulos que colisionan




	// Funcion GetCarVectors();
	void GetCarVectors(u8 id);
	// Obten las coordenadas de los tres puntos de referencia de cada coche



	// Funcion MainCarPhysics
	void MainCarPhysics(void);
	// Calcula todas la fisicas de los coches entre ellos



	// Funcion CarPhysicsFrontRear();
	// Colisiones Frontal -> Trasera
	extern u8 CarPhysicsFrontRear(s16 id_a, s16 id_b);



	// Funcion CarPhysicsFrontMid();
	// Colisiones Frontal -> Media
	extern u8 CarPhysicsFrontMid(s16 id_a, s16 id_b);



	// Funcion CarPhysicsFrontFront();
	// Colisiones Frontal -> Frontal
	extern u8 CarPhysicsFrontFront(s16 id_a, s16 id_b);



	// Funcion CheckObjectsColisions();
	void CheckObjectsColisions(void);
	// Colisiones con los objetos del decorado





#ifdef __cplusplus
}
#endif


#endif