#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de control de "Records"
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
	#include "records.h"
	#include "defines.h"
	#include "sound.h"
	#include "system.h"
	#include "save.h"





	// Funcion CheckTrackRecord();
	u8 CheckTrackRecord(u8 stage) {

		// Variables
		u8 n = 0;
		u8 m = 0;
		s32 time_a = 0;
		s32 time_b = 0;
		u8 best_lap = 255;		// Flag record de vuelta rapida
		u8 track_record = 255;	// Flag record de circuito
		u8 t = (stage - 1);		// Indice donde alamcenar el record
		u8 result = 0;			// Devuelve el resultado si hay records

		u8 d = OPTIONS[0].difficulty;	// Nivel de dificultad actual

		// Crea el array de backup
		records_info temp;

		/////////////////////////////////////////////
		// Verifica si hay record de vuelta rapida //
		/////////////////////////////////////////////
		// Mejor vuelta de la carrera
		time_a = ((LAPTIME[TRACK_BESTLAP].minutes * 6000) + (LAPTIME[TRACK_BESTLAP].seconds * 100) + LAPTIME[TRACK_BESTLAP].hundredths);
		// Mejor vuelta del circuito
		time_b = ((RECORDS[d][t].minutes[0] * 6000) + (RECORDS[d][t].seconds[0] * 100) + RECORDS[d][t].hundredths[0]);
		// Si el tiempo es inferior, marcalo
		if (time_a < time_b) best_lap = 0;

		///////////////////////////////////////
		// Verifica si hay record de carrera //
		///////////////////////////////////////
		// Tiempo total de la carrera
		time_a = ((LAPTIME[TRACK_TIME].minutes * 6000) + (LAPTIME[TRACK_TIME].seconds * 100) + LAPTIME[TRACK_TIME].hundredths);
		for (n = 1; n < TOTAL_RECORDS; n ++) {
			// Records del circuito
			time_b = ((RECORDS[d][t].minutes[n] * 6000) + (RECORDS[d][t].seconds[n] * 100) + RECORDS[d][n].hundredths[0]);
			// Si el tiempo es inferior, marcalo
			if (time_a < time_b) {
				track_record = n;
				n = TOTAL_RECORDS;
			}
		}

		// Si hay record de vuelta rapida, guardalo
		if (best_lap != 255) {
			RECORDS[d][t].minutes[0] = LAPTIME[TRACK_BESTLAP].minutes;
			RECORDS[d][t].seconds[0] = LAPTIME[TRACK_BESTLAP].seconds;
			RECORDS[d][t].hundredths[0] = LAPTIME[TRACK_BESTLAP].hundredths;
			// Y marca que se ha realizado ese record
			result |= BIT(0);
		}

		// Si hay record de circuito, guardalo
		if (track_record != 255) {
			// Inserta el record donde toque
			m = 1;
			for (n = 1; n < TOTAL_RECORDS; n ++) {
				if (n == track_record) {
					sprintf(temp.name[n], "AAA");
					temp.name[n][3] = '\0';
					temp.minutes[n] = LAPTIME[TRACK_TIME].minutes;
					temp.seconds[n] = LAPTIME[TRACK_TIME].seconds;
					temp.hundredths[n] = LAPTIME[TRACK_TIME].hundredths;
				} else {
					sprintf(temp.name[n], "%s", RECORDS[d][t].name[m]);
					temp.name[n][3] = '\0';
					temp.minutes[n] = RECORDS[d][t].minutes[m];
					temp.seconds[n] = RECORDS[d][t].seconds[m];
					temp.hundredths[n] = RECORDS[d][t].hundredths[m];
					m ++;
				}
			}
			// Reorganiza los records modificados
			for (n = 1; n < TOTAL_RECORDS; n ++) {
				sprintf(RECORDS[d][t].name[n], "%s", temp.name[n]);
				RECORDS[d][t].name[n][3] = '\0';
				RECORDS[d][t].minutes[n] = temp.minutes[n];
				RECORDS[d][t].seconds[n] = temp.seconds[n];
				RECORDS[d][t].hundredths[n] = temp.hundredths[n];
			}
			// Y marca que se ha realizado ese record
			result |= BIT(track_record);
		}

		// Devuelve el resultado
		return result;

	}



	// Funcion DisplayResults();
	void DisplayResults(u8 stage) {

		// Deten la musica actual y pon la de pantalla de records
		StopBgm();
		PlayBgm(1);

		// Resetea todo
		ResetAll();

		// Variables locales
		s32 bg3_x = (256 << 8);		// Posicion del fondo 3
		s32 bg3_y = (256 << 8);

		s16 fade = (16 << 8);		// Brillo
		bool fade_in = true;		// Flag Fade in
		bool fade_out = false;		// Flag Fade Out

		char text[32];				// Buffer de texto
		bool newtext = true;		// Flag de escritura de texto
		u8 track_id = (stage - 1);	// Id del circuito para los records

		char name[4];				// Iniciales para el record
		s8 text_pos = 0;			// Posicion en el campo de texto
		u8 letter_delay = 0;		// Temporizador parpadeo letra
		bool letter_blink = true;	// Letra visible o no?

		bool keyrepeat = false;		// Evita la repeticion de pulsaciones

		u8 d = OPTIONS[0].difficulty;	// Nivel de dificultad actual

		bool loop = true;				// Loop del menu

		u8 record = 0;				// Alamacena si hay un record
		u8 frame = 0;				// Frame de la animacion
		u8 more = 0;				// Si hay mas de una linea de animacion

		u8 n = 0;		// Variables para bucles
		u8 r = 0;

		// Pantalla en negro
		setBrightness(3, -16);

		// Verifica si hay record
		record = CheckTrackRecord(stage);

		// Si hay un record...
		if (record != 0) {
			// Carga la cadena de texto con el nombre si es necesario
			for (n = 0; n < TOTAL_RECORDS; n ++) {
				if (record & BIT(n)) {
					sprintf(name, "%s", RECORDS[d][track_id].name[n]); 
					name[3] = '\0';
				}
			}
		}


		//////////////////////////
		//						//
		//	Carga de archivos	//
		//						//
		//////////////////////////

		// Carga los fondos tileados necesarios
		NF_LoadTiledBg("bg/options/bg3", "bg3", 512, 512);
		NF_LoadTiledBg("bg/carcustom/bot_bg2", "bg2", 256, 256);
		NF_LoadTiledBg("bg/trackselect/ts_bg1", "bg1", 256, 256);

		// Carga la fuente		
		NF_LoadTextFont16("font/records", "top", 256, 256, 0);
		NF_LoadTextFont16("font/records", "bottom", 256, 256, 0);

		// Si hay record, carga los sprites y sus paletas
		if (record != 0) {
			NF_LoadSpritePal("sprite/records/recordanim", 0);
			NF_LoadSpriteGfx("sprite/records/recordanim_a", 0, 64, 32);
			NF_LoadSpriteGfx("sprite/records/recordanim_b", 1, 64, 32);
			NF_LoadSpriteGfx("sprite/records/recordanim_c", 2, 64, 32);
			NF_LoadSpriteGfx("sprite/records/recordanim_d", 3, 64, 32);
		}


		//////////////////////////////
		//							//
		//	Creacion de graficos	//
		//							//
		//////////////////////////////

		// Crea los fondos para la pantalla superior
		NF_CreateTiledBg(0, 3, "bg3");
		NF_ScrollBg(0, 3, 64, 64);
		NF_CreateTiledBg(0, 2, "bg2");
		REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
		REG_BLDALPHA = 0x06 | (0x06 << 8);
		NF_CreateTiledBg(0, 1, "bg1");

		// Crea los fondos para la pantalla inferior
		NF_CreateTiledBg(1, 3, "bg3");
		NF_ScrollBg(1, 3, 64, 64);
		NF_CreateTiledBg(1, 2, "bg2");
		REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
		REG_BLDALPHA_SUB = 0x06 | (0x06 << 8);
		NF_CreateTiledBg(1, 1, "bg1");

		// Crea la capa de texto en la pantalla inferior
		NF_CreateTextLayer16(0, 0, 0, "top");
		NF_CreateTextLayer16(1, 0, 0, "bottom");

		// Si hay record crea los sprites
		if (record != 0) {
			// Transfiere la paleta de los botones
			NF_VramSpritePal(1, 0, 0);
			// Transfiere los graficos de los botones
			for (n = 0; n < 4; n ++) {
				NF_VramSpriteGfx(1, n, n, true);
			}
		}

		// Hay record de vuelta rapida?
		if (record & BIT(0)) {
			for (n = 0; n < 4; n ++) {
				NF_CreateSprite(1, n, n, 0, (n << 6), 120);
				NF_SpriteLayer(1, n, 1);
				NF_SpriteFrame(1, n, 0);
			}
			more = 4;
		}

		// Hay algun record mas?
		for (r = 1; r < TOTAL_RECORDS; r ++) {
			if (record & BIT(r)) {
				for (n = 0; n < 4; n ++) {
					NF_CreateSprite(1, (n + more), n, 0, (n << 6), (40 + (r << 4)));
					NF_SpriteLayer(1, (n + more), 1);
					NF_SpriteFrame(1, (n + more), 0);
				}
			}
		}

		// Espera al sincronismo
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamSub);

		while (loop) {

			// Lee el teclado
			ReadKeypad();
			keyrepeat = false;

			// Effecto "fade in"
			if (fade_in) {
				fade -= 64;
				if (fade <= 0) {
					fade = 0;
					fade_in = false;
				}
				// Ajusta el brillo
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

			// Mueve el fondo
			bg3_x -= 128;
			if (bg3_x < (0 << 8)) bg3_x += (256 << 8);
			bg3_y -= 64;
			if (bg3_y < (0 << 8)) bg3_y += (256 << 8);

			// Control de parpadeo
			if ((record != 0) && !fade_out) {
				letter_delay ++;
				if (letter_delay > 10) {
					letter_delay = 0;
					letter_blink = !letter_blink;
					newtext = true;
				}
			}

			// Control de records, entrada de nombres
			if ((record != 0) && !fade_in && !fade_out) {
				// Cambio de posicion a derecha
				if (KEYPRESS.right && !KEYPRESS.a) {
					text_pos ++;
					if (text_pos > 2) text_pos = 2;
					letter_delay = 0;
					letter_blink = true;
					newtext = true;
				}
				// Cambio de posicion a derecha (Pulsando A) si no es la ultima letra
				if (!KEYPRESS.right && KEYPRESS.a && (text_pos < 2)) {
					text_pos ++;
					if (text_pos > 2) text_pos = 2;
					letter_delay = 0;
					letter_blink = true;
					newtext = true;
					keyrepeat = true;
				}
				// Cambio de posicion a izquierda
				if (KEYPRESS.left || KEYPRESS.b) {
					text_pos --;
					if (text_pos < 0) text_pos = 0;
					letter_delay = 0;
					letter_blink = true;
					newtext = true;
				}
				// Siguiente letra
				if (KEYPRESS.down) {
					name[text_pos] ++;
					if (name[text_pos] > 90) name[text_pos] = 65;
					letter_delay = 0;
					letter_blink = true;
					newtext = true;
				}
				// Letra anterior
				if (KEYPRESS.up) {
					name[text_pos] --;
					if (name[text_pos] < 65) name[text_pos] = 90;
					letter_delay = 0;
					letter_blink = true;
					newtext = true;
				}
			}

			// Salida
			if (KEYPRESS.a && !fade_in && !fade_out && (text_pos == 2) && (!keyrepeat)) {
				fade_out = true;
				letter_delay = 0;
				letter_blink = true;
				newtext = true;
			}

			// Textos
			if (newtext) {

				// Flag abajo
				newtext = false;

				// Pantalla superior

				// Cabecera
				NF_WriteText16(0, 0, 10, 2, "RACE RESULTS");

				// Lap records
				sprintf(text, "1ST LAP     %02d:%02d:%02d", LAPTIME[0].minutes, LAPTIME[0].seconds, LAPTIME[0].hundredths);
				NF_WriteText16(0, 0, 6, 4, text);
				sprintf(text, "2ND LAP     %02d:%02d:%02d", LAPTIME[1].minutes, LAPTIME[1].seconds, LAPTIME[1].hundredths);
				NF_WriteText16(0, 0, 6, 5, text);
				sprintf(text, "3RD LAP     %02d:%02d:%02d", LAPTIME[2].minutes, LAPTIME[2].seconds, LAPTIME[2].hundredths);
				NF_WriteText16(0, 0, 6, 6, text);
				sprintf(text, "4RT LAP     %02d:%02d:%02d", LAPTIME[3].minutes, LAPTIME[3].seconds, LAPTIME[2].hundredths);
				NF_WriteText16(0, 0, 6, 7, text);
				sprintf(text, "TOTAL TIME  %02d:%02d:%02d", LAPTIME[TRACK_TIME].minutes, LAPTIME[TRACK_TIME].seconds, LAPTIME[TRACK_TIME].hundredths);
				NF_WriteText16(0, 0, 6, 8, text);
				// Best lap
				sprintf(text, "BEST LAP    %02d:%02d:%02d", LAPTIME[TRACK_BESTLAP].minutes, LAPTIME[TRACK_BESTLAP].seconds, LAPTIME[TRACK_BESTLAP].hundredths);
				NF_WriteText16(0, 0, 6, 9, text);


				// Pantalla inferior //

				// Si no hay record...
				if (record == 0) {

					NF_WriteText16(1, 0, 9, 2, "TRACK  RECORDS");

				} else {	// Si hay record

					// Aplica el efecto parpadeo
					for (n = 0; n < TOTAL_RECORDS; n ++) {
						if (record & BIT(n)) {
							RECORDS[d][track_id].name[n][0] = name[0];
							RECORDS[d][track_id].name[n][1] = name[1];
							RECORDS[d][track_id].name[n][2] = name[2];
						}
					}
					if (!letter_blink)	{
						for (n = 0; n < TOTAL_RECORDS; n ++) {
							if (record & BIT(n)) {
								RECORDS[d][track_id].name[n][text_pos] = 32;
							}
						}
					}

					// Texto de cabecera
					NF_WriteText16(1, 0, 11, 2, "NEW RECORD");

				}

				// Lap records
				sprintf(text, "1ST  %s  %02d:%02d:%02d", RECORDS[d][track_id].name[1], RECORDS[d][track_id].minutes[1], RECORDS[d][track_id].seconds[1], RECORDS[d][track_id].hundredths[1]);
				NF_WriteText16(1, 0, 7, 4, text);
				sprintf(text, "2ND  %s  %02d:%02d:%02d", RECORDS[d][track_id].name[2], RECORDS[d][track_id].minutes[2], RECORDS[d][track_id].seconds[2], RECORDS[d][track_id].hundredths[2]);
				NF_WriteText16(1, 0, 7, 5, text);
				sprintf(text, "3RD  %s  %02d:%02d:%02d", RECORDS[d][track_id].name[3], RECORDS[d][track_id].minutes[3], RECORDS[d][track_id].seconds[3], RECORDS[d][track_id].hundredths[3]);
				NF_WriteText16(1, 0, 7, 6, text);

				// Best lap
				sprintf(text, "BEST LAP   %s  %02d:%02d:%02d ", RECORDS[d][track_id].name[0], RECORDS[d][track_id].minutes[0], RECORDS[d][track_id].seconds[0], RECORDS[d][track_id].hundredths[0]);
				NF_WriteText16(1, 0, 4, 8, text);

				// Actualiza la capa de textos
				NF_UpdateTextLayers();

			}

			// Animacion (si hay record)
			if (record != 0) {
				frame ++;
				if (frame > 7) frame = 0;
				for (n = 0; n < 4; n ++) {
					NF_SpriteFrame(1, n, frame);
				}
			}

			// Actualiza el OAM de la libreria
			NF_SpriteOamSet(1);
			// Espera al sincronismo
			swiWaitForVBlank();
			// Actualiza el OAM de Libnds
			oamUpdate(&oamSub);

			// Posiciona el fondo
			NF_ScrollBg(0, 3, (bg3_x >> 8), (bg3_y >> 8));
			NF_ScrollBg(1, 3, (bg3_x >> 8), (bg3_y >> 8));

		}

		// Si hay record...
		if (record != 0) {
			// Grabalo en el archivo
			WriteSavegame();
		}

	}





#ifdef __cplusplus
}
#endif