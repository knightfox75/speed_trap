#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de Creditos
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
	#include "credits.h"
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





	// Funcion Credits();
	void Credits(void) {

		// Variables
		u32 height = 0;		// Tamaño de la capa de creditos

		// Carga archivos
		LoadCreditsFiles(CREDITS_STAGE);

		// Crea el escenario
		CreateCreditsStage(CREDITS_STAGE);

		// Carga las capas de texto
		height = LoadCreditsFile("text/credits.txt");
		// Actualiza la capa de texto
		NF_UpdateTextLayers();

		// Kernel de los creditos
		CreditsKernel(height);

		// Al salir de los creditos, calcula el siguiente circuito
		CREDITS_STAGE ++;
		if (CREDITS_STAGE > TOTAL_TRACKS) CREDITS_STAGE = 1;

	}



	// Funcion LoadCreditsFiles();
	void LoadCreditsFiles(u8 stage) {

		ResetAll();					// Borralo todo

		// Carga archivos del circuito
		LoadTrackFiles(stage);

		// Carga los coches
		LoadCarSprites();

	}



	// Funcion CreateCreditsStage();
	void CreateCreditsStage(u8 stage) {

		// Variables
		u8 n = 0;
		u8 car = (int)(rand() % CAR_TOTALNUM);

		CreateTrack(stage);			// Crea la pista
		CreateCars(stage, car);		// Crea los coches

		// Baja el brillo
		setBrightness(3, -16);

		// Coloca los graficos
		CreditsEngine();
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		for (n = 1; n <= 3; n ++ ) {
			NF_ScrollBg(0, n, TRACK.x[0], TRACK.y[0]);
			NF_ScrollBg(1, n, TRACK.x[1], TRACK.y[1]);
		}

		// Modifica las paletas
		EditCreditsPalette();

		// Espera al sincronismo para que se muestren
		swiWaitForVBlank();

	}



	// Funcion EditCreditsPalette();
	void EditCreditsPalette(void) {

		// Variables
		u8 layer = 0;	// Capa
		u16 color = 0;	// Color a modificar
		u8 r, g, b;		// Desglose del RGB
		u8 nr, ng, nb;	// Colores convertidos
		u8 pal = 0;		// Paleta de los sprites

		/////////////////////////////////////
		// Edita las paletas de los fondos //
		/////////////////////////////////////

		// Bucle capas
		for (layer = 1; layer < 4; layer ++) {
			// Bucle colores
			for (color = 1; color < 256; color ++) {
				// Obten el color
				NF_BgGetPalColor(0, layer, color, &r, &g, &b);
				// Oscurecelos
				nr = (r >> 1);
				ng = (g >> 1);
				nb = (b >> 1);
				// Y grabalos en la paleta
				NF_BgEditPalColor(0, layer, color, nr, ng, nb);
			}
			// Actualiza la paleta en la VRAM
			NF_BgUpdatePalette(0, layer);
			NF_BgUpdatePalette(1, layer);
		}


		//////////////////////////////////////
		// Edita las paletas de los sprites //
		//////////////////////////////////////

		// Bucle capas
		for (pal = 0; pal < 4; pal ++) {
			// Bucle colores
			for (color = 1; color < 256; color ++) {
				// Obten el color
				NF_SpriteGetPalColor(0, pal, color, &r, &g, &b);
				// Oscurecelos
				nr = (r >> 1);
				ng = (g >> 1);
				nb = (b >> 1);
				// Y grabalos en la paleta
				NF_SpriteEditPalColor(0, pal, color, nr, ng, nb);
			}
			// Actualiza la paleta en la VRAM
			NF_SpriteUpdatePalette(0, pal);
			NF_SpriteUpdatePalette(1, pal);
		}

	}



	// Funcion CreditsKernel();
	void CreditsKernel(u32 height) {

		//////////////////////////
		// Ejecuta los creditos //
		//////////////////////////

		// Variables
		bool loop = true;		// Main loop
		u8 n = 0;				// Uso general

		s16 fade = (16 << 8);	// Brillo
		bool fade_in = true;	// Flag Fade in
		bool fade_out = false;	// Flag Fade Out

		u32 y = 0;
		u32 y_end = (height - (30 << 4));


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
			}

			// Control del final de la demo, si no se esta realizando ningun efecto
			if (!fade_in && !fade_out) {

				// Sal del modo autodemo si se presiona la A o B
				if (KEYPRESS.a || KEYPRESS.b) {
					fade_out = true;
				}

			}

			// Ejecuta el motor principal de la carrera
			CreditsEngine();

			// Calcula la posicion de la capa de texto
			if (y < (y_end << 8)) y += 64;
			if (!fade_in && !fade_out) {
				if (y >= (y_end << 8)) {
					y = (y_end << 8);
					fade_out = true;
				}
			}

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

			// Actualiza el scroll de texto
			NF_ScrollBg(0, 0, 0, (y >> 8));
			NF_ScrollBg(1, 0, 0, ((y >> 8) + 288));

		}

	}



	// Funcion CreditsEngine();
	void CreditsEngine(void) {

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

		// Si es necesario, recoloca los coches marioneta
		ReplacePuppetCar();

		// Dibuja el escenario
		DrawTrack(0);
		DrawCars(0);

		// Guarda las posiciones de los coches al salir del frame
		BackupCarPosition();

	}



	// Funcion LoadCreditsFile();
	u32 LoadCreditsFile(const char* file) {

		// Variables
		char* buffer;			// Buffer para almacenar el texto
		u32 size = 0;			// Tamaño del archivo
		buffer = NULL;
		FILE* file_id;			// Declara los punteros a los ficheros
		char filename[256];		// Variable para almacenar el path al archivo
		u32 lines = 0;			// Numero de lineas en el texto
		u32 y_size = 0;			// Altura de la capa de texto
		char text[64];			// Buffer de la linea de texto
		u16 len = 0;			// Longitud de la linea de texto
		s16 x = 0;				// Coordenadas para escribir el texto
		s16 y = 0;
		bool newline = false;	// Nueva linea
		u32 chr = 0;			// Caracter dentro de la linea
		u32 n = 0;				// Uso general

		sprintf(filename, "%s/%s", NF_ROOTFOLDER, file);
		file_id = fopen(filename, "rb");
		if (file_id) {	// Si el archivo existe...
			// Obten el tamaño del archivo
			fseek(file_id, 0, SEEK_END);
			size = ftell(file_id);
			rewind(file_id);
			// Reserva el espacio en RAM
			buffer = (char*) calloc (size, sizeof(char));
			if (buffer == NULL) {		// Si no hay suficiente RAM libre
				NF_Error(102, NULL, size);
			}
			// Lee el archivo y ponlo en la RAM
			fread(buffer, 1, size, file_id);
		} else {	// Si el archivo no existe...
			NF_Error(101, filename, 0);
		}
		fclose(file_id);		// Cierra el archivo

		// Cuenta lineas
		for (n = 0; n < size; n ++) {
			if (buffer[n] == 0x0a) lines ++;
		}

		// Calcula el tamaño de la capa de texto
		n = (480 + (lines << 4) + 480);
		y_size = (((n >> 8) + 1) << 8);

		// Carga las fuentes
		NF_LoadTextFont16("font/credits", "top", 256, y_size, 0);
		NF_LoadTextFont16("font/credits", "bottom", 256, y_size, 0);

		// Crea las capas de texto
		NF_CreateTextLayer16(0, 0, 0, "top");
		NF_CreateTextLayer16(1, 0, 0, "bottom");

		// Prepara la escritura de texto
		memset(text, 0, sizeof(text));
		y = 30;

		// Escribe el texto en las capas
		for (n = 0; n < size; n++) {

			// Si es una nueva linea
			if (newline) {
				// Escribe la linea actual
				text[chr] = '\0';
				len = strlen(text);
				x = (16 - (len >> 1));
				NF_WriteText16(0, 0, x, y, text);
				NF_WriteText16(1, 0, x, y, text);
				// Preparate para una nueva linea
				memset(text, 0, sizeof(text));
				chr = 0;
				y ++;
				// Linea completada
				newline = false;
			}

			// Copia los caracteres del buffer a la linea
			if ((buffer[n] != 0x0d) && (buffer[n] != 0x0a)) {
				text[chr] = buffer[n];
				chr ++;
			} else {
				// Si es final de linea, marcalo
				if (buffer[n] == 0x0a) newline = true;
			}

		}

		// Libera el buffer de texto
		free(buffer);

		// Devuelve el tamaño real de la capa de texto
		return  ((30 + lines + 30) << 4);

	}





#ifdef __cplusplus
}
#endif