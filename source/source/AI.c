#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de inteligencia artificial
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
	#include "ai.h"
	#include "defines.h"
	#include "system.h"
	#include "physics.h"
	#include "engine.h"





	// Funcion CpuCarsAI();
	void CpuCarsAI(u8 first) {

		// Variables
		u8 n = 0;

		// Obten la posicion de ruedas y ejes
		for (n = first; n < CAR_MAXNUM; n ++) {
			GetCarWeels(n);
		}

		// Conduccion normal
		AINormalDrive(first);

		// Conduccion al adelantar
		AITakeOverDrive(first);

	}



	// Funcion AINormalDrive();
	void AINormalDrive(u8 first) {

		// Variables
		u8 n = 0;		// Uso general

		for (n = first; n < CAR_MAXNUM; n ++) {

			// Resetea los parametros de la IA
			CAR[n].AI_acc = true;
			CAR[n].AI_brake = false;
			CAR[n].AI_right = false;
			CAR[n].AI_left = false;


			// Conducta precognitiva (Giro a la izquierda)

			// Si la linea de vision, toca el sector derecho, gira a la izquierda
			if (CAR[n].kt[2] == TILE_BLUE) {
				CAR[n].AI_left = true;
				if (CAR[n].speed > (4 << 8)) CAR[n].AI_acc = false;
			}
			// Si la linea de vision, sale del circuito, gira a la izquierda y frena
			if ((CAR[n].kt[2] == TILE_YELLOW) || (CAR[n].kt[2] == TILE_GRAY)) {
				CAR[n].AI_left = true;
				if (CAR[n].speed > (4 << 8)) CAR[n].AI_acc = false;
				if (CAR[n].speed > (3 << 8)) CAR[n].AI_brake = true;
			}


			// Conducta precognitiva (Giro a la derecha)
			
			// Si la linea de vision, toca el sector izquierdo, gira a la derecha
			if (CAR[n].kt[2] == TILE_GREEN) {
				CAR[n].AI_right = true;
				if (CAR[n].speed > (4 << 8)) CAR[n].AI_acc = false;
			}
			// Si la linea de vision, sale del circuito, gira a la derecha y frena
			if ((CAR[n].kt[2] == TILE_RED) || (CAR[n].kt[2] == TILE_BLACK)) {
				CAR[n].AI_right = true;
				if (CAR[n].speed > (4 << 8)) CAR[n].AI_acc = false;
				if (CAR[n].speed > (3 << 8)) CAR[n].AI_brake = true;
			}

			
			// Conducta si se sale del circuito
			
			// Si la rueda derecha sale del circuito, limita la velocidad y gira a la izquierda
			if (CAR[n].wt[0] == TILE_YELLOW) {
				CAR[n].AI_left = true;
				CAR[n].AI_right = false;
				if (CAR[n].speed > CAR_MAXMINSPEED) {
					CAR[n].AI_acc = false;
					CAR[n].speed -= (CAR[n].speed >> 7);
				}
			}
			// Si la rueda izquierda sale del circuito, limira la velocidad y gira a la derecha
			if (CAR[n].wt[1] == TILE_RED) {
				CAR[n].AI_right = true;
				CAR[n].AI_left = false;
				if (CAR[n].speed > CAR_MAXMINSPEED) {
					CAR[n].AI_acc = false;
					CAR[n].speed -= (CAR[n].speed >> 7);
				}
			}


		}

	}



	// Funcion AITakeOverDrive();
	void AITakeOverDrive(u8 first) {

		// Variables para el bucle
		s16 id_a = 0;
		s16 id_b = 0;

		// Variables
		s32 distance_x = 0;		// Distancia Eje delantero ID_A / Eje trasero ID_B
		s32 distance_y = 0;		// Distancia Eje delantero ID_A / Eje trasero ID_B


		// Bucles de comprovacion (0 to MAXNUM)
		for (id_a = 0; id_a < (CAR_MAXNUM - 1); id_a ++) {
			for (id_b = (id_a + 1); id_b < CAR_MAXNUM; id_b ++) {
				// Si el coche 0 esta en modo manual, ignoralo
				if ((first == 0) || ((first > 0) && (id_a > 0))) {
					// Estas a la distacia suficiente y a mas velocidad?
					distance_x = (CAR[id_b].x - CAR[id_a].x);
					distance_y = (CAR[id_a].y - CAR[id_b].y);
					if (
						(CAR[id_a].speed >= CAR[id_b].speed)
						&&
						((distance_y < (CAR_HEIGHT << 2)) && (distance_y > -(CAR_HEIGHT << 1)))
						&&
						((distance_x < (CAR_WIDTH << 2)) && (distance_x > -(CAR_WIDTH << 2)))
						) {
							// Si el vehiculo a adelantar, esta mas a la izquierda, gira a la derecha
							if (CAR[id_a].x > CAR[id_b].x) {
								if ((CAR[id_a].wt[0] != TILE_BLUE) && (CAR[id_a].kt[2] != TILE_YELLOW)) {
									CAR[id_a].AI_right = true;
									CAR[id_a].AI_left = false;
									CAR[id_a].AI_acc = true;
									CAR[id_a].AI_brake = false;
								} else {
									// Si no puedes, reduce
									if (distance_y > 0) {
										CAR[id_a].AI_acc = false;
										if (distance_y < ((CAR_HEIGHT << 1) + CAR_HEIGHT)) CAR[id_a].AI_brake = true;
									}
								}
							}
							// Si el veiculo a adelantar, esta mas a la derecha, gira a la izqueirda
							if (CAR[id_a].x <= CAR[id_b].x) {
								if ((CAR[id_a].wt[1] != TILE_GREEN) && (CAR[id_a].kt[2] != TILE_RED)) {
									CAR[id_a].AI_right = false;
									CAR[id_a].AI_left = true;
									CAR[id_a].AI_acc = true;
									CAR[id_a].AI_brake = false;
								} else {
									// Si no puedes, reduce
									if (distance_y > 0) {
										CAR[id_a].AI_acc = false;
										if (distance_y < ((CAR_HEIGHT << 1) + CAR_HEIGHT)) CAR[id_a].AI_brake = true;
									}
								}
							}
					}
				}
			}
		}



		// Bucles de comprovacion (MAXNUM to 0)
		for (id_a = (CAR_MAXNUM - 1); id_a > 0; id_a --) {
			for (id_b = (id_a - 1); id_b > -1; id_b --) {
				// Si el coche 0 esta en modo manual, ignoralo
				if ((first == 0) || ((first > 0) && (id_a > 0))) {
					// Estas a la distacia suficiente y a mas velocidad?
					distance_x = (CAR[id_b].x - CAR[id_a].x);
					distance_y = (CAR[id_a].y - CAR[id_b].y);
					if (
						(CAR[id_a].speed >= CAR[id_b].speed)
						&&
						((distance_y < (CAR_HEIGHT << 2)) && (distance_y > -(CAR_HEIGHT << 1)))
						&&
						((distance_x < (CAR_WIDTH << 2)) && (distance_x > -(CAR_WIDTH << 2)))
						) {
							// Si el vehiculo a adelantar, esta mas a la izquierda, gira a la derecha
							if (CAR[id_a].x > CAR[id_b].x) {
								if ((CAR[id_a].wt[0] != TILE_BLUE) && (CAR[id_a].kt[2] != TILE_YELLOW)) {
									CAR[id_a].AI_right = true;
									CAR[id_a].AI_left = false;
									CAR[id_a].AI_acc = true;
									CAR[id_a].AI_brake = false;
								} else {
									// Si no puedes, reduce
									if (distance_y > 0) {
										CAR[id_a].AI_acc = false;
										if (distance_y < ((CAR_HEIGHT << 1) + CAR_HEIGHT)) CAR[id_a].AI_brake = true;
									}
								}
							}
							// Si el veiculo a adelantar, esta mas a la derecha, gira a la izqueirda
							if (CAR[id_a].x <= CAR[id_b].x) {
								if ((CAR[id_a].wt[1] != TILE_GREEN) && (CAR[id_a].kt[2] != TILE_RED)) {
									CAR[id_a].AI_right = false;
									CAR[id_a].AI_left = true;
									CAR[id_a].AI_acc = true;
									CAR[id_a].AI_brake = false;
								} else {
									// Si no puedes, reduce
									if (distance_y > 0) {
										CAR[id_a].AI_acc = false;
										if (distance_y < ((CAR_HEIGHT << 1) + CAR_HEIGHT)) CAR[id_a].AI_brake = true;
									}
								}
							}
					}
				}
			}
		}


	}



	// Funcion GetCarWeels();
	void GetCarWeels(u8 id) {

		// Variables locales
		s32 radius = 0;		// Radio del circulo
		s32 x = 0;			// Uso general
		s32 y = 0;
		u8 n = 0;
		s32 fcos = 0;		// Calculo de la posicion de los puntos
		s32 fsin = 0;
		s32 angle = 0;
		s32 cx = 0;			// Eje centralde las ruedas
		s32 cy = 0;

		// Linia de vision
		radius = ((CAR_HEIGHT + CAR[id].PARAM_sight) << 8);		// Radio en base 256
		fcos = (cosLerp((CAR[id].angle << 6)) >> 5);
		if (fcos >= 0) {
			cy = (CAR[id].fy - ((radius * fcos) >> 7));
		} else {
			fcos = -fcos;
			cy = (CAR[id].fy + ((radius * fcos) >> 7));
		}
		fsin = (sinLerp((CAR[id].angle << 6)) >> 5);
		if (fsin >= 0) {
			cx = (CAR[id].fx + ((radius * fsin) >> 7));
		} else {
			fsin = -fsin;
			cx = (CAR[id].fx - ((radius * fsin) >> 7));
		}
		CAR[id].kx[2] = (cx >> 8);
		CAR[id].ky[2] = (cy >> 8);
		
		// Ruedas delanteras, calcula el centro
		radius = (CAR_HEIGHT << 8);		// Radio en base 256
		fcos = (cosLerp((CAR[id].angle << 6)) >> 5);
		if (fcos >= 0) {
			cy = (CAR[id].fy - ((radius * fcos) >> 7));
		} else {
			fcos = -fcos;
			cy = (CAR[id].fy + ((radius * fcos) >> 7));
		}
		fsin = (sinLerp((CAR[id].angle << 6)) >> 5);
		if (fsin >= 0) {
			cx = (CAR[id].fx + ((radius * fsin) >> 7));
		} else {
			fsin = -fsin;
			cx = (CAR[id].fx - ((radius * fsin) >> 7));
		}
		CAR[id].kx[0] = (cx >> 8);
		CAR[id].ky[0] = (cy >> 8);

		// Calcula la posicion de las 2 ruedas delanteras
		radius = (CAR_WIDTH << 8);		// Radio en base 256
		/// Delantera derecha [0]
		angle = (CAR[id].angle + 128);
		if (angle > 511) angle -= 512;
		fcos = (cosLerp((angle << 6)) >> 5);
		if (fcos >= 0) {
			y = (cy - ((radius * fcos) >> 7));
		} else {
			fcos = -fcos;
			y = (cy + ((radius * fcos) >> 7));
		}
		CAR[id].wy[0] = (y >> 8);
		fsin = (sinLerp((angle << 6)) >> 5);
		if (fsin >= 0) {
			x = (cx + ((radius * fsin) >> 7));
		} else {
			fsin = -fsin;
			x = (cx - ((radius * fsin) >> 7));
		}
		CAR[id].wx[0] = (x >> 8);
		/// Delantera izquierda [1]
		angle = (CAR[id].angle - 128);
		if (angle < 0) angle += 512;
		fcos = (cosLerp((angle << 6)) >> 5);
		if (fcos >= 0) {
			y = (cy - ((radius * fcos) >> 7));
		} else {
			fcos = -fcos;
			y = (cy + ((radius * fcos) >> 7));
		}
		CAR[id].wy[1] = (y >> 8);
		fsin = (sinLerp((angle << 6)) >> 5);
		if (fsin >= 0) {
			x = (cx + ((radius * fsin) >> 7));
		} else {
			fsin = -fsin;
			x = (cx - ((radius * fsin) >> 7));
		}
		CAR[id].wx[1] = (x >> 8);

		// Ruedas traseras, calcula el centro
		radius = (CAR_HEIGHT << 8);		// Radio en base 256
		angle = (CAR[id].angle + 256);
		if (angle > 511) angle -= 512;
		fcos = (cosLerp((angle << 6)) >> 5);
		if (fcos >= 0) {
			cy = (CAR[id].fy - ((radius * fcos) >> 7));
		} else {
			fcos = -fcos;
			cy = (CAR[id].fy + ((radius * fcos) >> 7));
		}
		fsin = (sinLerp((angle << 6)) >> 5);
		if (fsin >= 0) {
			cx = (CAR[id].fx + ((radius * fsin) >> 7));
		} else {
			fsin = -fsin;
			cx = (CAR[id].fx - ((radius * fsin) >> 7));
		}
		CAR[id].kx[1] = (cx >> 8);
		CAR[id].ky[1] = (cy >> 8);

		// Calcula la posicion de las 2 ruedas traseras
		radius = (CAR_WIDTH << 8);		// Radio en base 256
		/// Trasera derecha [2]
		angle = (CAR[id].angle + 128);
		if (angle > 511) angle -= 512;
		fcos = (cosLerp((angle << 6)) >> 5);
		if (fcos >= 0) {
			y = (cy - ((radius * fcos) >> 7));
		} else {
			fcos = -fcos;
			y = (cy + ((radius * fcos) >> 7));
		}
		CAR[id].wy[2] = (y >> 8);
		fsin = (sinLerp((angle << 6)) >> 5);
		if (fsin >= 0) {
			x = (cx + ((radius * fsin) >> 7));
		} else {
			fsin = -fsin;
			x = (cx - ((radius * fsin) >> 7));
		}
		CAR[id].wx[2] = (x >> 8);
		/// Trasera izquierda [3]
		angle = (CAR[id].angle - 128);
		if (angle < 0) angle += 512;
		fcos = (cosLerp((angle << 6)) >> 5);
		if (fcos >= 0) {
			y = (cy - ((radius * fcos) >> 7));
		} else {
			fcos = -fcos;
			y = (cy + ((radius * fcos) >> 7));
		}
		CAR[id].wy[3] = (y >> 8);
		fsin = (sinLerp((angle << 6)) >> 5);
		if (fsin >= 0) {
			x = (cx + ((radius * fsin) >> 7));
		} else {
			fsin = -fsin;
			x = (cx - ((radius * fsin) >> 7));
		}
		CAR[id].wx[3] = (x >> 8);

		// Obten los tiles de debajo los ejes
		CAR[id].kt[0] = NF_GetPoint(0, CAR[id].kx[0], CAR[id].ky[0]);
		CAR[id].kt[1] = NF_GetPoint(0, CAR[id].kx[1], CAR[id].ky[1]);

		// Y el tile en la line ade vision
		CAR[id].kt[2] = NF_GetPoint(0, CAR[id].kx[2], CAR[id].ky[2]);

		// Ahora obten los tiles de debajo de cada rueda
		for (n = 0; n < 4; n ++) {
			CAR[id].wt[n] = NF_GetPoint(0, CAR[id].wx[n], CAR[id].wy[n]);
		}

	}





#ifdef __cplusplus
}
#endif