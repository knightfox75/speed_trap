#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de temporizadores
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
	#include "display.h"
	#include "defines.h"




	// Funcion SetTimer();
	void SetTimer(u8 id, s8 minutes, s8 seconds, s8 hundredths) {

		LAPTIME[id].ticks = 0;
		LAPTIME[id].hundredths = hundredths;
		LAPTIME[id].seconds = seconds;
		LAPTIME[id].minutes = minutes;

	}



	// Funcion CopyTimer();
	void CopyTimer(u8 id_a, u8 id_b) {

		LAPTIME[id_b].ticks = LAPTIME[id_a].ticks;
		LAPTIME[id_b].hundredths = LAPTIME[id_a].hundredths;
		LAPTIME[id_b].seconds = LAPTIME[id_a].seconds;
		LAPTIME[id_b].minutes = LAPTIME[id_a].minutes;

	}



	// Funcion CountUpTimer();
	void CountUpTimer(u8 id) {

		// Resta los ticks necesarios
		LAPTIME[id].ticks += 427;

		// Si has superado las 100 milesimas
		if (LAPTIME[id].ticks > 25620) {
			LAPTIME[id].ticks = 0;
			LAPTIME[id].seconds ++;
			// Si has llegado a los 60 segundos
			if (LAPTIME[id].seconds > 59) {
				LAPTIME[id].seconds = 0;
				LAPTIME[id].minutes ++;
				// Si has llegado a los 120 minutos, resetealo
				if (LAPTIME[id].minutes > 99) LAPTIME[id].minutes = 99;
			}
		}

		// Obten las centesimas
		LAPTIME[id].hundredths = (LAPTIME[id].ticks >> 8);
		if (LAPTIME[id].hundredths > 99) LAPTIME[id].hundredths = 99;

	}



	// Funcion CountDownTimer();
	void CountDownTimer(u8 id) {

		// Resta los ticks necesarios
		LAPTIME[id].ticks -= 427;

		// Si has superado las 100 milesimas (regresivo)
		if (LAPTIME[id].ticks < 0) {
			if ((LAPTIME[id].minutes > 0) || (LAPTIME[id].seconds > 0)) {
				LAPTIME[id].ticks = 25620;
				LAPTIME[id].seconds --;
			} else {
				LAPTIME[id].ticks = 0;
			}
			// Si has llegado a los 60 segundos (regresivo)
			if (LAPTIME[id].seconds < 0) {
				LAPTIME[id].seconds = 59;
				LAPTIME[id].minutes --;
				// Si has llegado a los 0 minutos, limitalo a 0
				if (LAPTIME[id].minutes < 0) LAPTIME[id].minutes = 0;
			}
		}

		// Obten las centesimas
		LAPTIME[id].hundredths = (LAPTIME[id].ticks >> 8);
		if (LAPTIME[id].hundredths > 99) LAPTIME[id].hundredths = 99;

	}



	// Funcion LapsControl();
	void LapsControl(void) {

		// Si pasas por meta con la carrera aun por finalizar
		if (TRACK.game_on) {

			BestLap();						// Calcula si se ha realizado la mejor vuelta
			TRACK.lap_flag = false;			// Flag de control de vuelta abajo
			TRACK.lap --;					// Resta una vuelta

			// Si no era la ultima vuelta....
			if (TRACK.lap > 0) {

				// Flag de siguiente check point
				TRACK.checkpoint_flag = true;
				// Time extended
				EncueMessage(3, 0);
				// Mensage de vueltas restantes
				switch (TRACK.lap) {
					case 3:		// 3 Laps to Go
						EncueMessage(0, 0);
						break;
					case 2:		// 2 Laps to Go
						EncueMessage(1, 0);
						break;
					case 1:		// Final Lap
						EncueMessage(2, 0);
						break;
				}

				// Añade tiempo al contador
				TimeExtended();

			} else {

				// Si era la ultima vuelta, mensage de buena carrera
				EncueMessage(5, 1);

				// Marca el final de la partida
				TRACK.game_on = false;
				TRACK.result = 0;

			}

		}

	}



	// Funcion CheckPoints();
	void CheckPoints(void) {
		
		// Si la carrera sigue activa (aun no se han dado todas las vueltas)
		if (TRACK.game_on) {

			// Si has pasado por el check point...
			if ((CAR[0].fy < (TRACK.checkpoint << 8)) && TRACK.checkpoint_flag) {

				// Flag de checkpoint abajo
				TRACK.checkpoint_flag = false;

				// Marcador de vuelta arriba
				TRACK.lap_flag = true;

				// Mensage de Check-Point
				EncueMessage(3, 0);

				// Añade tiempo al contador
				TimeExtended();

			}

		}

	}


	// Funcion TimeExtended();
	void TimeExtended(void) {

		// Añade tiempo al contador
		LAPTIME[TRACK_COUNTDOWN].seconds += TRACK.time_extended[OPTIONS[0].difficulty];
		if (LAPTIME[TRACK_COUNTDOWN].seconds > 59) {
			LAPTIME[TRACK_COUNTDOWN].seconds -= 60;
			LAPTIME[TRACK_COUNTDOWN].minutes ++;
		}

		// Flag Hurry!!!
		TRACK.hurry = true;

	}



	// Funcion BestLap();
	void BestLap(void) {

		// Variables
		s8 lap = (TRACK_LAPS - TRACK.lap);
		// Tiempo Best Lap
		s32 time_a = ((LAPTIME[TRACK_BESTLAP].minutes * 6000) + (LAPTIME[TRACK_BESTLAP].seconds * 100) + LAPTIME[TRACK_BESTLAP].hundredths);
		// Tiempo Vuelta actual
		s32 time_b = ((LAPTIME[lap].minutes * 6000) + (LAPTIME[lap].seconds * 100) + LAPTIME[lap].hundredths);

		if (time_b < time_a) CopyTimer(lap, TRACK_BESTLAP);

	}


	
	// Funcion TimeOut();
	void TimeOut(void) {

		// Si la carrera esta activa
		if (TRACK.game_on) {

			// Texto Hurry!!!
			if ((LAPTIME[TRACK_COUNTDOWN].minutes == 0) && (LAPTIME[TRACK_COUNTDOWN].seconds < 10) && TRACK.hurry) {

				// Flag abajo
				TRACK.hurry = false;
				// Mensage de Check-Point
				EncueMessage(4, 0);

			}

			// Fin de juego
			if ((LAPTIME[TRACK_COUNTDOWN].minutes == 0) && (LAPTIME[TRACK_COUNTDOWN].seconds == 0) && (LAPTIME[TRACK_COUNTDOWN].hundredths == 0)) {

				// Marca el final del juego
				TRACK.game_on = false;
				// Resultado de la carrera
				TRACK.result = 1;
				// Mensage de Game Over
				EncueMessage(6, 1);

			}

		}


	}



	// Funcion UpdateDisplay();
	void UpdateDisplay(void) {

		UpdateCountDownChrono();	// Actualiza la cuenta atras
		UpdateSpeedometer();		// Actualiza el velocimetro

	}



	// Funcion EncueMessage();
	void EncueMessage(u8 id, u8 mode) {

		// Variables
		u8 cue = MSGPOPUP.cue;

		// Registra la accion a realizar
		// Mode: 0 - Slide // 1 - In // 2 - Out
		// Action: 0 - In // 1 - Wait for ever // 2 - Wait time // 3 - Out
		switch (mode) {
			case 0:		// Slide
				MSGPOPUP.action[cue] = 0;
				MSGPOPUP.id[cue] = id;
				cue ++;
				MSGPOPUP.action[cue] = 2;
				MSGPOPUP.id[cue] = id;
				cue ++;
				MSGPOPUP.action[cue] = 3;
				MSGPOPUP.id[cue] = id;
				cue ++;
				MSGPOPUP.cue = cue;
				break;
			case 1:		// In
				MSGPOPUP.action[cue] = 0;
				MSGPOPUP.id[cue] = id;
				cue ++;
				MSGPOPUP.action[cue] = 1;
				MSGPOPUP.id[cue] = id;
				cue ++;
				MSGPOPUP.cue = cue;
				break;
			case 2:		// Out
				MSGPOPUP.action[cue] = 3;
				MSGPOPUP.id[cue] = id;
				cue ++;
				MSGPOPUP.cue = cue;
				break;
		}

	}



	// Funcion PopupMessage();
	u8 PopupMessage(void) {

		// Variables locales
		u8 n = 0;
		s32 x = 0;		// Coordenadas
		s32 dx = 0;		// Distancias

		bool donext = false;	// Salta a la siguiente accion


		// Si hay algun mensage en cola
		if (MSGPOPUP.cue > 0) {

			// Si no esta en ejecucion, mira que tienes que hacer
			if (!MSGPOPUP.running) {

				// Prepara el proceso
				switch (MSGPOPUP.action[0]) {

					case 0:	// In
						MSGPOPUP.x = -96;
						MSGPOPUP.y = 16;
						for (n = 0; n < 3; n ++) {
							NF_SpriteFrame(1, (MSG_SPRITE + n), MSGPOPUP.id[0]);
						}
						MSGPOPUP.running = true;
						break;

					case 1:	// Wait for ever
						MSGPOPUP.running = true;
						break;

					case 2:	// Wait time
						MSGPOPUP.running = true;
						MSGPOPUP.timer = 15;
						break;

					case 3:	// Out
						MSGPOPUP.running = true;
						break;

				}

			} else {

				// Ejecuta la accion
				switch (MSGPOPUP.action[0]) {

					case 0:	// In
						// Calcula la distancia
						dx = ((128 - MSGPOPUP.x) >> 3);
						if (dx < 1) dx = 1;
						MSGPOPUP.x += dx;
						// Si esta colocado, salta a la siguiente accion
						if (MSGPOPUP.x >= 128) {
							MSGPOPUP.x = 128;
							donext = true;
						}
						break;

					case 1:	// Wait for ever
						donext = true;
						break;

					case 2:	// Wait time
						MSGPOPUP.timer --;
						if (MSGPOPUP.timer <= 0) {
							MSGPOPUP.timer = 0;
							donext = true;
						}
						break;

					case 3:	// Out
						// Calcula la distancia
						dx = ((MSGPOPUP.x - 128) >> 3);
						if (dx < 1) dx = 1;
						MSGPOPUP.x += dx;
						// Si esta colocado, salta a la siguiente accion
						if (MSGPOPUP.x >= 351) {
							MSGPOPUP.x = 351;
							donext = true;
						}
						break;

				}

				// Actualiza los sprites del mensage
				x = (MSGPOPUP.x - 96);	// Izquierda
				if (x < -64) x = -64;
				if (x > 319) x = 319;
				NF_MoveSprite(1, MSG_SPRITE, x, MSGPOPUP.y);
				x = (MSGPOPUP.x - 32);	// Centro
				if (x < -64) x = -64;
				if (x > 319) x = 319;
				NF_MoveSprite(1, (MSG_SPRITE + 1), x, MSGPOPUP.y);
				x = (MSGPOPUP.x + 32);	// Derecho
				if (x < -64) x = -64;
				if (x > 319) x = 319;
				NF_MoveSprite(1, (MSG_SPRITE + 2), x, MSGPOPUP.y);

				// Si debes ejecutar la siguiente accion
				if (donext) {
					// Organiza la cola de opciones
					for (n = 0; n < (TOTAL_POPUPS - 1); n ++) {
						MSGPOPUP.action[n] = MSGPOPUP.action[(n + 1)];
						MSGPOPUP.id[n] = MSGPOPUP.id[(n + 1)];
					}
					// Actualiza el contador
					MSGPOPUP.cue --;
					// Marca el trabajo como terminado
					MSGPOPUP.running = false;
				}



			}

		}

		// Devuelve los elementos en cola
		return MSGPOPUP.cue;

	}



	// Funcion UpdateCountDownChrono();
	void UpdateCountDownChrono(void) {

		// Variables
		u8 unidades = 0;
		u8 decenas = 0;

		// Marcador minutos
		NF_SpriteFrame(0, CHRONO_SPRITE, LAPTIME[TRACK_COUNTDOWN].minutes);

		// Marcador segundos
		decenas = (int)(LAPTIME[TRACK_COUNTDOWN].seconds / 10);
		unidades = (LAPTIME[TRACK_COUNTDOWN].seconds - (decenas * 10));
		NF_SpriteFrame(0, (CHRONO_SPRITE + 2), decenas);
		NF_SpriteFrame(0, (CHRONO_SPRITE + 3), unidades);

		// Marcador centesimas
		decenas = (int)(LAPTIME[TRACK_COUNTDOWN].hundredths / 10);
		unidades = (LAPTIME[TRACK_COUNTDOWN].hundredths - (decenas * 10));
		NF_SpriteFrame(0, (CHRONO_SPRITE + 5), decenas);
		NF_SpriteFrame(0, (CHRONO_SPRITE + 6), unidades);

	}



	// Funcion UpdateSpeedometer();
	void UpdateSpeedometer(void) {

		// Variables
		u8 unidades = 0;
		u8 decenas = 0;
		u8 centenas = 0;
		s16 speed = (CAR[0].speed >> 3);

		// Calcula los sprites
		centenas = (int)(speed / 100);
		decenas = (int)((speed - (centenas * 100)) / 10);
		unidades = (speed - ((centenas * 100) + (decenas * 10)));

		// Actualiza los Sprites
		NF_SpriteFrame(1, SPEED_SPRITE, centenas);
		NF_SpriteFrame(1, (SPEED_SPRITE + 1), decenas);
		NF_SpriteFrame(1, (SPEED_SPRITE + 2), unidades);

	}

	



#ifdef __cplusplus
}
#endif
