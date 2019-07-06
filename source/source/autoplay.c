#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones Autoplay (modo demo)
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
	#include "autoplay.h"
	#include "game.h"
	#include "engine.h"
	#include "sound.h"
	#include "load.h"
	#include "create.h"
	#include "display.h"
	#include "system.h"
	#include "defines.h"
	#include "physics.h"
	#include "ai.h"



	// Funcion AutoPlay();
	void AutoPlay(void) {

		// Variables
		u8 car = (int)(rand() % CAR_TOTALNUM);

		// Prepara el circuito
		PrepareRace(AUTOPLAY_STAGE, car);

		// Ejecuta la demo
		AutoPlayKernel();

		// Al salir de la demo, calcula el siguiente circuito
		AUTOPLAY_STAGE ++;
		if (AUTOPLAY_STAGE > TOTAL_TRACKS) AUTOPLAY_STAGE = 1;

	}



	// Funcion AutoPlayKernel();
	void AutoPlayKernel(void) {

		/////////////////////////
		// Ejecuta el autodemo //
		/////////////////////////

		// Variables
		bool loop = true;		// Main loop
		u8 n = 0;				// Uso general
		s32 timer = 0;			// Contador de tiempo de la demo

		s16 fade = (16 << 8);	// Brillo
		bool fade_in = true;	// Flag Fade in
		bool fade_out = false;	// Flag Fade Out
		
		// Sonido del motor del coche
		if (OPTIONS[0].engine_volume > 0) PlaySfx(0);

		// Ejecuta hasta terminar la carrera
		while (loop) {

			// Lee el teclado
			ReadKeypad();

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


			// Control del final de la demo, si no se esta realizando ningun efecto
			if (!fade_in && !fade_out) {

				// Sal del modo autodemo si se presiona la A o B
				if (KEYPRESS.a || KEYPRESS.b) {
					fade_out = true;
				}

				// Control de tiempo de la demo
				timer ++;
				if (timer > 3600) fade_out = true;

			}

			// Ejecuta el motor principal de la carrera
			AutoPlayEngine();

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

	}



	// Funcion AutoPlayEngine();
	void AutoPlayEngine(void) {

		// Actualiza los cronometros
		CountUpTimer(TRACK_TIME);
		CountUpTimer((TRACK_LAPS - TRACK.lap));
		CountDownTimer(TRACK_COUNTDOWN);

		// IA de los coches
		CpuCarsAI(0);

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







#ifdef __cplusplus
}
#endif