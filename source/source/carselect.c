#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones del menu de seleccion del coche
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
	#include "carselect.h"
	#include "sound.h"
	#include "system.h"
	#include "defines.h"



	// Funcion CarSelect();
	u8 CarSelect(void) {

		// Resetea todo
		ResetAll();

		// Variables locales
		s32 bg3_x = (256 << 8);		// Posicion del fondo 3, pantalla inferior
		s32 bg3_y = (256 << 8);

		s32 cs_x = 0;				// Posicion texto "CAR SELECT"

		bool next = false;			// Cambio a siguiente circuito
		bool back = false;			// Cambio a circuito anterior

		s8 option = 0;					// Opcion del menu seleccionada
		u8 last = (CAR_TOTALNUM - 1);	// Ultima opcion del menu

		char car_name[64];			// Guarda el texto con el nombre del coche
		s32 cn_x = 0;				// Coordenada X
		s32 cn_limit = 0;			// Limite del movimiento

		s16 fade = (16 << 8);		// Brillo
		bool fade_in = true;		// Flag Fade in
		bool fade_out = false;		// Flag Fade Out

		char text[32];				// Buffer de texto
		bool newtext = true;		// Actualiza los textos

		bool menu = false;			// Estado del movimiento del menu

		bool cancel = false;		// Se ha cancelado en este menu?

		bool loop = true;			// Loop del menu

		s16 stats = 0;				// Estadisticas

		// Pantalla en negro
		setBrightness(3, -16);

		// Carga los archivos necesarios
		LoadCarSelectFiles();

		// Crea el menu
		CreateCarSelectMenu();


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
				// Ajusta el brillo
				setBrightness(3, (0 - (fade >> 8)));
				// Ajusta el volumen
				if (!cancel) BgmVolume(((4096 - fade) >> 2));
			}

			// Mueve el fondo (bandera)
			bg3_x -= 128;
			if (bg3_x < 0) bg3_x += (256 << 8);
			bg3_y -= 64;
			if (bg3_y < 0) bg3_y += (256 << 8);

			// Mueve el texto "CAR SELECT"
			cs_x += 128;
			if (cs_x > (120 << 8)) cs_x -= (120 << 8);

			// Mueve el texto "CAR NAME"
			cn_x += 128;
			if (cn_x > (cn_limit << 8)) cn_x -= (cn_limit << 8);


			// Si el menu se mueve a la derecha (siguiente)
			if (next) {
				menu = CarSelectMoveMenu(-8);
				if (menu) {
					option ++;
					if (option > last) option = 0;
					newtext = true;
					next = false;
				}
			}

			// Si el menu se mueve a la izquierda (anterior)
			if (back) {
				menu = CarSelectMoveMenu(8);
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

				// Nombre del coche
				switch (option) {
					case 0:
						sprintf(text, "BLUE ICE            ");
						break;
					case 1:
						sprintf(text, "PURPLE EMOTION      ");
						break;
					case 2:
						sprintf(text, "GREEN FEEL          ");
						break;
					case 3:
						sprintf(text, "LIMA PASSION        ");
						break;
					case 4:
						sprintf(text, "RED BARON           ");
						break;
					case 5:
						sprintf(text, "CRIMSON WEELS       ");
						break;
					case 6:
						sprintf(text, "FIRE BIRD           ");
						break;
					case 7:
						sprintf(text, "BLACK KNIGHT        ");
						break;
					case 8:
						sprintf(text, "WHITE SKULL         ");
						break;
					case 9:
						sprintf(text, "DARK AVENGER        ");
						break;
				}
				cn_limit = (strlen(text) << 3);
				sprintf(car_name, "%s%s%s", text, text, text);
				NF_WriteText16(0, 0, 0, 10, car_name);

				// Actualiza la capa de textos
				NF_UpdateTextLayers();


				// Actualiza las estadisticas del coche

				// Velocidad
				stats = (((CARPARAM[option].speed - CAR_MIN_SPEED) >> 5) + 4);
				SetStatusBar(0, stats);
				// Aceleracion
				stats = (CARPARAM[option].acc + 4);
				SetStatusBar(1, stats);
				// Franado
				stats = ((CARPARAM[option].brk >> 1) + 4);
				SetStatusBar(2, stats);
				// Giro
				stats = (CARPARAM[option].turn >> 1);
				SetStatusBar(3, stats);
				// Peso
				stats = (12 - (CARPARAM[option].inertia - 4));
				SetStatusBar(4, stats);

			}

			// Actualiza el OAM
			NF_SpriteOamSet(0);
			NF_SpriteOamSet(1);

			// Espera al sincronismo
			swiWaitForVBlank();

			// Actualiza el VRAM del OAM
			oamUpdate(&oamMain);
			oamUpdate(&oamSub);

			// Posiciona el fondo (bandera)
			NF_ScrollBg(1, 3, (bg3_x >> 8), (bg3_y >> 8));

			// Posiciona el texto "CAR SELECT"
			NF_ScrollBg(0, 1, (cs_x >> 8), 0);

			// Posiciona el texto "CAR NAME"
			NF_ScrollBg(0, 0, (cn_x >> 8), 0);

		}

		// Si se ha cancelado...
		if (cancel) option = 255;

		// Devuelve el coche seleccionado
		return option;

	}



	// Funcion LoadCarSelectFiles();
	void LoadCarSelectFiles(void) {

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
		NF_LoadTextFont16("font/records", "car_select", 512, 256, 0);
		NF_LoadTextFont16("font/records", "car_name", 768, 256, 0);
		NF_LoadTextFont16("font/records", "records", 256, 256, 0);

		// Carga los sprites de las miniaturas
		for (n = 0; n < CAR_TOTALNUM; n ++) {
			sprintf(file, "sprite/carselect/car%02d", n);
			NF_LoadSpriteGfx(file, n, 64, 64);
			NF_LoadSpritePal(file, n);
		}
		// Carga los sprites de la barra de estado
		NF_LoadSpriteGfx("sprite/carselect/status_bar", CAR_TOTALNUM, 32, 16);
		NF_LoadSpritePal("sprite/carselect/status_bar", CAR_TOTALNUM);

	}



	// Funcion CreateCarSelectMenu();
	void CreateCarSelectMenu(void) {

		// Variables
		u8 n = 0;
		u8 x = 0;
		u8 y = 0;

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
		NF_CreateTextLayer16(0, 1, 0, "car_select");
		NF_WriteText16(0, 1, 0, 1, "CAR SELECT     CAR SELECT     CAR SELECT     CAR SELECT     ");
		NF_CreateTextLayer16(0, 0, 0, "car_name");
		
		// Crea la capa de texto en la pantalla inferior
		NF_CreateTextLayer16(1, 0, 0, "records");

		// Cabecera
		NF_WriteText16(1, 0, 10, 2, "CAR FEATURES");

		// Testo caracteristicas coches
		NF_WriteText16(1, 0, 6, 4, "SPEED");
		NF_WriteText16(1, 0, 6, 5, "ACCEL");
		NF_WriteText16(1, 0, 6, 6, "BRAKE");
		NF_WriteText16(1, 0, 6, 7, "TURN");
		NF_WriteText16(1, 0, 6, 8, "WEIGHT");

		// Transfiere paletas de los Sprites
		for (n = 0; n < CAR_TOTALNUM; n ++) {
			NF_VramSpritePal(0, n, n);
		}
		NF_VramSpritePal(1, CAR_TOTALNUM, 0);

		// Transfiere los graficos de las barras de estado
		NF_VramSpriteGfx(1, CAR_TOTALNUM, 0, false);

		// Crea los sprites
		CarSelectSprites(0);

		n = 0;
		// Crea las barras de estado
		for (y = 0; y < 5; y ++) {
			for (x = 0; x < 3; x ++) {
				NF_CreateSprite(1, n, 0, 0, ((x << 5) + 112), ((y + 4) << 4));
				NF_SpriteLayer(1, n, 0);
				NF_SpriteFrame(1, n, 0);
				n ++;
			}
		}

		// Actualiza el OAM
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);

		// Espera al sincronismo
		swiWaitForVBlank();

		// Actualiza el VRAM del OAM
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

	}



	// Funcion CarSelectSprites();
	void CarSelectSprites(u8 id) {

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
			if (gfx > (CAR_TOTALNUM - 1)) gfx = 0;
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
			if (gfx < 0) gfx = (CAR_TOTALNUM - 1);
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



	// Funcion CarSelectMoveMenu();
	bool CarSelectMoveMenu(s16 move_x) {

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
				gfx += 5;
				if (gfx > (CAR_TOTALNUM - 1)) gfx -= CAR_TOTALNUM;
			} else {
				// Reposicion los sprites
				ROLLERMENU[id].x = 352;
				ROLLERMENU[(id + 1)].x = 416;
				// Calcula el sprite nuevo
				gfx = ROLLERMENU[id].gfx;
				gfx -= 5;
				if (gfx < 0) gfx += CAR_TOTALNUM;
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



	// Funcion SetStatusBar();
	void SetStatusBar(u8 bar, u8 value) {

		// Variables
		u8 bar_id = (bar * 3);

		s8 part_a = value;
		if (part_a < 0) part_a = 0;
		if (part_a > 4) part_a = 4;

		s8 part_b = (value - 4);
		if (part_b < 0) part_b = 0;
		if (part_b > 4) part_b = 4;

		s8 part_c = (value - 8);
		if (part_c < 0) part_c = 0;
		if (part_c > 4) part_c = 4;

		// Muestra la barra
		NF_SpriteFrame(1, (bar_id + 0), part_a);
		NF_SpriteFrame(1, (bar_id + 1), part_b);
		NF_SpriteFrame(1, (bar_id + 2), part_c);

	}





#ifdef __cplusplus
}
#endif
