#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones del motor del juego
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
	#include "engine.h"
	#include "defines.h"
	#include "system.h"
	#include "physics.h"
	#include "ai.h"
	#include "create.h"
	#include "display.h"





	// Funcion MainEngine();
	void MainEngine(bool run) {

		// Variables
		u8 n = 0;

		// Si los coches pueden moverse...
		if (run) {

			// Control de los coches
			if (TRACK.game_on) {
				// Actualiza los cronometros
				CountUpTimer(TRACK_TIME);
				CountUpTimer((TRACK_LAPS - TRACK.lap));
				CountDownTimer(TRACK_COUNTDOWN);
				// Calcula la posicion del  jugador
				ControlPlayerCar();
				// IA de los coches de la CPU
				CpuCarsAI(1);
			} else {
				// IA de los coches de la CPU
				if (TRACK.result == 0) {
					CpuCarsAI(0);
				} else {
					CAR[0].AI_acc = false;
					CAR[0].AI_brake = false;
					CAR[0].AI_right = false;
					CAR[0].AI_left = false;
					CpuCarsAI(1);
				}
			}

		} else {	// Si el flag de carrera empezada esta abajo

			// Override global de la IA (todos detenidos)
			for (n = 0; n < CAR_MAXNUM; n ++) {
				CAR[n].AI_acc = false;
				CAR[n].AI_brake = false;
				CAR[n].AI_right = false;
				CAR[n].AI_left = false;
			}

		}

		// Take over de la IA (eventos en el anterior frame)
		ControlTakeOver();

		// Mueve los coches
		MoveCars();

		// Calcula las fisicas de los coches
		MainCarPhysics();

		// Calcula las colisiones con el decorado
		CheckObjectsColisions();

		// Control del trazado
		TrackLoop();
		CheckPoints();
		TimeOut();

		// Si es necesario, recoloca los coches marioneta
		ReplacePuppetCar();

		// Dibuja el escenario
		DrawTrack(0);
		DrawCars(0);

		// Guarda las posiciones de los coches al salir del frame
		BackupCarPosition();

		// Actualiza display
		UpdateDisplay();

		// Mensages pop-up pendientes
		PopupMessage();

	}



	// Funcion ControlPlayerCar();
	void ControlPlayerCar(void) {

		// Resetea los parametros de la IA
		CAR[0].AI_acc = false;
		CAR[0].AI_brake = false;
		CAR[0].AI_right = false;
		CAR[0].AI_left = false;

		// Asigna los parametros de la IA segun la pulsacion del jugador
		if (KEYHELD.b) CAR[0].AI_brake = true;			// Frenado (B)
		if (KEYHELD.a) CAR[0].AI_acc = true;			// Aceleracion (A)
		if (KEYHELD.right) CAR[0].AI_right = true;		// Giro derecha ( > )
		if (KEYHELD.left) CAR[0].AI_left = true;		// Giro izquierda ( < )
		if (KEYPRESS.x && (CAR[0].speed == 0)) ReplacePlayerCar();	// X - Reposiciona el coche

		// Obten la posicion de las ruedas y ejes
		GetCarWeels(0);

		// Conducta si se sale del circuito	
		// Si la rueda derecha sale del circuito, frena y gira a la izquierda
		if (CAR[0].wt[0] == TILE_YELLOW) {
			// Limita la velocidad fuera del circuito
			if (CAR[0].speed > CAR_MAXMINSPEED) {
				CAR[0].AI_acc = false;
				CAR[0].speed -= (CAR[0].speed >> 7);
			}
		}
		// Si la rueda izquierda sale del circuito, frena y gira a la derecha
		if (CAR[0].wt[1] == TILE_RED) {
			// Limita la velocidad fuera del circuito
			if (CAR[0].speed > CAR_MAXMINSPEED) {
				CAR[0].AI_acc = false;
				CAR[0].speed -= (CAR[0].speed >> 7);
			}
		}

	}



	// Funcion MoveCars();
	void MoveCars(void) {

		// Variables locales
		s32 fcos = 0;	// Coseno en base 128
		s32 fsin = 0;	// Seno en base 128
		u8 n = 0;		// Uso general


		// Para todos los coches de la CPU
		for (n = 0; n < CAR_MAXNUM; n ++) {

			// Si has presionado el boton de aceleracion (y no se esta pulsando el freno)
			if (!CAR[n].AI_brake) {
				if (CAR[n].AI_acc) {
					CAR[n].speed += CAR[n].PARAM_acc;
					if (CAR[n].speed > CAR[n].PARAM_maxspeed) CAR[n].speed = CAR[n].PARAM_maxspeed;
				} else {
					CAR[n].speed -= CAR[n].PARAM_inertia;
					if (CAR[n].speed < 0) CAR[n].speed = 0;
				}
			}

			// Si has presionado el boton de frenado
			if (CAR[n].AI_brake) {
				CAR[n].speed -= CAR[n].PARAM_brake;
				if (CAR[n].speed < 0) CAR[n].speed = 0;
				// Enciende las luces de freno
				NF_SpriteFrame(1, CAR[n].sprite, 1);
			} else {
				// Apaga las luces de freno
				NF_SpriteFrame(1, CAR[n].sprite, 0);
			}

			// Si el coche se mueve...
			if (CAR[n].speed > 0) {

				// Pasemos a calcular el giro a la derecha
				if (!CAR[n].AI_left) {
					if (CAR[n].AI_right) {
						CAR[n].turn += CAR[n].PARAM_turn;
						if (CAR[n].turn > 512) CAR[n].turn = 512;
						CAR[n].angle += ((CAR[n].turn >> 8) + 1);
						if (CAR[n].angle > CAR_MAXANGLE) CAR[n].angle = CAR_MAXANGLE;	// Maximo angulo de giro 45º
					} else {
						CAR[n].turn = 0;
					}
				}

				// Y el giro a la izquierda
				if (!CAR[n].AI_right) {
					if (CAR[n].AI_left) {
						CAR[n].turn += CAR[n].PARAM_turn;
						if (CAR[n].turn > 512) CAR[n].turn = 512;
						CAR[n].angle -= ((CAR[n].turn >> 8) + 1);
						if (CAR[n].angle < CAR_MINANGLE) CAR[n].angle = CAR_MINANGLE;	// Maximo angulo de giro 45º
					} else {
						CAR[n].turn = 0;
					}
				}

			} else {

				// Si el coche esta detenido, coeficiente de giro 0
				CAR[n].turn = 0;

			}

			// Segun la aceleracion y el giro, mueve el coche
			fcos = (cosLerp((CAR[n].angle << 6)) >> 5);
			if (fcos >= 0) {
				CAR[n].fy -= ((CAR[n].speed * fcos) >> 7);
			}
			if (fcos < 0) {
				fcos = -fcos;
				CAR[n].fy += ((CAR[n].speed * fcos) >> 7);
			}
			fsin = (sinLerp((CAR[n].angle << 6)) >> 5);
			if (fsin >= 0) {
				CAR[n].fx += ((CAR[n].speed * fsin) >> 7);
			}
			if (fsin < 0) {
				fsin = -fsin;
				CAR[n].fx -= ((CAR[n].speed * fsin) >> 7);
			}

		}

	}



	// Funcion TrackLoop();
	void TrackLoop(void) {

		// Variables
		bool doloop = false;
		u8 n = 0;

		// Verifica el coche principal ( CAR[0] )
		if (CAR[0].fy < (TRACK.end << 8)) {
			doloop = true;
			// Reposiciona el coche
			CAR[0].fy += (TRACK.goback << 8);
			// Contador de vueltas
			if (TRACK.lap_flag && (TRACK.lap > 0)) LapsControl();
		}

		// Reposiciona el resto de coches
		for (n = 1; n < CAR_MAXNUM; n ++) {
			// Si el coche principal hace loop
			if (doloop) CAR[n].fy += (TRACK.goback << 8);
			// Manten los coches en los limites del circuito
			if (CAR[n].fy < 0) CAR[n].fy += (TRACK.goback << 8);
			if (CAR[n].fy > (TRACK.height << 8)) CAR[n].fy -= (TRACK.goback << 8);
		}

	}



	// Funcion ControlTakeOver();
	void ControlTakeOver(void) {

		// Variables
		u8 n = 0;		// Uso general

		// Busca si algun coche tiene que sufrir el Take Over de la IA
		for (n = 0; n < CAR_MAXNUM; n ++) {
			if (TAKEOVER[n].timer > 0) {
				CAR[n].AI_left = TAKEOVER[n].AI_left;
				CAR[n].AI_right = TAKEOVER[n].AI_right;
				CAR[n].AI_acc = TAKEOVER[n].AI_acc;
				CAR[n].AI_brake = TAKEOVER[n].AI_brake;
				TAKEOVER[n].timer --;
			}
		}

	}



	// Funcion ReplacePuppetCar();
	void ReplacePuppetCar(void) {

		// Variables locales
		u8 n = 0;
		u8 screen = 0;			// Pantalla
		u8 car = 0;				// Nuevo modelo de coche
		s32 distance = 0;		// Distancia entre coches
		u8 replace = 0;			// Se debe de hacer el reemplazo
		s16 x = 0;				// Nueva coordenada X
		s16 y = 0;				// Nueva coordenada Y
		bool seek_x = true;		// Busca una posicion valida
		bool seek_car = true;	// Busca un nuevo modelo de coche

		// Verifica si tienes que recolocar el PUPPET
		for (n = CAR_PUPPET; n < CAR_MAXNUM; n ++) {

			// Reset de variables
			replace = 0;
			x = 0;
			y = 0;

			// Si el coche esta mas adelantado que el PUPPET
			if (CAR[0].fy < CAR[n].fy) {
				distance = ((CAR[n].fy - CAR[0].fy) >> 8);
				if (distance > 256) replace = 1;	// Fuera de campo, colocalo delante
			} else {	// Si el coche esta mas atrasado que el PUPPET
				distance = ((CAR[0].fy - CAR[n].fy) >> 8);
				if (distance > 640) replace = 2;	// Fuera de campo, colocalo delante
			}

			// Si hay que mover el puppet...
			if (replace > 0) {

				// Nueva posicion
				if (replace == 1) y = ((CAR[0].fy >> 8) - 640);
				if (replace == 2) y = ((CAR[0].fy >> 8) + 256);

				// Resetea los parametros
				CAR[n].angle = 0;
				CAR[n].AI_acc = true;
				CAR[n].AI_brake = false;
				CAR[n].AI_left = false;
				CAR[n].AI_right = false;

				// Busca una posicion X valida
				while (seek_x) {

					// Si la posicion X esta en la carretera...
					if (NF_GetPoint(0, x, y) == TILE_MAGENTA) {
						seek_x = false;
					} else {	// Si no, muevete 4 tiles a la derecha (32 pixeles)
						x += 32;
						// Error trap
						if (x > TRACK.width) {
							x = (TRACK.width >> 2);
							seek_x = false;
						}
					}

				}

				// Borra el Sprite actual
				DeleteCarSprite(n);

				// Busca un nuevo modelo de coche
				seek_car = true;
				while (seek_car) {
					car = (int)(rand() % CAR_TOTALNUM);
					// Si es diferente del coche protagonista y del ultimo coche usado...
					if ((car != CAR[0].model) && (car != CAR[n].model)) seek_car = false;
				}

				// Crea el nuevo sprite
				CreateCarSprite(car, n, x, y);

				// Y asignale la velocidad maxima permitida
				CAR[n].speed = CAR[n].PARAM_maxspeed;

				// Parametros adicionales
				for (screen = 0; screen < 2; screen ++) {
					// Capa donde se dibuja
					NF_SpriteLayer(screen, CAR[n].sprite, 3);
					// Habilita el rotscale del coche
					NF_EnableSpriteRotScale(screen, CAR[n].sprite, CAR[n].rotscale, true);
					// Define el frame por defecto
					NF_SpriteFrame(screen, CAR[n].sprite, 0);
				}

			}

		}

	}


	// Funcion DrawTrack();
	void DrawTrack(u8 id) {

		// Variables
		u8 n = 0;
		s32 fcos = 0;	// Coseno en base 128
		s32 fsin = 0;	// Seno en base 128
		s32 relx = 0;	// Posicion relativa del coche (x)
		s32 rely = 0;	// Posicion relativa del coche (y)

		// Calcula la posicion del coche en el decorado
		CAR[id].x = (CAR[id].fx >> 8);
		CAR[id].y = (CAR[id].fy >> 8);

		// Ahora calcularemos la posicion relativa del coche en la pantalla segun la direccion
		// (Angulo de camara)
		rely = 96;
		fcos = (cosLerp((CAR[id].angle << 6)) >> 5);
		if (fcos >= 0) {
			rely += ((56 * fcos) >> 7);
		}
		if (fcos < 0) {
			fcos = -fcos;
			rely -= ((56 * fcos) >> 7);
		}
		relx = 128;
		fsin = (sinLerp((CAR[id].angle << 6)) >> 5);
		if (fsin >= 0) {
			relx -= ((80 * fsin) >> 7);
		}
		if (fsin < 0) {
			fsin = -fsin;
			relx += ((80 * fsin) >> 7);
		}

		// Calcula la posicion del fondo
		TRACK.x[1] = (CAR[id].x - relx);
		if (TRACK.x[1] < 0) TRACK.x[1] = 0;
		if (TRACK.x[1] > (TRACK.width - 256)) TRACK.x[1] = (TRACK.width - 256);
		TRACK.y[1] = (CAR[id].y - rely);
		if (TRACK.y[1] < 0) TRACK.y[1] = 0;
		if (TRACK.y[1] > (TRACK.height - 192)) TRACK.y[1] = (TRACK.height - 192);

		// Hecho esto, calcula la posicion del coche en pantalla
		CAR[id].sx[1] = ((CAR[id].x - TRACK.x[1]) - CAR_SPRITE_WIDTH);
		CAR[id].sy[1] = ((CAR[id].y - TRACK.y[1]) - CAR_SPRITE_HEIGHT);

		// El coche seleccionado, esta en pantalla
		CAR[n].on_screen[1] = true;

		// En la pantalla superior, este coche SIEMPRE esta oculto
		CAR[id].sx[0] = -64;
		CAR[id].sy[0] = -64;
		CAR[n].on_screen[0] = false;

		// Ahora calcula la posicion de los fondos en la pantalla superior
		TRACK.x[0] = TRACK.x[1];
		TRACK.y[0] = (TRACK.y[1] - 288);
		if (TRACK.y[0] < 0) TRACK.y[0] += TRACK.goback;

	}


	// Funcion DrawTrack();
	void DrawCars(u8 id) {

		// Variables
		u8 n = 0;
		s8 screen = 0;	// Pantalla 0 - 1

		// Calcula la posicion de los coches segun la pantalla
		for (screen = 0; screen < 2; screen ++) {
			// Calcula todos los coches del circuito
			for (n = 0; n < CAR_MAXNUM; n ++) {
				// Excepto si es el coche principal
				if (n != id) {
					// Calcula la posicion del coche en pantalla
					CAR[n].x = (CAR[n].fx >> 8);
					CAR[n].y = (CAR[n].fy >> 8);
					CAR[n].sx[screen] = ((CAR[n].x - TRACK.x[screen]) - CAR_SPRITE_WIDTH);
					CAR[n].sy[screen] = ((CAR[n].y - TRACK.y[screen]) - CAR_SPRITE_HEIGHT);
					// Posiciona el Sprite del coche en la pantalla
					// si esta en el campo de vision de la camara
					if (
						((CAR[n].x - TRACK.x[screen]) < -(CAR_HEIGHT))
						||
						((CAR[n].y - TRACK.y[screen]) < -(CAR_HEIGHT))
						||
						((CAR[n].x - TRACK.x[screen]) > (256 + CAR_HEIGHT))
						||
						((CAR[n].y - TRACK.y[screen]) > (192 + CAR_HEIGHT))
						) {
							// Esconde el Sprite
							CAR[n].sx[screen] = -64;
							CAR[n].sy[screen] = -64;
							CAR[n].on_screen[screen] = false;
					} else {
						CAR[n].on_screen[screen] = true;
					}
				}
			}
		}

		// Una vez completados los calculos, coloca lo necesario en pantalla
		// Calcula la posicion de los coches segun la pantalla
		for (screen = 0; screen < 2; screen ++) {
			// Calcula todos los coches del circuito
			for (n = 0; n < CAR_MAXNUM; n ++) {
				// Giro del coche
				NF_SpriteRotScale(screen, CAR[n].rotscale, CAR[n].angle, 256, 256);
				// Posiciona el Sprite del coche en la pantalla
				NF_MoveSprite(screen, CAR[n].sprite, CAR[n].sx[screen], CAR[n].sy[screen]);
				// Y su sombra
				NF_MoveSprite(screen, (CAR[n].sprite + CAR_MAXNUM), (CAR[n].sx[screen] + 4), (CAR[n].sy[screen] + 4));
				// El coche se muestra?
				NF_ShowSprite(screen, CAR[n].sprite, CAR[n].on_screen[screen]);
				// Efecto de la sombra
				if (screen == 1) CAR[n].shadow = (!CAR[n].shadow);
				NF_ShowSprite(screen, (CAR[n].sprite + CAR_MAXNUM), (CAR[n].shadow & CAR[n].on_screen[screen]));
			}
		}

	}



	// Funcion BackupCarPosition();
	void BackupCarPosition(void) {

		// Variables
		u8 n = 0;

		// Guarda las posiciones del ultimo frame
		for (n = 0; n < CAR_MAXNUM; n ++) {
			CAR[n].old_fx = CAR[n].fx;
			CAR[n].old_fy = CAR[n].fy;
			CAR[n].old_angle = CAR[n].angle;
		}

	}



	// Funcion ReplacePlayerCar();
	void ReplacePlayerCar(void) {

		// Variables
		s32 x = 0;
		s32 y = 0;
		bool seek = true;

		// Resetea las variables
		CAR[0].AI_left = false;
		CAR[0].AI_right = false;
		CAR[0].AI_acc = false;
		CAR[0].AI_brake = false;
		CAR[0].speed = 0;
		CAR[0].angle = 0;

		// Almacena las variables de posicion
		y = (CAR[0].fy >> 8);

		// Busca una posicion X valida
		while (seek) {
			// Si la posicion X esta en la carretera...
			if (NF_GetPoint(0, x, y) == TILE_MAGENTA) {
				seek = false;
			} else {	// Si no, muevete 4 tiles a la derecha (32 pixeles)
				x += 32;
				// Error trap
				if (x > TRACK.width) {
					x = (TRACK.width >> 2);
					seek = false;
				}
			}
		}

		// Reasigna la posicion al coche
		CAR[0].fx = (x << 8);
		CAR[0].fy = (y << 8);

	}





#ifdef __cplusplus
}
#endif