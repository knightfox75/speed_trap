#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones del menu de seleccion del circuito
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
	#include "trackselect.h"
	#include "system.h"
	#include "defines.h"



	// Funcion TrackSelect();
	u8 TrackSelect(void) {

		// Resetea todo
		ResetAll();

		// Variables locales
		s32 bg3_x = (256 << 8);		// Posicion del fondo 3, pantalla inferior
		s32 bg3_y = (256 << 8);

		s32 ts_x = 0;				// Posicion texto "TRACK SELECT"

		bool next = false;			// Cambio a siguiente circuito
		bool back = false;			// Cambio a circuito anterior

		s8 option = 0;					// Opcion del menu seleccionada
		u8 last = (TOTAL_TRACKS - 1);	// Ultima opcion del menu

		char track_name[64];		// Guarda el texto con el nombre del circuito
		s32 tn_x = 0;				// Coordenada X
		s32 tn_limit = 0;			// Limite del movimiento

		s16 fade = (16 << 8);		// Brillo
		bool fade_in = true;		// Flag Fade in
		bool fade_out = false;		// Flag Fade Out

		char text[32];				// Buffer de texto
		bool newtext = true;		// Actualiza los textos

		bool menu = false;			// Estado del movimiento del menu

		bool cancel = false;		// Se ha cancelado este menu?

		u8 d = OPTIONS[0].difficulty;	// Nivel de dificultad actual

		bool loop = true;				// Loop del menu

		// Pantalla en negro
		setBrightness(3, -16);

		// Carga los archivos necesarios
		LoadTrackSelectFiles();

		// Crea el menu
		CreateTrackSelectMenu();


		// Nucleo del menu

		while (loop) {
			
			// Control del menu
			ReadKeypad();

			// Si no esta cambiando de opcion el menu
			if (!next && !back && !fade_in && !fade_out) {
				// Circuito Siguiente
				if (KEYPRESS.right || KEYHELD.right) {
					next = true;
				}
				// Circuito Anterior
				if (KEYPRESS.left || KEYHELD.left) {
					back = true;
				}
				// Acepta la seleccion
				if (KEYPRESS.a) {
					fade_out = true;
				}
				// Cancela la seleccion
				if (KEYPRESS.b) {
					fade_out = true;
					cancel = true;
				}
			}

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
				setBrightness(3, (0 - (fade >> 8)));
			}

			// Mueve el fondo (bandera)
			bg3_x -= 128;
			if (bg3_x < 0) bg3_x += (256 << 8);
			bg3_y -= 64;
			if (bg3_y < 0) bg3_y += (256 << 8);

			// Mueve el texto "TRACK SELECT"
			ts_x += 128;
			if (ts_x > (136 << 8)) ts_x -= (136 << 8);

			// Mueve el texto "TRACK NAME"
			tn_x += 128;
			if (tn_x > (tn_limit << 8)) tn_x -= (tn_limit << 8);


			// Si el menu se mueve a la derecha (siguiente)
			if (next) {
				menu = TrackSelectMoveMenu(-8);
				if (menu) {
					option ++;
					if (option > last) option = 0;
					newtext = true;
					next = false;
				}
			}

			// Si el menu se mueve a la izquierda (anterior)
			if (back) {
				menu = TrackSelectMoveMenu(8);
				if (menu) {
					option --;
					if (option < 0) option = last;
					newtext = true;
					back = false;
				}
			}

			// Texto en la pantalla inferior
			if (newtext) {

				// Flag abajo
				newtext = false;

				// Cabecera
				NF_WriteText16(1, 0, 9, 2, "TRACK  RECORDS");

				// Nombre del circuito
				switch (option) {
					case 0:
						sprintf(text, "SPRING SPEED     ");
						break;
					case 1:
						sprintf(text, "DESERT TRAP     ");
						break;
					case 2:
						sprintf(text, "THE FACTORY     ");
						break;
					default:
						sprintf(text, "UNNAMED TRACK     ");
						break;
				}
				tn_limit = (strlen(text) << 3);
				sprintf(track_name, "%s%s%s", text, text, text);
				NF_WriteText16(0, 0, 0, 10, track_name);

				// Lap records
				sprintf(text, "1ST  %s  %02d:%02d:%02d", RECORDS[d][option].name[1], RECORDS[d][option].minutes[1], RECORDS[d][option].seconds[1], RECORDS[d][option].hundredths[1]);
				NF_WriteText16(1, 0, 7, 4, text);
				sprintf(text, "2ND  %s  %02d:%02d:%02d", RECORDS[d][option].name[2], RECORDS[d][option].minutes[2], RECORDS[d][option].seconds[2], RECORDS[d][option].hundredths[2]);
				NF_WriteText16(1, 0, 7, 5, text);
				sprintf(text, "3RD  %s  %02d:%02d:%02d", RECORDS[d][option].name[3], RECORDS[d][option].minutes[3], RECORDS[d][option].seconds[3], RECORDS[d][option].hundredths[3]);
				NF_WriteText16(1, 0, 7, 6, text);

				// Best lap
				sprintf(text, "BEST LAP   %s  %02d:%02d:%02d", RECORDS[d][option].name[0], RECORDS[d][option].minutes[0], RECORDS[d][option].seconds[0], RECORDS[d][option].hundredths[0]);
				NF_WriteText16(1, 0, 4, 8, text);

				// Actualiza la capa de textos
				NF_UpdateTextLayers();

			}

			// Actualiza el OAM
			NF_SpriteOamSet(0);

			// Espera al sincronismo
			swiWaitForVBlank();

			// Actualiza el VRAM del OAM
			oamUpdate(&oamMain);

			// Posiciona el fondo (bandera)
			NF_ScrollBg(1, 3, (bg3_x >> 8), (bg3_y >> 8));

			// Posiciona el texto "TRACK SELECT"
			NF_ScrollBg(0, 1, (ts_x >> 8), 0);

			// Posiciona el texto "TRACK NAME"
			NF_ScrollBg(0, 0, (tn_x >> 8), 0);

		}

		// Si se ha cancelado...
		if (cancel) option = 254;

		// Devuelve la pista seleccionada
		return (option + 1);

	}



	// Funcion LoadTrackSelectFiles();
	void LoadTrackSelectFiles(void) {

		// Variables
		u8 n = 0;
		char file[64];

		// Carga los fondos tileados necesarios (pantalla superior)
		NF_LoadTiledBg("bg/trackselect/top_bg3", "top_bg3", 256, 256);
		NF_LoadTiledBg("bg/trackselect/top_bg2", "top_bg2", 256, 256);

		// Carga los fondos tileados necesarios (pantalla inferior)
		NF_LoadTiledBg("bg/menu/bigflag", "bg3_bottom", 512, 512);
		NF_LoadTiledBg("bg/trackselect/ts_bg2", "bg2_bottom", 256, 256);
		NF_LoadTiledBg("bg/trackselect/ts_bg1", "bg1_bottom", 256, 256);

		// Carga las fuentes
		NF_LoadTextFont16("font/records", "track_select", 512, 256, 0);
		NF_LoadTextFont16("font/records", "track_name", 768, 256, 0);
		NF_LoadTextFont16("font/records", "records", 256, 256, 0);

		// Carga los sprites
		for (n = 0; n < TOTAL_TRACKS; n ++) {
			sprintf(file, "sprite/trackselect/track%02d", (n + 1));
			NF_LoadSpriteGfx(file, n, 64, 64);
			NF_LoadSpritePal(file, n);
		}

	}



	// Funcion CreateTrackSelectMenu();
	void CreateTrackSelectMenu(void) {

		// Variables
		u8 n = 0;

		// Crea los fondos para la pantalla superior
		NF_CreateTiledBg(0, 3, "top_bg3");
		NF_CreateTiledBg(0, 2, "top_bg2");
		NF_ScrollBg(0, 2, 0, 1);

		// Crea los fondos para la pantalla inferior
		NF_CreateTiledBg(1, 3, "bg3_bottom");
		NF_ScrollBg(1, 3, 64, 64);
		NF_CreateTiledBg(1, 2, "bg2_bottom");
		REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
		REG_BLDALPHA_SUB = 0x03 | (0x03 << 8);
		NF_CreateTiledBg(1, 1, "bg1_bottom");

		// Crea la capa de texto en la pantalla superior
		NF_CreateTextLayer16(0, 1, 0, "track_select");
		NF_WriteText16(0, 1, 0, 1, "TRACK SELECT     TRACK SELECT     TRACK SELECT     ");
		NF_CreateTextLayer16(0, 0, 0, "track_name");
		
		// Crea la capa de texto en la pantalla inferior
		NF_CreateTextLayer16(1, 0, 0, "records");

		// Transfiere paletas de los Sprites
		for (n = 0; n < TOTAL_TRACKS; n ++) {
			NF_VramSpritePal(0, n, n);
		}

		// Ahora coloca los que tocan
		TrackSelectSprites(0);

		// Actualiza el OAM
		NF_SpriteOamSet(0);

		// Espera al sincronismo
		swiWaitForVBlank();

		// Actualiza el VRAM del OAM
		oamUpdate(&oamMain);

	}



	// Funcion TrackSelectSprites();
	void TrackSelectSprites(u8 id) {

		// Variables
		s16 temp_x = 0;
		s8 gfx = 0;
		s8 n = 0;
		s16 x = 0;
		s16 y = 0;

		// Calcula el Gfx del Sprite central
		ROLLERMENU[(2 << 1)].x = (128 - 64);
		ROLLERMENU[((2 << 1) + 1)].x = 128;
		ROLLERMENU[(2 << 1)].gfx = id;
		ROLLERMENU[((2 << 1) + 1)].gfx = id;


		// Calcula los Gfx de los sprites a la derecha
		gfx = id;
		temp_x = 128;

		for (n = 3; n <= 4; n ++) {

			// Graficos
			gfx ++;
			if (gfx > (TOTAL_TRACKS - 1)) gfx = 0;
			ROLLERMENU[(n << 1)].gfx = gfx;
			ROLLERMENU[((n << 1) + 1)].gfx = gfx;

			// Posicion
			temp_x += 80;
			ROLLERMENU[(n << 1)].x = temp_x;
			temp_x += 64;
			ROLLERMENU[((n << 1) + 1)].x = temp_x;

		}

		// Calcula los Gfx de los sprites a la izquierda
		gfx = id;
		temp_x = 128;

		for (n = 1; n >= 0; n --) {

			// Graficos
			gfx --;
			if (gfx < 0) gfx = (TOTAL_TRACKS - 1);
			ROLLERMENU[(n << 1)].gfx = gfx;
			ROLLERMENU[((n << 1) + 1)].gfx = gfx;

			// Posicion
			temp_x -= 208;
			ROLLERMENU[(n << 1)].x = temp_x;
			temp_x += 64;
			ROLLERMENU[((n << 1) + 1)].x = temp_x;

		}


		// Crea todos los sprites
		y = 64;
		for (n = 0; n < 10; n ++) {

			NF_VramSpriteGfx(0, ROLLERMENU[n].gfx, n, true);

			x = ROLLERMENU[n].x;
			if (x < -64) x = -64;
			if (x > 256) x = 256;
			NF_CreateSprite(0, n, n, ROLLERMENU[n].gfx, x, y);
			NF_SpriteFrame(0, n, (n % 2));

		}
		
	}



	// Funcion TrackSelectMoveMenu();
	bool TrackSelectMoveMenu(s16 move_x) {

		// Variables
		u8 n = 0;
		s16 x = 0;
		s16 y = 64;
		u8 id = 255;
		s8 gfx = 0;
		bool state;

		// Mueve los sprites
		for (n = 0; n < 10; n ++) {

			// Calcula el movimiento y posiciona el sprite
			ROLLERMENU[n].x += move_x;
			x = ROLLERMENU[n].x;
			if (x < -64) x = -64;
			if (x > 256) x = 256;
			NF_MoveSprite(0, n, x, y);

			// Analiza si es el final de movimiento, segun el sentido
			if (move_x > 0) {
				if (ROLLERMENU[n].x >= 560) id = (n - 1);
			} else {
				if (ROLLERMENU[n].x <= -368) id = n;
			}

		}


		// Si has llegado al final, indicalo y haz el cambio de sprites
		if (id != 255) {

			// Borra el sprite y gfx del elemento antiguo
			NF_DeleteSprite(0, id);
			NF_DeleteSprite(0, (id + 1));
			NF_FreeSpriteGfx(0, id);
			NF_FreeSpriteGfx(0, (id + 1));

			if (move_x > 0) {
				// Reposicion los sprites
				ROLLERMENU[id].x = -224;
				ROLLERMENU[(id + 1)].x = -160;
				// Calcula el sprite nuevo
				gfx = ROLLERMENU[id].gfx;
				gfx += 1;
				if (gfx > (TOTAL_TRACKS - 1)) gfx = 0;
			} else {
				// Reposicion los sprites
				ROLLERMENU[id].x = 352;
				ROLLERMENU[(id + 1)].x = 416;
				// Calcula el sprite nuevo
				gfx = ROLLERMENU[id].gfx;
				gfx -= 1;
				if (gfx < 0) gfx = (TOTAL_TRACKS - 1);
			}

			// Manda el nuevo GFX a la VRAM y crea el Sprite
			ROLLERMENU[id].gfx = gfx;
			ROLLERMENU[(id + 1)].gfx = gfx;
			NF_VramSpriteGfx(0, gfx, id, true);
			NF_VramSpriteGfx(0, gfx, (id + 1), true);
			NF_CreateSprite(0, id, id, ROLLERMENU[id].gfx, ROLLERMENU[id].x, y);
			NF_CreateSprite(0, (id + 1), (id + 1), ROLLERMENU[(id + 1)].gfx, ROLLERMENU[(id + 1)].x, y);
			NF_SpriteFrame(0, id, 0);
			NF_SpriteFrame(0, (id + 1), 1);

		}


		// Devuelve si has terminado
		if (id == 255) {
			state = false;
		} else {
			state = true;
		}
		return state;

	}





#ifdef __cplusplus
}
#endif
