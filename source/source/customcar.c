#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de parametrizacion de los coches
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
	#include "customcar.h"
	#include "system.h"
	#include "defines.h"
	#include "carselect.h"	




	// Funcion CustomCarMenu();
	void CustomCarMenu(void) {

		// Borralo todo
		ResetAll();

		// Variables
		s16 fade = (16 << 8);		// Brillo
		bool fade_in = true;		// Flag Fade in
		bool fade_out = false;		// Flag Fade Out

		s32 bg3_x = 0;				// Coordenadas del fondo mobil
		s32 bg3_y = 0;

		s32 bg1top_x = 0;			// Coordenadas texto "Customize your car"

		bool next = false;			// Cambio a siguiente circuito
		bool back = false;			// Cambio a circuito anterior

		s8 option = 0;					// Opcion del menu seleccionada
		u8 last = (CAR_TOTALNUM - 1);	// Ultima opcion del menu

		char car_name[64];			// Guarda el texto con el nombre del coche
		s32 bg0top_x = 0;			// Coordenada X
		s32 bg0top_limit = 0;		// Limite del movimiento

		char text[32];				// Buffer de texto
		bool newtext = true;		// Actualiza los textos

		bool menu = false;			// Estado del movimiento del menu

		s16 stats = 0;				// Estadisticas

		bool car_select = true;		// Menu seleccion de coche
		bool car_param = false;		// Menu parametrizacion del coche
		bool loop_back = false;		// Previene la duplicaciones de pulsaciones

		bool loop = true;			// Loop del menu


		// Variables del control de paramtrizacion


		bool update_stats = false;		// Actualiza la pantalla de estados
		bool update_params = false;		// Actualiza los parametros
		bool update_points = false;		// Actualiza la barra de puntos
		s8 points = 0;					// Puntos a repartir
		s8 id = 0;						// Parametro seleccionado

		// Backup de los parametros
		typedef struct {
			s32 speed;
			s32 acc;
			s32 brk;
			s32 turn;
			s32 inertia;
		} backup_info;
		backup_info backup;
		backup.speed = 0;
		backup.acc = 0;
		backup.brk = 0;
		backup.turn = 0;
		backup.inertia = 0;


		// Pantalla en negro
		setBrightness(3, -16);

		// Carga los archivos necesarios
		LoadCustomCarFiles();

		// Crea el menu
		CreateCustomCarMenu();

		// Bucle
		while (loop) {

			// Lee el teclado
			ReadKeypad();
			// Borra el flag de duplicacion de pulsacion
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
				setBrightness(3, (0 - (fade >> 8)));
			}



			/////////////////////////////////
			// Menu de seleccion del coche //
			/////////////////////////////////

			if (car_select) {

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
					// Ejecuta la paramatrizacion del coche
					if (KEYPRESS.a && !loop_back) {
						ShowCustomCarBar();		// Muestra la barra
						UpdateParamBar(0);		// Y actualizala
						id = 0;					// Id de parametro a 0
						///////////////////////////////////////////
						// Guarda los parametros actuales
						backup.speed = CARPARAM[option].speed;
						backup.acc = CARPARAM[option].acc;
						backup.brk = CARPARAM[option].brk;
						backup.turn = CARPARAM[option].turn;
						backup.inertia = CARPARAM[option].inertia;
						///////////////////////////////////////////
						update_stats = false;	// Flags de actualizacion de marcadores
						update_params = false;
						update_points = false;
						points = 0;				// Puntos a repartir
						car_select = false;
						car_param = true;
						loop_back = true;
					}
					// Sal del menu
					if (KEYPRESS.b && !loop_back) {
						car_select = false;
						fade_out = true;
						loop_back = true;
					}
				}

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
					bg0top_limit = (strlen(text) << 3);
					sprintf(car_name, "%s%s%s", text, text, text);
					NF_WriteText16(0, 0, 0, 10, car_name);

					// Actualiza la capa de textos
					NF_UpdateTextLayers();

					// Marca para actualizar los indicadores de parametros
					update_params = true;

				}

			}



			///////////////////////////////////////
			// Menu de parametrizacion del coche //
			///////////////////////////////////////

			if (car_param) {

				// Muevete por los parametros
				if (KEYPRESS.up) {
					id --;
					if (id < 0) id = 0;
					update_stats = true;
				}
				if (KEYPRESS.down) {
					id ++;
					if (id > 4) id = 4;
					update_stats = true;
				}

				// Modifica los parametros (-)
				if (KEYPRESS.left) {
					// Segun el parametro seleccionado
					switch (id) {
						// Velocidad
						case 0:
							if (CARPARAM[option].speed > CAR_MIN_SPEED) {
								CARPARAM[option].speed -= 32;
								points ++;
								update_points = true;
								update_params = true;
							}
							break;
						// Aceleracion
						case 1:
							if (CARPARAM[option].acc > CAR_MIN_ACC) {
								CARPARAM[option].acc -= 1;
								points ++;
								update_points = true;
								update_params = true;
							}
							break;
						// Frenado
						case 2:
							if (CARPARAM[option].brk > CAR_MIN_BRAKE) {
								CARPARAM[option].brk -= 2;
								points ++;
								update_points = true;
								update_params = true;
							}
							break;
						// Giro
						case 3:
							if (CARPARAM[option].turn > CAR_MIN_TURN) {
								CARPARAM[option].turn -= 2;
								points ++;
								update_points = true;
								update_params = true;
							}
							break;
						// Inercia (Peso)
						case 4:
							if (CARPARAM[option].inertia < CAR_MIN_INERTIA) {
								CARPARAM[option].inertia += 1;
								points ++;
								update_points = true;
								update_params = true;
							}
							break;
					}
				}

				// Modifica los parametros (+)
				if (KEYPRESS.right && (points > 0)) {
					// Segun el parametro seleccionado
					switch (id) {
						// Velocidad
						case 0:
							if (CARPARAM[option].speed < CAR_MAX_SPEED) {
								CARPARAM[option].speed += 32;
								points --;
								update_points = true;
								update_params = true;
							}
							break;
						// Aceleracion
						case 1:
							if (CARPARAM[option].acc < CAR_MAX_ACC) {
								CARPARAM[option].acc += 1;
								points --;
								update_points = true;
								update_params = true;
							}
							break;
						// Frenado
						case 2:
							if (CARPARAM[option].brk < CAR_MAX_BRAKE) {
								CARPARAM[option].brk += 2;
								points --;
								update_points = true;
								update_params = true;
							}
							break;
						// Giro
						case 3:
							if (CARPARAM[option].turn < CAR_MAX_TURN) {
								CARPARAM[option].turn += 2;
								points --;
								update_points = true;
								update_params = true;
							}
							break;
						// Inercia (Peso)
						case 4:
							if (CARPARAM[option].inertia > CAR_MAX_INERTIA) {
								CARPARAM[option].inertia -= 1;
								points --;
								update_points = true;
								update_params = true;
							}
							break;
					}

				}

				// Si ya se han repartido todos los puntos, pulsa A para volver
				if (KEYPRESS.a && (points == 0) && !loop_back) {
					HideCustomCarBar();		// Oculta la barra
					UpdateParamBar(0);		// Y actualizala
					id = 0;					// Id de parametro a 0
					update_stats = true;	// Flags de actualizacion de marcadores
					update_params = true;
					update_points = true;
					points = 0;				// Puntos a repartir
					car_select = true;
					car_param = false;
					loop_back = true;
				}

				// Si se pulsa B, cancela y restaura la configuracion del coche
				if (KEYPRESS.b && !loop_back) {
					///////////////////////////////////////////
					// Restaura los parametros originales
					CARPARAM[option].speed = backup.speed;
					CARPARAM[option].acc = backup.acc;
					CARPARAM[option].brk = backup.brk;
					CARPARAM[option].turn = backup.turn;
					CARPARAM[option].inertia = backup.inertia;
					///////////////////////////////////////////
					HideCustomCarBar();		// Oculta la barra
					UpdateParamBar(0);		// Y actualizala
					id = 0;					// Id de parametro a 0
					update_stats = true;	// Flags de actualizacion de marcadores
					update_params = true;
					update_points = true;
					points = 0;				// Puntos a repartir
					car_select = true;
					car_param = false;
					loop_back = true;
				}

				// Si es necesario, actualiza el marcador de minimos
				if (update_stats) {
					// Actualiza el indicador de parametro seleccionado
					UpdateParamBar(id); 
					// Marcalo como actualizado
					update_stats = false;
				}

				// Si es necesario, actualiza el contador de puntos disponibles
				if (update_points) {
					// Actualiza el indicador de puntos disponibles
					UpdatePointsBar(points);
					// Marcalo como actualizado
					update_points = false;
				}

			}



			////////////////////
			// Tareas comunes //
			////////////////////

			// Actualiza la barra de parametros del coche
			if (update_params) {
				// Velocidad
				stats = (((CARPARAM[option].speed - CAR_MIN_SPEED) >> 5) + 4);
				SetStatusBar(0, stats);
				// Aceleracion
				stats = (CARPARAM[option].acc + 4);
				SetStatusBar(1, stats);
				// Frenado
				stats = ((CARPARAM[option].brk >> 1) + 4);
				SetStatusBar(2, stats);
				// Giro
				stats = (CARPARAM[option].turn >> 1);
				SetStatusBar(3, stats);
				// Peso
				stats = (12 - (CARPARAM[option].inertia - 4));
				SetStatusBar(4, stats);
				// Marca como actualizados
				update_params = false;
			}

			// Calcula el movimiento del fondo BG3
			bg3_x += 128;
			bg3_y += 64;
			if (bg3_x > (256 << 8)) bg3_x -= (256 << 8);
			if (bg3_y > (256 << 8)) bg3_y -= (256 << 8);

			// Calcula el movimiento del texto "Customize your car"
			bg1top_x += 128;
			if (bg1top_x > (184 << 8)) bg1top_x -= (184 << 8);

			// Mueve el texto "CAR NAME"
			bg0top_x += 128;
			if (bg0top_x > (bg0top_limit << 8)) bg0top_x -= (bg0top_limit << 8);

			// Actualiza el OAM
			NF_SpriteOamSet(0);
			NF_SpriteOamSet(1);

			// Espera al sincronismo
			swiWaitForVBlank();

			// Actualiza el VRAM del OAM
			oamUpdate(&oamMain);
			oamUpdate(&oamSub);

			// Mueve el fondo BG3
			NF_ScrollBg(0, 3, (bg3_x >> 8), (bg3_y >> 8));
			NF_ScrollBg(1, 3, (bg3_x >> 8), (bg3_y >> 8));

			// Mueve el texto "Customize your car"
			NF_ScrollBg(0, 1, (bg1top_x >> 8), 0);

			// Mueve el texto "Car name"
			NF_ScrollBg(0, 0, (bg0top_x >> 8), 0);

		}

	}



	// Funcion LoadCustomCarFiles();
	void LoadCustomCarFiles(void) {

		// Variables
		u8 n = 0;
		char file[64];

		// Carga los archivos de fondos (Generales)
		NF_LoadTiledBg("bg/options/bg3", "bg3", 512, 512);
		NF_LoadTiledBg("bg/carcustom/bot_bg2", "bot_bg2", 256, 256);
		NF_LoadTiledBg("bg/carcustom/bot_bg1", "bot_bg1", 256, 256);

		// Fondos pantalla superior
		NF_LoadTiledBg("bg/carcustom/top_bg2", "top_bg2", 256, 256);

		// Carga los sprites de las miniaturas
		for (n = 0; n < CAR_TOTALNUM; n ++) {
			sprintf(file, "sprite/carselect/car%02d", n);
			NF_LoadSpriteGfx(file, n, 64, 64);
			NF_LoadSpritePal(file, n);
		}
		// Carga los sprites de la barra de estado
		NF_LoadSpriteGfx("sprite/carselect/status_bar", CAR_TOTALNUM, 32, 16);
		NF_LoadSpritePal("sprite/carselect/status_bar", CAR_TOTALNUM);
		// Barra de caracteristicas minimas
		NF_LoadSpriteGfx("sprite/carparam/param_bar", (CAR_TOTALNUM + 1), 32, 16);
		NF_LoadSpritePal("sprite/carparam/carparam", (CAR_TOTALNUM + 1));
		// Marcador de posicion
		NF_LoadSpriteGfx("sprite/carparam/select_bar", (CAR_TOTALNUM + 2), 64, 32);

		// Carga las fuentes
		NF_LoadTextFont16("font/carparam", "banner", 512, 256, 0);
		NF_LoadTextFont16("font/carparam", "car_name", 768, 256, 0);
		NF_LoadTextFont16("font/carparam", "params", 256, 256, 0);

	}



	// Funcion CreateCustomCarMenu();
	void CreateCustomCarMenu(void) {

		// Variables
		s16 n = 0;
		s16 x = 0;
		s16 y = 0;

		// Fondos pantalla superior
		NF_CreateTiledBg(0, 3, "bg3");
		NF_CreateTiledBg(0, 2, "top_bg2");
		// Alinea la capa 2
		NF_ScrollBg(0, 2, 0, 1);

		// Fondos pantalla inferior
		NF_CreateTiledBg(1, 3, "bg3");
		NF_CreateTiledBg(1, 2, "bot_bg2");
		NF_CreateTiledBg(1, 1, "bot_bg1");

		// Efectos alpha
		REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
		REG_BLDALPHA_SUB = 0x06 | (0x06 << 8);

		// Crea la capa de texto en la pantalla superior
		NF_CreateTextLayer16(0, 1, 0, "banner");
		NF_WriteText16(0, 1, 0, 1, "CUSTOMIZE YOUR CAR     CUSTOMIZE YOUR CAR     CUSTOMIZE YOUR");
		NF_CreateTextLayer16(0, 0, 0, "car_name");

		// Crea la capa de texto en la pantalla inferior
		NF_CreateTextLayer16(1, 0, 0, "params");

		// Cabecera
		NF_WriteText16(1, 0, 6, 2, "POINTS");

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
		NF_VramSpritePal(1, CAR_TOTALNUM, 0);			// Paletas de las barras de estado
		NF_VramSpritePal(1, (CAR_TOTALNUM + 1), 1);		// Paletas de las barras extra

		// Transfiere los graficos de las barras de estado
		NF_VramSpriteGfx(1, CAR_TOTALNUM, 0, false);
		// Barras de caracteristicas minimas
		NF_VramSpriteGfx(1, (CAR_TOTALNUM + 1), 1, false);
		// Marcador de posicion
		NF_VramSpriteGfx(1, (CAR_TOTALNUM + 2), 2, true);

		// Crea los sprites
		CarSelectSprites(0);

		n = 0;
		// Crea las barras de estado (0 - 14)
		for (y = 0; y < 5; y ++) {
			for (x = 0; x < 3; x ++) {
				NF_CreateSprite(1, n, 0, 0, ((x << 5) + 112), ((y + 4) << 4));
				NF_SpriteLayer(1, n, 1);
				NF_SpriteFrame(1, n, 0);
				n ++;
			}
		}
		// Barras de puntos restantes (15 - 17)
		for (x = 0; x < 3; x ++) {
			NF_CreateSprite(1, n, 0, 0, ((x << 5) + 112), (2 << 4));
			NF_SpriteLayer(1, n, 1);
			NF_SpriteFrame(1, n, 0);
			n ++;
		}
		// Barras de caracteristicas minimas (18 - 20)
		for (x = 0; x < 3; x ++) {
			NF_CreateSprite(1, n, 1, 1, ((x << 5) + 112), (4 << 4));
			NF_SpriteLayer(1, n, 0);
			NF_SpriteFrame(1, n, 0);
			NF_ShowSprite(1, n, false);
			n ++;
		}
		// Marcador de posicion (21)
		NF_CreateSprite(1, n, 2, 1, 40, (4 << 4));
		NF_SpriteLayer(1, n, 1);
		NF_ShowSprite(1, n, false);

		// Actualiza el OAM
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);

		// Actualiza las capas de texto
		NF_UpdateTextLayers();

		// Espera al sincronismo
		swiWaitForVBlank();

		// Actualiza el VRAM del OAM
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

	}



	// Funcion ShowCustomCarBar();
	void ShowCustomCarBar(void) {

		// Variables
		u8 n = 0;

		// Muestra los Sprites (18 - 21)
		for (n = 18; n < 22; n ++) {
			NF_ShowSprite(1, n, true);
		}

	}



	// Funcion HideCustomCarBar();
	void HideCustomCarBar(void) {

		// Variables
		u8 n = 0;

		// Oculta los Sprites (18 - 21)
		for (n = 18; n < 22; n ++) {
			NF_ShowSprite(1, n, false);
		}

	}



	// Funcion UpdateParamBar();
	void UpdateParamBar(u8 id) {

		// Variables
		u8 n = 0;
		u8 x = 0;
		s16 value = 0;

		// Posiciona la barra (18 - 20)
		for (n = 18; n < 21; n ++) {
			NF_MoveSprite(1, n, ((x << 5) + 112), ((id + 4) << 4));
			x ++;
		}

		// Marcador
		NF_MoveSprite(1, 21, 40, ((id + 4) << 4));

		// Captura el valor minimo del parametro
		switch (id) {
			// Velocidad
			case 0:
				value = 4;
				break;
			// Aceleracion
			case 1:
				value = (CAR_MIN_ACC + 4);
				break;
			// Frenado
			case 2:
				value = ((CAR_MIN_BRAKE >> 1) + 4);
				break;
			// Giro
			case 3:
				value = (CAR_MIN_TURN >> 1);
				break;
			// Peso
			case 4:
				value = CAR_MIN_INERTIA;
				break;
		}

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
		NF_SpriteFrame(1, 18, part_a);
		NF_SpriteFrame(1, 19, part_b);
		NF_SpriteFrame(1, 20, part_c);

	}



	// Funcion UpdatePointsBar();
	void UpdatePointsBar(u8 value) {

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
		NF_SpriteFrame(1, 15, part_a);
		NF_SpriteFrame(1, 16, part_b);
		NF_SpriteFrame(1, 17, part_c);

	}





#ifdef __cplusplus
}
#endif