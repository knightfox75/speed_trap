#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de menu de opciones
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
	#include "options.h"
	#include "system.h"
	#include "defines.h"
	#include "save.h"
	#include "customcar.h"





	// Funcion OptionsMenu();
	u8 OptionsMenu(void) {

		// Resetea todo
		ResetAll();

		// Variables
		s16 fade = (16 << 8);		// Brillo
		bool fade_in = true;		// Flag Fade in
		bool fade_out = false;		// Flag Fade Out

		s32 bg3_x = 0;				// Coordenadas del fondo mobil
		s32 bg3_y = 0;

		s32 bg2_x = 256;			// Coordenadas menu principal

		s32 bg0_x = 0;				// Coordenadas submenus

		s8 option = 0;				// Opcion del menu seleccionada
		s8 reset = 0;				// Opcion del menu reset seleccionada
		bool newoption = false;		// Cambio de opcion?
		bool execute = false;		// Ejecuta la opcion seleccionada

		bool mainmenu = true;		// Control del menu seleccionado
		bool difmenu = false;
		bool resetmenu = false;

		bool menu_in = false;		// Flag menu in
		bool menu_out = false;		// Flag menu out

		bool loop = true;			// Loop del menu

		// Pantalla en negro
		setBrightness(3, -16);

		// Carga los archivos de datos
		LoadOptionsMenuFiles();

		// Crea el menu de opciones
		CreateOptionsMenu();

		// Bucle del menu
		while (loop) {

			// Control del menu
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
				setBrightness(3, (0 - (fade >> 8)));
			}



			//////////////////////
			// Bloque MAIN MENU //
			//////////////////////

			if (mainmenu) {

				// Si no hay ningun efecto pendiente
				if (!fade_in && !fade_out) {

					// Si pulsas abajo
					if (KEYPRESS.down) {
						option ++;
						if (option > 3) option = 3;
						newoption = true;
					}

					// Si pulsas arriba
					if (KEYPRESS.up) {
						option --;
						if (option < 0) option = 0;
						newoption = true;
					}

					// Si pulsas A
					if (KEYPRESS.a && !execute) {
						execute = true;
					}

					// Si pulsas B
					if (KEYPRESS.b) {
						fade_out = true;
					}

				}

				// Cambia el fondo si es necesario
				if (newoption) {
					// Borra el fondo actual
					NF_DeleteTiledBg(1, 1);
					// Crea el fondo nuevo
					switch (option) {
						case 0:
							NF_CreateTiledBg(1, 1, "bg1_a");
							break;
						case 1:
							NF_CreateTiledBg(1, 1, "bg1_b");
							break;
						case 2:
							NF_CreateTiledBg(1, 1, "bg1_c");
							break;
						case 3:
							NF_CreateTiledBg(1, 1, "bg1_d");
							break;
					}
					// Reposiciona el fondo
					NF_ScrollBg(1, 1, 256, 0);
					// Flag abajo
					newoption = false;
				}

				// Ejecuta las funciones suplementarias
				if (execute) {
					// Dependiendo de la opcion seleccionada
					switch (option) {
						// Menu de seleccion de dificultad
						case 0:
							// Inicializa las variables
							bg2_x = 256;
							bg0_x = 0;
							mainmenu = false;
							difmenu = true;
							menu_in = true;
							menu_out = false;
							// Crea el fondo
							switch (OPTIONS[0].difficulty) {
								case 0:
									NF_CreateTiledBg(1, 0, "diff_a");
									break;
								case 1:
									NF_CreateTiledBg(1, 0, "diff_b");
									break;
								case 2:
									NF_CreateTiledBg(1, 0, "diff_c");
									break;
							}
							// Y posicionalo
							NF_ScrollBg(1, 0, 0, 0);
							break;
						// Menu parametrizacion de los coches
						case 1:
							fade_out = true;
							break;
						// Menu parametrizacion del sonido
						case 2:
							fade_out = true;
							break;
						// Menu de borrado de datos
						case 3:
							// Inicializa las variables
							bg2_x = 256;
							bg0_x = 256;
							mainmenu = false;
							resetmenu = true;
							menu_in = true;
							menu_out = false;
							reset = 1;
							// Crea el fondo
							NF_CreateTiledBg(1, 0, "reset_b");
							// Y posicionalo
							NF_ScrollBg(1, 0, 256, 0);
							break;
					}
					// Flag abajo
					execute = false;
				}

			}



			////////////////////////////
			// Bloque MENU DIFICULTAD //
			////////////////////////////

			if (difmenu) {

				// Entrada del menu
				if (menu_in) {
					// Calcula el movimiento
					bg2_x += 8;
					bg0_x += 8;

					// Limite del movimiento
					if (bg0_x >= 256) {
						bg0_x = 256;
						bg2_x = 512;
						menu_in = false;
					}

					// Scroll de los fondos
					NF_ScrollBg(1, 2, bg2_x, 0);
					NF_ScrollBg(1, 1, bg2_x, 0);
					NF_ScrollBg(1, 0, bg0_x, 0);

				}

				// Salida del menu
				if (menu_out) {
					// Calcula el movimiento
					bg2_x -= 8;
					bg0_x -= 8;

					// Limite del movimiento
					if (bg0_x <= 0) {
						bg0_x = 0;
						bg2_x = 256;
						menu_out = false;
						// Borra el fondo
						NF_DeleteTiledBg(1, 0);
						// Sal del menu
						mainmenu = true;
						difmenu = false;
					}

					// Scroll de los fondos
					NF_ScrollBg(1, 2, bg2_x, 0);
					NF_ScrollBg(1, 1, bg2_x, 0);
					NF_ScrollBg(1, 0, bg0_x, 0);

				}


				// Lectura del teclado
				if (!menu_in && !menu_out) {

					// Si pulsas abajo
					if (KEYPRESS.down) {
						OPTIONS[0].difficulty ++;
						if (OPTIONS[0].difficulty > 2) OPTIONS[0].difficulty = 2;
						newoption = true;
					}

					// Si pulsas arriba
					if (KEYPRESS.up) {
						OPTIONS[0].difficulty --;
						if (OPTIONS[0].difficulty < 0) OPTIONS[0].difficulty = 0;
						newoption = true;
					}

					// Si pulsas A
					if (KEYPRESS.a || KEYPRESS.b) {
						menu_out = true;
					}

				}


				// Cambio de la imagen del menu
				if (newoption) {
					// Borra el fondo actual
					NF_DeleteTiledBg(1, 0);
					// Crea el fondo nuevo
					switch (OPTIONS[0].difficulty) {
						case 0:
							NF_CreateTiledBg(1, 0, "diff_a");
							break;
						case 1:
							NF_CreateTiledBg(1, 0, "diff_b");
							break;
						case 2:
							NF_CreateTiledBg(1, 0, "diff_c");
							break;
					}
					// Scroll de los fondos
					NF_ScrollBg(1, 2, bg2_x, 0);
					NF_ScrollBg(1, 1, bg2_x, 0);
					NF_ScrollBg(1, 0, bg0_x, 0);
					// Flags abajo
					newoption = false;
				}

			}



			////////////////////////////////
			// Bloque MENU RESET DE DATOS //
			////////////////////////////////

			if (resetmenu) {

				// Entrada del menu
				if (menu_in) {
					// Calcula el movimiento
					bg2_x -= 8;
					bg0_x -= 8;

					// Limite del movimiento
					if (bg0_x <= 0) {
						bg0_x = 0;
						bg2_x = 0;
						menu_in = false;
					}

					// Scroll de los fondos
					NF_ScrollBg(1, 2, bg2_x, 0);
					NF_ScrollBg(1, 1, bg2_x, 0);
					NF_ScrollBg(1, 0, bg0_x, 0);

				}

				// Salida del menu
				if (menu_out) {
					// Calcula el movimiento
					bg2_x += 8;
					bg0_x += 8;

					// Limite del movimiento
					if (bg0_x >= 256) {
						bg0_x = 256;
						bg2_x = 256;
						menu_out = false;
						// Borra el fondo
						NF_DeleteTiledBg(1, 0);
						// Sal del menu
						mainmenu = true;
						resetmenu = false;
					}

					// Scroll de los fondos
					NF_ScrollBg(1, 2, bg2_x, 0);
					NF_ScrollBg(1, 1, bg2_x, 0);
					NF_ScrollBg(1, 0, bg0_x, 0);

				}


				// Lectura del teclado
				if (!menu_in && !menu_out) {

					// Si pulsas abajo
					if (KEYPRESS.down) {
						reset ++;
						if (reset > 1) reset = 1;
						newoption = true;
					}

					// Si pulsas arriba
					if (KEYPRESS.up) {
						reset --;
						if (reset < 0) reset = 0;
						newoption = true;
					}

					// Si pulsas A, borra los datos si esta seleccionada la opcion
					if (KEYPRESS.a) {
						menu_out = true;
						// Si esta seleccionado "YES", resetea los datos
						if (reset == 0) InitGameData();
					}

					// Si pulsas B, sal del menu, sin borrar
					if (KEYPRESS.b) {
						menu_out = true;
					}

				}


				// Cambio de la imagen del menu
				if (newoption) {
					// Borra el fondo actual
					NF_DeleteTiledBg(1, 0);
					// Crea el fondo nuevo
					switch (reset) {
						case 0:
							NF_CreateTiledBg(1, 0, "reset_a");
							break;
						case 1:
							NF_CreateTiledBg(1, 0, "reset_b");
							break;
					}
					// Scroll de los fondos
					NF_ScrollBg(1, 2, bg2_x, 0);
					NF_ScrollBg(1, 1, bg2_x, 0);
					NF_ScrollBg(1, 0, bg0_x, 0);
					// Flags abajo
					newoption = false;
				}

			}



			////////////////////
			// Tareas comunes //
			////////////////////

			// Calcula el movimiento del fondo BG3
			bg3_x += 128;
			bg3_y += 64;
			if (bg3_x > (256 << 8)) bg3_x -= (256 << 8);
			if (bg3_y > (256 << 8)) bg3_y -= (256 << 8);

			// Espera al sincronismo vertical
			swiWaitForVBlank();

			// Mueve el fondo BG3
			NF_ScrollBg(0, 3, (bg3_x >> 8), (bg3_y >> 8));
			NF_ScrollBg(1, 3, (bg3_x >> 8), (bg3_y >> 8));

		}

		// Al salir, guarda los datos
		WriteSavegame();

		// Devuelve el resultado de la salida
		return option;

	}



	// Funcion LoadOptionsMenuFiles();
	void LoadOptionsMenuFiles(void) {

		// Logo
		NF_LoadTiledBg("bg/options/bg_top", "bg_top", 256, 256);
		// Carga los archivos de fondos
		NF_LoadTiledBg("bg/options/bg3", "bg3", 512, 512);
		// Mascara
		NF_LoadTiledBg("bg/options/bg2", "bg2", 768, 256);
		// Menu principal
		NF_LoadTiledBg("bg/options/bg1_a", "bg1_a", 768, 256);
		NF_LoadTiledBg("bg/options/bg1_b", "bg1_b", 768, 256);
		NF_LoadTiledBg("bg/options/bg1_c", "bg1_c", 768, 256);
		NF_LoadTiledBg("bg/options/bg1_d", "bg1_d", 768, 256);
		// Menu dificultad
		NF_LoadTiledBg("bg/options/diff_a", "diff_a", 512, 256);
		NF_LoadTiledBg("bg/options/diff_b", "diff_b", 512, 256);
		NF_LoadTiledBg("bg/options/diff_c", "diff_c", 512, 256);
		// Menu reseteo de datos
		NF_LoadTiledBg("bg/options/reset_a", "reset_a", 512, 256);
		NF_LoadTiledBg("bg/options/reset_b", "reset_b", 512, 256);

	}



	// Funcion CreateOptionsMenu();
	void CreateOptionsMenu(void) {

		// Pantalla superior
		NF_CreateTiledBg(0, 3, "bg3");
		NF_CreateTiledBg(0, 2, "bg_top");

		// Pantalla inferior
		NF_CreateTiledBg(1, 3, "bg3");
		NF_CreateTiledBg(1, 2, "bg2");
		NF_ScrollBg(1, 2, 256, 0);
		NF_CreateTiledBg(1, 1, "bg1_a");
		NF_ScrollBg(1, 1, 256, 0);

		// Efectos alpha
		REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
		REG_BLDALPHA_SUB = 0x06 | (0x06 << 8);

	}





#ifdef __cplusplus
}
#endif