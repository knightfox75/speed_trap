#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de control del juego
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
	#include "game.h"
	#include "menu.h"
	#include "trackselect.h"
	#include "carselect.h"
	#include "autoplay.h"
	#include "credits.h"
	#include "records.h"
	#include "engine.h"
	#include "sound.h"
	#include "load.h"
	#include "create.h"
	#include "display.h"
	#include "records.h"
	#include "options.h"
	#include "customcar.h"
	#include "system.h"
	#include "defines.h"





	// Funcion Kernel();
	void Kernel(void) {

		// Variables
		bool loop = true;
		u8 thread = 0;
		bool execute = false;
		u8 stage = 0;
		u8 player = 0;
		u8 options = 10;

		// Musica para el menu
		PlayBgm(0);

		// Menu Principal
		while (loop) {

			// Ejecuta el menu y decide segun el resultado
			switch (MainMenu()) {

				// Start Race
				case 0:
					// Threas inicial
					thread = 1;
					while (thread > 0) {
						switch (thread) {
							// Seleccion de circuito
							case 1:
								// Selecciona el circuito
								stage = TrackSelect();
								// Si devuelve 255 (B pulsada), sal del menu sin ejecutar
								if (stage == 255) {
									thread --;
									execute = false;
								} else {
									// Siguiente menu
									thread ++;
								}
								break;
							// Selecciona el coche
							case 2:
								// Selecciona el coche
								player = CarSelect();
								// Si devuelve 255 (B pulsada), vuelve al menu anterior
								if (player == 255) {
									thread --;
									execute = false;
								} else {
									// Si todo esta aceptado, ejecutalo
									execute = true;
									// Y sal de los menus
									thread = 0;
								}
								break;
							// No hagas nada si hay otro thread
							default:
								break;
						}
					}
					// Si se debe ejecutar el resto de bloque... (ambos menus aceptados)
					if (execute) {
						// Cambia la musica del menu por la del circuito
						StopBgm();
						PlayBgm((10 + stage));
						// Prepara la carrera
						PrepareRace(stage, player);
						// Ejecuta la carrera
						RunRace(stage);
						// Cambia la musica del circuito por la del menu
						StopBgm();
						PlayBgm(0);
					}
					break;

				// Options
				case 1:
					// Ejecuta el menu de opciones miestras sea necesario
					while ((options == 1) || (options == 2) || (options == 10)) {
						// Ejecuta el menu principal de opciones
						options = OptionsMenu();
						// Si se ha seleccionado, ejecuta el submenu de parametrizacion de coches
						if (options == 1) CustomCarMenu();
						// Si se ha seleccionado, ejecuta el submenu de parametrizacion del sonido
						if (options == 2) SoundSetup();
					}
					// Marcalo para la siguiente entrada
					options = 10;
					break;

				// Credits
				case 2:
					// Cambia la musica del menu por la de los creditos
					StopBgm();
					PlayBgm(2);
					// Ejecuta los creditos
					Credits();
					// Cambia la musica de los creditos por la del menu
					StopBgm();
					PlayBgm(0);
					break;

				// Autodemo
				case 10:
					// Cambia la musica del menu por la del circuito
					StopBgm();
					PlayBgm((10 + AUTOPLAY_STAGE));
					// Ejecuta el autoplay
					AutoPlay();
					// Cambia la musica del autoplay por la del menu
					StopBgm();
					PlayBgm(0);
					break;

			}

		}

	}



	// Funcion PrepareRace();
	void PrepareRace(u8 stage, u8 player) {

		ResetAll();						// Borralo todo
		LoadStageFiles(stage);			// Carga el contenido del Stage
		CreateStage(stage, player);		// Crea el Stage

		// Variables
		u8 n = 0;

		// Reset de la IA (todos detenidos)
		for (n = 0; n < CAR_MAXNUM; n ++) {
			CAR[n].AI_acc = false;
			CAR[n].AI_brake = false;
			CAR[n].AI_right = false;
			CAR[n].AI_left = false;
		}

		// Resetea los temporizadores
		for (n = 0; n < TOTAL_TIMERS; n ++) {
			SetTimer(n, 0, 0, 0);
		}

		// Resetea el control de Mensages pop-up
		for (n = 0; n < TOTAL_POPUPS; n ++) {
			MSGPOPUP.id[n] = 0;
			MSGPOPUP.action[n] = 0;
		}
		MSGPOPUP.cue = 0;
		MSGPOPUP.timer = 0;
		MSGPOPUP.running = false;
		MSGPOPUP.next = false;

		// Temporizador Best Lap
		SetTimer(TRACK_BESTLAP, 9, 59, 99);
		// Temporizador Tiempo restante
		SetTimer(TRACK_COUNTDOWN, TRACK.st_minutes, TRACK.st_seconds, 0);

		// Paramtros comunes del circuito
		TRACK.checkpoint_flag = true;
		TRACK.lap_flag = false;
		TRACK.lap = TRACK_LAPS;
		TRACK.game_on = true;
		TRACK.hurry = true;
		TRACK.result = 255;		// Por defecto, el resultado de la carrera es partida abortada

		// Baja el brillo
		setBrightness(3, -16);

		// Coloca los graficos
		MainEngine(false);
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		for (n = 1; n <= 3; n ++ ) {
			NF_ScrollBg(0, n, TRACK.x[0], TRACK.y[0]);
			NF_ScrollBg(1, n, TRACK.x[1], TRACK.y[1]);
		}

		// Espera al sincronismo para que se muestren
		swiWaitForVBlank();

	}



	// Funcion RunRace();
	u8 RunRace(u8 stage) {

		// Variables
		bool loop = true;		// Main loop
		u8 n = 0;				// Uso general
		bool run = false;		// Flag de carrera empezada
		bool pause = false;		// Flag de pausa

		s16 fade = (16 << 8);	// Brillo
		bool fade_in = true;	// Flag Fade in
		bool fade_out = false;	// Flag Fade Out

		s16 semaphore_y = -64;				// Posicion Y del semaforo
		s8 semaphore_lights = 5;			// Contador de luces encendidas
		s16 semaphore_timer = 0;			// Temporizador de encendido
		bool semaphore_in = true;			// Entrada del semaforo
		bool semaphore_out = false;			// Salida del semaforo
		bool semaphore_counter = false;		// Cuenta atras

		bool loop_back = false;		// Proteccion pulsacion repetida

		s32 timer = 0;				// Temporizador de time out

		// Crea el semaforo
		CreateSemaphore();

		// Sonido del motor del coche
		if (OPTIONS[0].engine_volume > 0) PlaySfx(0);

		// Ejecuta hasta terminar la carrera
		while (loop) {

			// Lee el teclado
			ReadKeypad();
			loop_back = false;

			// Effecto "fade in"
			if (fade_in) {
				fade -= 64;
				if (fade <= 0) {
					fade = 0;
					fade_in = false;
				}
				setBrightness(3, (0 - (fade >> 8)));
			}

			// Effecto "fade out"
			if (fade_out) {
				fade += 64;
				if (fade >= (16 << 8)) {
					fade = (16 << 8);
					fade_out = false;
					loop = false;
				}
				// Ajusta el brillo
				setBrightness(3, (0 - (fade >> 8)));
				// Ajusta el volumen
				BgmVolume(((4096 - fade) >> 2));
				if (OPTIONS[0].engine_volume > 0) SfxVolume(0, ((4096 - fade) >> 2));
			}

			// Semaforo dentro
			if (semaphore_in) {
				semaphore_y ++;
				if (semaphore_y >= 16) {
					semaphore_y = 16;
					semaphore_in = false;
					semaphore_counter = true;
				}
				NF_MoveSprite(1, SEMAPHORE_SPRITE, 64, semaphore_y);
				NF_MoveSprite(1, (SEMAPHORE_SPRITE + 1), 128, semaphore_y);
			}

			// Cuenta atras
			if (semaphore_counter) {
				// Contador
				semaphore_timer ++;
				if (semaphore_timer > 59) {
					semaphore_timer = 0;
					// Una luz menos
					semaphore_lights --;
					// Si es la ultima luz, salida
					if (semaphore_lights == 0) {
						if (OPTIONS[0].sfx_volume > 0) PlaySfx(3);
						semaphore_counter = false;
						semaphore_out = true;
						run = true;
					} else {
						if (OPTIONS[0].sfx_volume > 0) PlaySfx(2);
					}
					// Actualiza el semaforo
					switch (semaphore_lights) {
						case 0:
							NF_SpriteFrame(1, SEMAPHORE_SPRITE, 3);
							NF_SpriteFrame(1, (SEMAPHORE_SPRITE + 1), 3);
							break;
						case 1:
							NF_SpriteFrame(1, SEMAPHORE_SPRITE, 2);
							NF_SpriteFrame(1, (SEMAPHORE_SPRITE + 1), 3);
							break;
						case 2:
							NF_SpriteFrame(1, SEMAPHORE_SPRITE, 1);
							NF_SpriteFrame(1, (SEMAPHORE_SPRITE + 1), 3);
							break;
						case 3:
							NF_SpriteFrame(1, SEMAPHORE_SPRITE, 0);
							NF_SpriteFrame(1, (SEMAPHORE_SPRITE + 1), 2);
							break;
						case 4:
							NF_SpriteFrame(1, SEMAPHORE_SPRITE, 0);
							NF_SpriteFrame(1, (SEMAPHORE_SPRITE + 1), 1);
							break;
					}
				}
			}

			// Salida del semaphoro
			if (semaphore_out) {
				if (semaphore_timer < 30) {
					semaphore_timer ++;
				} else {
					semaphore_y -= 2;
					if (semaphore_y <= -64) {
						semaphore_y = -64;
						semaphore_out = false;
						DeleteSemaphore();
					}
					NF_MoveSprite(1, SEMAPHORE_SPRITE, 64, semaphore_y);
					NF_MoveSprite(1, (SEMAPHORE_SPRITE + 1), 128, semaphore_y);
				}
			}

			// Control de salida de la partida, abortar y pausa
			if (!fade_in && !fade_out) {

				// Aborta carrera
				if (KEYPRESS.select && (TRACK.result == 255)) {
					TRACK.result = 2;
					fade_out = true;
				}

				// Contador de timeout para salir al final de la carrera
				if ((TRACK.result == 0) || (TRACK.result == 1)) {
					timer ++;
					if ((timer > 1200) || KEYPRESS.a) {
						fade_out = true;
						if (TRACK.result == 0) EncueMessage(5, 2);
						if (TRACK.result == 1) EncueMessage(6, 2);
					}
				}

				// Si pulsas START, pausa
				if (KEYPRESS.start && !loop_back) { 
					pause = true;
					loop_back = true;
					setBrightness(3, -8);
				}

			}

			// Si la pausa esta activa
			while (pause) {

				// Lee el teclado
				ReadKeypad();

				// Si has pulsas START, sal de la pausa
				if (KEYPRESS.start && !loop_back) {
					pause = false;
					loop_back = true;
					setBrightness(3, 0);
				}

				/////////////////////////////
				// Tareas durante el Vsync //
				/////////////////////////////
				// Actualiza el OAM de Sprites
				NF_SpriteOamSet(0);
				NF_SpriteOamSet(1);
				// Espera al sincronismo vertical
				swiWaitForVBlank();
				// Actualiza el OAM de Libnds
				oamUpdate(&oamMain);
				oamUpdate(&oamSub);

				// Flag abajo
				loop_back = false;

			}

			// Ejecuta el motor principal de la carrera
			MainEngine(run);

			// Actualiza el OAM de Sprites
			NF_SpriteOamSet(0);
			NF_SpriteOamSet(1);

			// Espera al sincronismo vertical
			swiWaitForVBlank();

			// Actualiza el OAM de Libnds
			oamUpdate(&oamMain);
			oamUpdate(&oamSub);

			// Actualiza el scroll de fondos
			for (n = 1; n <= 3; n ++ ) {
				NF_ScrollBg(0, n, TRACK.x[0], TRACK.y[0]);
				NF_ScrollBg(1, n, TRACK.x[1], TRACK.y[1]);
			}

			// Actualiza el sonido del motor
			if (OPTIONS[0].engine_volume > 0) SfxFreq(0, (1024 + (CAR[0].speed >> 1)));

		}


		// Si hay algun SFX activo...
		if ((OPTIONS[0].engine_volume > 0) || (OPTIONS[0].sfx_volume > 0)) {
			// Deten los SFX
			StopAllSfx();
			// Descarga los SFX de la RAM
			UnloadSfxFiles();
		}

		// Muestra la pantalla con el resumen de la misma, si has terminado la carrera
		if (TRACK.result == 0) DisplayResults(stage);

		// Devuelve el resultado
		return 0;

	}





#ifdef __cplusplus
}
#endif
