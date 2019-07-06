#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de calculo de fisicas
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
	#include "physics.h"
	#include "defines.h"
	#include "engine.h"
	#include "ai.h"





	// CAR 1	[2][1][0] [2][1][0] [2][1][0]
	// BITS		[8][7][6] [5][4][3] [2][1][0]
	// CAR 0	   [2]		 [1]	   [0]





	// Funcion CheckCarsColisions();
	u16 CheckCarsColisions(u8 car0, u8 car1) {

		// Variables locales
		u8 n = 0;			// Variable general
		u8 m = 0;			
		u8 id = 0;			// Id. del coche
		u16 colision = 0;	// Resultado de las colisiones
		s32 diameter = 0 ;	// Diametro del circulo
		s32 xside = 0;		// Cateto del Triangulo rectangulo
		s32 yside = 0;

		// Calcula el rango de la colision
		diameter = (CAR_COLISION_RADIUS << 1);	// Diametro en pixeles

		// Ahora calcula si los circulos te tocan
		// N = Coche 0   -   M = Coche 1
		// Los circulos van de superior a inferior
		for (n = 0; n < 3; n ++) {
			for (m = 0; m < 3; m ++) {
				// Calcula el lado X
				xside = (CAR[car0].cx[n] - CAR[car1].cx[m]);
				if (xside < 0) xside = -xside;
				// Calcula el lado Y
				yside = (CAR[car0].cy[n] - CAR[car1].cy[m]);
				if (yside < 0) yside = -yside;
				// Ahora calcula la distancia entre circulos
				if (sqrt32((xside * xside) + (yside * yside)) < diameter) {
					// Si hay colision, marcalo
					// En este formato
					//			  Detras	Centro	 Delante
					// CAR 1	[2][1][0] [2][1][0] [2][1][0]
					// BITS		[8][7][6] [5][4][3] [2][1][0]
					// CAR 0	   [2]		 [1]	   [0]
					id = ((n * 3) + m);
					colision |= BIT(id);
				}
			}
		}

		// Devuelve el resultado
		return colision;

	}



	// Funcion GetCarVectors();
	void GetCarVectors(u8 id) {

		// Variables locales
		s32 radius = 0;		// Radio del circulo
		s32 x = 0;			// Uso general
		s32 y = 0;
		s32 fcos = 0;		// Calculo de la posicion de los puntos
		s32 fsin = 0;
		s32 angle = 0;
		
		// Calcula la posicion de los tres puntos claves del coche
		radius = (CAR_COLISION_RADIUS << 8);	// Radio en base 256

		// Circulo central [1]
		CAR[id].cx[1] = (CAR[id].fx >> 8);
		CAR[id].cy[1] = (CAR[id].fy >> 8);
		// Circulo superior [0]
		fcos = (cosLerp((CAR[id].angle << 6)) >> 5);
		if (fcos >= 0) {
			y = (CAR[id].fy - ((radius * fcos) >> 7));
		} else {
			fcos = -fcos;
			y = (CAR[id].fy + ((radius * fcos) >> 7));
		}
		CAR[id].cy[0] = (y >> 8);
		fsin = (sinLerp((CAR[id].angle << 6)) >> 5);
		if (fsin >= 0) {
			x = (CAR[id].fx + ((radius * fsin) >> 7));
		} else {
			fsin = -fsin;
			x = (CAR[id].fx - ((radius * fsin) >> 7));
		}
		CAR[id].cx[0] = (x >> 8);
		// Circulo inferior [2]
		angle = (CAR[id].angle + 256);
		if (angle > 511) angle -= 512;
		fcos = (cosLerp((angle << 6)) >> 5);
		if (fcos >= 0) {
			y = (CAR[id].fy - ((radius * fcos) >> 7));
		} else {
			fcos = -fcos;
			y = (CAR[id].fy + ((radius * fcos) >> 7));
		}
		CAR[id].cy[2] = (y >> 8);
		fsin = (sinLerp((angle << 6)) >> 5);
		if (fsin >= 0) {
			x = (CAR[id].fx + ((radius * fsin) >> 7));
		} else {
			fsin = -fsin;
			x = (CAR[id].fx - ((radius * fsin) >> 7));
		}
		CAR[id].cx[2] = (x >> 8);

	}



	// Funcion MainCarPhysics
	void MainCarPhysics(void) {

		// Variables para el bucle
		s16 id_a = 0;
		s16 id_b = 0;
		s32 distance = 0;

		// Bucles de comprovacion (0 to MAXNUM)
		for (id_a = 0; id_a < (CAR_MAXNUM - 1); id_a ++) {
			for (id_b = (id_a + 1); id_b < CAR_MAXNUM; id_b ++) {
				distance = (CAR[id_a].y - CAR[id_b].y);
				if (distance < 0) distance = -distance;
				if (distance < ((CAR_HEIGHT << 1) + (CAR_HEIGHT >> 1))) {
					CarPhysicsFrontRear(id_a, id_b);
					CarPhysicsFrontMid(id_a, id_b);
					CarPhysicsFrontFront(id_a, id_b);
				}
			}
		}

		// Bucles de comprovacion (MAXNUM to 0)
		for (id_a = (CAR_MAXNUM - 1); id_a > 0; id_a --) {
			for (id_b = (id_a - 1); id_b > -1; id_b --) {
				distance = (CAR[id_a].y - CAR[id_b].y);
				if (distance < 0) distance = -distance;
				if (distance < ((CAR_HEIGHT << 1) + (CAR_HEIGHT >> 1))) {
					CarPhysicsFrontRear(id_a, id_b);
					CarPhysicsFrontMid(id_a, id_b);
					CarPhysicsFrontFront(id_a, id_b);
				}
			}
		}

	}



	// Funcion CarPhysicsFrontRear();
	// Colisiones Frontal -> Trasera
	u8 CarPhysicsFrontRear(s16 id_a, s16 id_b) {

		// Variables locales
		u16 bits = 0;		// Resultado de la colision en BITS
		u8 colisions = 0;	// Numero de colisiones registradas
		bool check = true;	// Flag de test de colisiones
		s32 fcos = 0;		// Coseno en base 128
		s32 fsin = 0;		// Seno en base 128

		while (check) {

			// Flag abajo
			check = false;

			// Obten los nuevos vectores
			GetCarVectors(id_a);
			GetCarVectors(id_b);

			// Obten los BITS de colision
			bits = CheckCarsColisions(id_a, id_b);
			// Si has golpeado al coche por detras
			if ((bits & BIT(2)) && !(bits & BIT(1))) {
				// Desplaza el coche DESTINO en la direccion y fuerza del de ORIGEN
				// Segun la aceleracion y el giro, mueve el coche
				fcos = (cosLerp((CAR[id_a].angle << 6)) >> 5);
				if (fcos >= 0) {
					CAR[id_b].fy -= ((128 * fcos) >> 7);
				}
				if (fcos < 0) {
					fcos = -fcos;
					CAR[id_b].fy += ((128 * fcos) >> 7);
				}
				fsin = (sinLerp((CAR[id_a].angle << 6)) >> 5);
				if (fsin >= 0) {
					CAR[id_b].fx += ((128 * fsin) >> 7);
				}
				if (fsin < 0) {
					fsin = -fsin;
					CAR[id_b].fx -= ((128 * fsin) >> 7);
				}

				// Reduce la velocidad del coche por el impacto
				if (colisions == 0) CAR[id_a].speed -= (CAR[id_a].speed >> 3);

				// Marca la colision
				colisions = 1;

				// Y repite el test para asegurar que no existen mas colisiones
				check = true;

			}

		}


		// Si han ocurrido colisiones...
		if (colisions) {

			// Define el take over
			TAKEOVER[id_a].timer = 10;
			TAKEOVER[id_a].AI_acc = false;
			TAKEOVER[id_a].AI_brake = true;
			TAKEOVER[id_b].timer = 10;
			TAKEOVER[id_b].AI_acc = true;
			TAKEOVER[id_b].AI_brake = false;

			// Altera el angulo del veiculo de destino
			if (CAR[id_b].angle < CAR[id_a].angle) {
				TAKEOVER[id_a].AI_right = false;
				TAKEOVER[id_a].AI_left = false;
				TAKEOVER[id_b].AI_right = false;
				TAKEOVER[id_b].AI_left = true;
			}
			if (CAR[id_b].angle > CAR[id_a].angle) {
				TAKEOVER[id_a].AI_right = false;
				TAKEOVER[id_a].AI_left = false;
				TAKEOVER[id_b].AI_right = true;
				TAKEOVER[id_b].AI_left = false;
			}

		}

		// Devuelve cuantas colisiones se han registrado
		return colisions;
		
	}



	// Funcion CarPhysicsFrontMid();
	// Colisiones Frontal -> Media
	u8 CarPhysicsFrontMid(s16 id_a, s16 id_b) {

		// Variables locales
		u16 bits = 0;		// Resultado de la colision en BITS
		u8 colisions = 0;	// Numero de colisiones registradas
		bool check = true;	// Flag de test de colisiones
		s32 fcos = 0;		// Coseno en base 128
		s32 fsin = 0;		// Seno en base 128

		while (check) {

			// Flag abajo
			check = false;

			// Obten los nuevos vectores
			GetCarVectors(id_a);
			GetCarVectors(id_b);

			// Obten los BITS de colision
			bits = CheckCarsColisions(id_a, id_b);
			// Si has golpeado al coche por detras
			if ((bits & BIT(1)) && !(bits & BIT(0))) {
				// Desplaza el coche DESTINO en la direccion y fuerza del de ORIGEN
				// Segun la aceleracion y el giro, mueve el coche
				fcos = (cosLerp((CAR[id_a].angle << 6)) >> 5);
				if (fcos >= 0) {
					CAR[id_b].fy -= ((128 * fcos) >> 7);
				}
				if (fcos < 0) {
					fcos = -fcos;
					CAR[id_b].fy += ((128 * fcos) >> 7);
				}
				fsin = (sinLerp((CAR[id_a].angle << 6)) >> 5);
				if (fsin >= 0) {
					CAR[id_b].fx += ((128 * fsin) >> 7);
				}
				if (fsin < 0) {
					fsin = -fsin;
					CAR[id_b].fx -= ((128 * fsin) >> 7);
				}

				// Reduce la velocidad del coche por el impacto
				if (colisions == 0) {
					CAR[id_a].speed -= (CAR[id_a].speed / 10);
					CAR[id_b].speed -= (CAR[id_b].speed / 12);
				}

				// Marca la colision
				colisions = 1;

				// Y repite el test para asegurar que no existen mas colisiones
				check = true;

			}

		}


		// Si han ocurrido colisiones...
		if (colisions) {

			// Define el take over
			TAKEOVER[id_a].timer = 5;
			TAKEOVER[id_a].AI_acc = false;
			TAKEOVER[id_a].AI_brake = false;
			TAKEOVER[id_b].timer = 5;
			TAKEOVER[id_b].AI_acc = false;
			TAKEOVER[id_b].AI_brake = false;

			// Altera el angulo del veiculo de destino
			if (CAR[id_a].fx > CAR[id_b].fx) {
				TAKEOVER[id_a].AI_right = true;
				TAKEOVER[id_a].AI_left = false;
				TAKEOVER[id_b].AI_right = false;
				TAKEOVER[id_b].AI_left = true;
			}
			if (CAR[id_a].fx < CAR[id_b].fx) {
				TAKEOVER[id_a].AI_right = false;
				TAKEOVER[id_a].AI_left = true;
				TAKEOVER[id_b].AI_right = true;
				TAKEOVER[id_b].AI_left = false;
			}


		}

		// Devuelve cuantas colisiones se han registrado
		return colisions;
		
	}



	// Funcion CarPhysicsFrontFront();
	// Colisiones Frontal -> Frontal
	u8 CarPhysicsFrontFront(s16 id_a, s16 id_b) {

		// Variables locales
		u16 bits = 0;		// Resultado de la colision en BITS
		u8 colisions = 0;	// Numero de colisiones registradas
		bool check = true;	// Flag de test de colisiones
		s32 fcos = 0;		// Coseno en base 128
		s32 fsin = 0;		// Seno en base 128

		while (check) {

			// Flag abajo
			check = false;

			// Obten los nuevos vectores
			GetCarVectors(id_a);
			GetCarVectors(id_b);

			// Obten los BITS de colision
			bits = CheckCarsColisions(id_a, id_b);
			// Si has golpeado al coche por detras
			if (bits & BIT(0)) {
				// Desplaza el coche DESTINO en la direccion y fuerza del de ORIGEN
				// Segun la aceleracion y el giro, mueve el coche
				fcos = (cosLerp((CAR[id_a].angle << 6)) >> 5);
				if (fcos >= 0) {
					CAR[id_b].fy -= ((128 * fcos) >> 7);
				}
				if (fcos < 0) {
					fcos = -fcos;
					CAR[id_b].fy += ((128 * fcos) >> 7);
				}
				fsin = (sinLerp((CAR[id_a].angle << 6)) >> 5);
				if (fsin >= 0) {
					CAR[id_b].fx += ((128 * fsin) >> 7);
				}
				if (fsin < 0) {
					fsin = -fsin;
					CAR[id_b].fx -= ((128 * fsin) >> 7);
				}

				// Reduce la velocidad del coche por el impacto
				if (colisions == 0) {
					CAR[id_a].speed -= (CAR[id_a].speed / 12);
					CAR[id_b].speed -= (CAR[id_b].speed / 12);
				}

				// Marca la colision
				colisions = 1;

				// Y repite el test para asegurar que no existen mas colisiones
				check = true;

			}

		}


		// Si han ocurrido colisiones...
		if (colisions) {

			// Define el take over
			TAKEOVER[id_a].timer = 10;
			TAKEOVER[id_a].AI_acc = false;
			TAKEOVER[id_a].AI_brake = false;
			TAKEOVER[id_b].timer = 5;
			TAKEOVER[id_b].AI_acc = false;
			TAKEOVER[id_b].AI_brake = false;

			// Altera el angulo del veiculo de destino
			if (CAR[id_a].fx > CAR[id_b].fx) {
				TAKEOVER[id_a].AI_right = true;
				TAKEOVER[id_a].AI_left = false;
				TAKEOVER[id_b].AI_right = false;
				TAKEOVER[id_b].AI_left = true;
			}
			if (CAR[id_a].fx < CAR[id_b].fx) {
				TAKEOVER[id_a].AI_right = false;
				TAKEOVER[id_a].AI_left = true;
				TAKEOVER[id_b].AI_right = true;
				TAKEOVER[id_b].AI_left = false;
			}

		}

		// Devuelve cuantas colisiones se han registrado
		return colisions;
		
	}



	// Funcion CheckObjectsColisions();
	void CheckObjectsColisions(void) {

		// Variables
		u8 n = 0;

		// Verifica las las posibles colisiones
		for (n = 0; n < CAR_MAXNUM; n ++) {

			// Obten la posicion de las ruedas
			GetCarWeels(n);

			// Si la rueda derecha toca los limites del circuito, limita la velocidad y gira a la izquierda
			while (CAR[n].wt[0] == TILE_WHITE) {
				CAR[n].AI_left = true;
				CAR[n].AI_right = false;
				if (CAR[n].speed > CAR_MAXMINSPEED) CAR[n].speed = CAR_MAXMINSPEED;
				CAR[n].fx -= 256;
				CAR[n].angle -= 4;
				if (CAR[n].angle < CAR_MINANGLE) CAR[n].angle = CAR_MINANGLE;
				GetCarWeels(n);
			}

			// Si la rueda izquierda toca los limites del circuito, limita la velocidad y gira a la izquierda
			while (CAR[n].wt[1] == TILE_WHITE) {
				CAR[n].AI_right = true;
				CAR[n].AI_left = false;
				if (CAR[n].speed > CAR_MAXMINSPEED) CAR[n].speed = CAR_MAXMINSPEED;
				CAR[n].fx += 256;
				CAR[n].angle += 4;
				if (CAR[n].angle > CAR_MAXANGLE) CAR[n].angle = CAR_MAXANGLE;
				GetCarWeels(n);
			}

			// Si la rueda derecha toca un objeto, restaura la posicion y paralo en seco
			if ((CAR[n].wt[0] == TILE_GRAY) || (CAR[n].wt[0] == TILE_BLACK)) {
				CAR[n].AI_left = false;
				CAR[n].AI_right = false;
				CAR[n].AI_acc = false;
				CAR[n].AI_brake = false;
				CAR[n].speed = 0;
				CAR[n].fx = CAR[n].old_fx;
				CAR[n].fy = CAR[n].old_fy;
				CAR[n].angle = CAR[n].old_angle;
				CAR[n].angle -= 4;
				if (CAR[n].angle < CAR_MINANGLE) CAR[n].angle = CAR_MINANGLE;
			}

			// Si la rueda izquierda toca un objeto, restaura la posicion y paralo en seco
			if ((CAR[n].wt[1] == TILE_GRAY) || (CAR[n].wt[1] == TILE_BLACK)) {
				CAR[n].AI_left = false;
				CAR[n].AI_right = false;
				CAR[n].AI_acc = false;
				CAR[n].AI_brake = false;
				CAR[n].speed = 0;
				CAR[n].fx = CAR[n].old_fx;
				CAR[n].fy = CAR[n].old_fy;
				CAR[n].angle = CAR[n].old_angle;
				CAR[n].angle += 4; 
				if (CAR[n].angle > CAR_MAXANGLE) CAR[n].angle = CAR_MAXANGLE;
			}

		}

	}



#ifdef __cplusplus
}
#endif