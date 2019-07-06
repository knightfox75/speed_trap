#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de menu y splashscreens
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010



	// Includes c/c++
	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>
	#include <time.h>


	// Includes propietarios NDS
	#include <nds.h>

	// Includes NightFox's Lib
	#include <nf_lib.h>

	// Includes del juego
	#include "menu.h"
	#include "sound.h"
	#include "system.h"
	#include "defines.h"





	// Funcion SplashScreens();
	void SplashScreens(void) {

		// Resetea todo
		ResetAll();

		///////////////////////////////
		// Pon la DS en modo 16 bits //
		///////////////////////////////

		// Inicializa el motor 2D en modo BITMAP
		NF_Set2D(0, 5);				// Modo 2D_5 en ambas pantallas
		NF_Set2D(1, 5);
		// Resetea los buffers de 16 bits
		NF_Reset16bitsBgBuffers();
		// Inicializa los fondos en modo "BITMAP"
		NF_InitBitmapBgSys(0, 1);
		NF_InitBitmapBgSys(1, 1);
		// Habilita el backbuffer en ambas pantallas
		NF_Enable16bitsBackBuffer(0);
		NF_Enable16bitsBackBuffer(1);


		u8 thread = 0;			// Proceso
		bool next = false;		// Flag del siguiente proceso
		bool loop = true;		// Control del LOOP

		// Carga las imagenes necesarias
		NF_Load16bitsBg("bg/splash/sb2010", 0);
		NF_Load16bitsBg("bg/splash/neoflash", 1);
		NF_Load16bitsBg("bg/splash/devkitpro", 2);
		NF_Load16bitsBg("bg/splash/nfl", 3);
		NF_Load16bitsBg("bg/splash/nfandco_up", 4);
		NF_Load16bitsBg("bg/splash/nfandco_down", 5);


		while (loop) {

			// Nucleo de la funcion
			switch (thread) {

				case 0:	// Pantalla en negro y espera 30 segundos
					setBrightness(3, -16);
					WaitTime(30);
					next = true;
					break;

				case 1:	// Muestra las pantallas de SceneryBeta 2010 con fade in durante 90 frames
					NF_Copy16bitsBuffer(0, 1, 0);
					NF_Copy16bitsBuffer(1, 1, 1);
					swiWaitForVBlank();
					NF_Flip16bitsBackBuffer(0);
					NF_Flip16bitsBackBuffer(1);
					FadeIn(3, 64);
					WaitTime(90);
					next = true;
					break;

				case 2:	// Cross Disolve a devkitPro y NightFox's Lib y espera 90 frames
					CrossDisolve(0, 0, 2);
					CrossDisolve(1, 1, 3);
					WaitTime(90);
					next = true;
					break;

				case 3:	// Cross Disolve a NightFox & Co. y espera 90 frames
					CrossDisolve(0, 2, 4);
					CrossDisolve(1, 3, 5);
					WaitTime(90);
					next = true;
					break;

				case 4:	// Fade Out
					FadeOut(3, 64);
					next = true;
					break;

				case 5:	// Fin
					setBrightness(3, -16);
					WaitTime(30);
					loop = false;
					break;

			}


			// Si debes saltar al siguiente thread, hazlo
			if (next) {
				thread ++;
				next = false;
			}

			swiWaitForVBlank();		// Espera al sincronismo vertical

		}


		/////////////////////////////
		// Resetea el modo 16 bits //
		/////////////////////////////

		// Resetea los buffers de 16 bits
		NF_Reset16bitsBgBuffers();
		// Resetea los backbuffers y deshabilitalos
		NF_Disble16bitsBackBuffer(0);
		NF_Disble16bitsBackBuffer(1);	

	}



	// Funcion CrossDisolve();
	void CrossDisolve(u8 screen, u8 origin, u8 destination) {

		// Variables para obtener el RGB
		u16 rgb = 0;
		u8 r[3];
		u8 g[3];
		u8 b[3];

		// Variables adicionales
		u16 x = 0;
		u16 y = 0;
		u16 effect = 0;
		u16 size = 32;
		u16 speed = 8;
		u16 start = 0;
		u16 end = 0;

		for (effect = 0; effect < (192 + size + speed); effect += speed) {
		
			// Calcula el rango de la imagen destino
			if (effect >= size) {
				memcpy(NF_16BITS_BACKBUFFER[screen], NF_BG16B[destination].buffer, ((effect << 8) << 1));
			}

			// Calcula el rango del efecto
			if (effect < size) {
				start = 0;
				end = effect;
			} else {
				start = (effect - size);
				end = effect;
			}
			// Aplica el efecto
			for (y = start; y < end; y ++) {
				for (x = 0; x < 256; x ++) {
					// Si la linea del efecto esta dentro de la pantalla
					if (y < 192) {
						// Obten los valores RGB del destino
						rgb = NF_BG16B[destination].buffer[((y << 8) + x)];
						r[0] = (((rgb & 0x1F) * (end - y)) / size);
						g[0] = ((((rgb >> 5) & 0x1F) * (end - y)) / size);
						b[0] = ((((rgb >> 10) & 0x1F) * (end - y)) / size);
						// Obten los valores RGB del origen
						rgb = NF_BG16B[origin].buffer[((y << 8) + x)];
						r[1] = (((rgb & 0x1F) * (size - (end - y))) / size);
						g[1] = ((((rgb >> 5) & 0x1F) * (size - (end - y))) / size);
						b[1] = ((((rgb >> 10) & 0x1F) * (size - (end - y))) / size);
						// Calcula el valor del fade
						r[2] = (r[0] + r[1]);
						g[2] = (g[0] + g[1]);
						b[2] = (b[0] + b[1]);
						NF_16BITS_BACKBUFFER[screen][((y << 8) + x)] = ((r[2])|((g[2]) << 5)|((b[2]) << 10)|(BIT(15)));
					}
				}
			}

			// Espera al sincronismo vertical
			swiWaitForVBlank();

			// Vuelca la iamgen resultante
			NF_Flip16bitsBackBuffer(screen);

		}

	}



	// Funcion Main Menu
	u8 MainMenu(void) {

		// Variables
		bool loop = true;		// Control del loop
		u8 n = 0;				// Uso general
		s16 flag_x = 0;			// Posicion X de la minibandera
		s16 fade = (16 << 8);	// Brillo
		bool fade_in = true;	// Flag Fade in
		bool fade_out = false;	// Flag Fade Out
		bool button_in = false;	// Flag Button In
		s8 menu_option = 0;		// Opcion seelccionada
		s32 button_x[3];		// Posicion X del boton
		s32 button_y[3];		// Posicion X del boton
		s32 button_dx[3];		// Destino X del boton
		u8 button = 0;			// Boton a mover
		s32 x = 0;				// Uso global
		s32 timer = 0;			// Temporizador de autodemo

		//////////////////////////
		//						//
		//	Inicializaciones	//
		//						//
		//////////////////////////

		// Resetealo todo
		ResetAll();

		// Pon el modo adecuado a cada pantalla
		NF_Set2D(1, 2);	// Modo 2 en la pantalla inferior (modo affine)

		// Activa los fondos affine para la pantalla inferior
		NF_InitAffineBgSys(1);

		// Inicializa el sistema de Sprites para la pantalla inferior
		NF_InitSpriteSys(1);

		// Pantalla en negro
		setBrightness(3, -16);

		// Regenera el SEED para los numeros random
		srand(time(NULL));


		//////////////////////////
		//						//
		//	Carga de archivos	//
		//						//
		//////////////////////////

		// Carga los fondos tileados necesarios
		NF_LoadTiledBg("bg/menu/bigflag", "bigflag", 512, 512);
		NF_LoadTiledBg("bg/menu/title", "title", 256, 256);

		// Carga los fondos affine necesarios
		NF_LoadAffineBg("bg/menu/flag", "flag", 256, 256);
		NF_LoadAffineBg("bg/menu/waveflag", "waveflag", 512, 512);

		// Carga los Sprites del menu
		NF_LoadSpritePal("sprite/menu/menu_buttons", 0);
		NF_LoadSpriteGfx("sprite/menu/start_l", 0, 64, 32);
		NF_LoadSpriteGfx("sprite/menu/start_r", 1, 64, 32);
		NF_LoadSpriteGfx("sprite/menu/options_l", 2, 64, 32);
		NF_LoadSpriteGfx("sprite/menu/options_r", 3, 64, 32);
		NF_LoadSpriteGfx("sprite/menu/credits_l", 4, 64, 32);
		NF_LoadSpriteGfx("sprite/menu/credits_r", 5, 64, 32);


		//////////////////////////////
		//							//
		//	Creacion de graficos	//
		//							//
		//////////////////////////////

		// Crea los fondos tileados (pantalla superior)
		NF_CreateTiledBg(0, 3, "bigflag");
		NF_CreateTiledBg(0, 2, "title");

		// Crea los fondos affine (pantalla inferior)
		NF_CreateAffineBg(1, 2, "flag", 1);
		NF_CreateAffineBg(1, 3, "waveflag", 1);
		NF_AffineBgCenter(1, 3, 256, 512);

		// Habilita el efecto sombreado en la bandera del menu
		REG_BLDCNT = BLEND_FADE_BLACK | BLEND_SRC_BG3;
		REG_BLDCNT_SUB = BLEND_FADE_BLACK | BLEND_SRC_BG3;


		// Crea los botones del menu

		// Transfiere la paleta de los botones
		NF_VramSpritePal(1, 0, 0);
		// Transfiere los graficos de los botones
		for (n = 0; n < 6; n ++) {
			NF_VramSpriteGfx(1, n, n, true);
		}

		// Crea los botones en pantalla

		// Boton Start Race
		NF_CreateSprite(1, 20, 0, 0, 256, 32);
		NF_CreateSprite(1, 21, 1, 0, 256, 32);
		button_dx[0] = 16;
		button_x[0] = 256;
		button_y[0] = 32;
		// Boton Options
		NF_CreateSprite(1, 22, 2, 0, 256, 80);
		NF_CreateSprite(1, 23, 3, 0, 256, 80);
		button_dx[1] = 64;
		button_x[1] = 256;
		button_y[1] = 80;
		// Boton Credits
		NF_CreateSprite(1, 24, 4, 0, 256, 128);
		NF_CreateSprite(1, 25, 5, 0, 256, 128);
		button_dx[2] = 112;
		button_x[2] = 256;
		button_y[2] = 128;

		// Define la capa y el frame por defecto
		for (n = 0; n < 6; n ++) {
			// Capa de destino
			NF_SpriteLayer(1, (20 + n), 0);
			// Frame
			NF_SpriteFrame(1, (20 + n), 0);
		}

		// Actualiza la pantalla
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamSub);


		//////////////////////////
		//						//
		//	Control del menu	//
		//						//
		//////////////////////////
		
		// Inicializa el menu
		MENU_BIGFLAG_X = (64 << 8);
		MENU_BIGFLAG_Y = (128 << 8);
		MENU_BIGFLAG_ANGLE = 0;
		MENU_WAVEFLAG = 0;
		
		// Habilita la ejecucion del efecto "modo 7" en la bandera
		irqSet(IRQ_HBLANK, MenuFlag);
		irqEnable(IRQ_HBLANK);

		// Repite
		while(loop) {

			// Effecto "fade in"
			if (fade_in) {
				fade -= 64;
				if (fade <= 0) {
					fade = 0;
					fade_in = false;
				}
				if (fade == (8 << 8)) button_in = true;
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
				// Si es necesario, ajusta el volumen
				if ((menu_option == 2) || (menu_option == 10)) {
					// Ajusta el volumen
					BgmVolume(((4096 - fade) >> 2));
				}
			}
			// Effecto "Button In"
			if (button_in) {
				for (button = 0; button < 3; button ++) {
					n = 0;
					x = ((button_x[button] - button_dx[button]) >> 3);
					if (x < 1) x = 1;
					button_x[button] -= x;
					NF_MoveSprite(1, (20 + (button << 1)), button_x[button], button_y[button]);
					x =  button_x[button] + 64;
					if (x > 256) x = 256;
					NF_MoveSprite(1, (21 + (button << 1)), x, button_y[button]);
					if (button_x[button] <= button_dx[button]) {
						button_x[button] = button_dx[button];
						button ++;
					} else {
						n ++;
					}
					if (n == 0) button_in = false;
				}
			}

			// Lee el teclado
			ReadKeypad();

			// Si no hay ningun efecto pendiente...
			if (!fade_in && !fade_out && !button_in) {
				// Seleccion una opcion de menu segun se pulse
				if (KEYPRESS.up) {
					if (menu_option > 0) menu_option --;
					timer = 0;
				}
				if (KEYPRESS.down) {
					if (menu_option < 2) menu_option ++;
					timer = 0;
				}
				if (KEYPRESS.a) fade_out = true;
			}

			// Control de la autodemo
			if (!fade_out) {
				timer ++;
				if (timer > 1200) {
					menu_option = 10;
					fade_out = true;
				}
			}

			// Movimiento bandera titulo
			MENU_BIGFLAG_X += 64;
			if (MENU_BIGFLAG_X > (128 << 8)) MENU_BIGFLAG_X -= (64 << 8);
			MENU_BIGFLAG_Y -= 64;
			if (MENU_BIGFLAG_Y < (64 << 8)) MENU_BIGFLAG_Y += (64 << 8);
			MENU_BIGFLAG_ANGLE += 2;
			if (MENU_BIGFLAG_ANGLE > 512) MENU_BIGFLAG_ANGLE -= 512;

			// Efecto movimiento bandera (pantalla inferior)
			flag_x += 1;
			if (flag_x > 16) flag_x -=16;
			MENU_WAVEFLAG += 64;
			if (MENU_WAVEFLAG > (512 << 8)) MENU_WAVEFLAG -= (512 << 8);
			
			// Cambia los botones (Si hay una opcion valida seleccionada)
			if (menu_option < 3) {
				for (n = 0; n < 6; n ++) {
					if ((n >> 1) == menu_option) {
						NF_SpriteFrame(1, (20 + n), 1);
					} else {
						NF_SpriteFrame(1, (20 + n), 0);
					}
				}
			}

			// Actualiza el OAM de la libreria
			NF_SpriteOamSet(1);

			// Espera al sincronismo vertical
			swiWaitForVBlank();

			// Actualiza el OAM de Libnds
			oamUpdate(&oamSub);

			// Actualiza los fondos
			NF_AffineBgMove(1, 2, flag_x, 0, 0);

		}

		// Deshabilita la interrupcion
		irqDisable(IRQ_HBLANK);

		// Devuelve el estado
		return menu_option;

	}


	// Funcion MenuFlag();
	void MenuFlag(void) {

		// Deshabilita la interrupcion
		irqDisable(IRQ_HBLANK);

		// Variables
		s32 wave = 0;
		s32 fsin = 0;
		s32 angle = 0;
		s32 line = VLINE;

		// Si la linea de dibujado esta en pantalla
		if (line < 192) {

			// Calcula el effecto wave
			angle = (MENU_BIGFLAG_ANGLE + line);
			if (angle > 512) angle -= 512;
			fsin = (sinLerp((angle << 6)) >> 5);
			if (fsin >= 0) {
				wave = ((fsin * 24) >> 7);
			}
			if (fsin < 0) {
				fsin = -fsin;
				wave = ((fsin * 24) >> 7);
				wave = -wave;
			}

			///////////////////////
			// Pantalla Superior //
			///////////////////////

			// Sombreado parte superior
			if (line >= 0 && line <= 32) {
				REG_BLDY = (16 - (line >> 1));
			}
			// Sombreado parte inferior
			if (line >= 159 && line <= 191) {
				REG_BLDY = (16 - ((191 - line) >> 1));
			}

			// Posiciona el fondo
			NF_ScrollBg(0, 3, ((MENU_BIGFLAG_X >> 8) + wave), (MENU_BIGFLAG_Y >> 8));


			///////////////////////
			// Pantalla Inferior //
			///////////////////////

			// Sombreado parte superior
			if (line >= 16 && line <= 48) {
				REG_BLDY_SUB = (16 - ((line - 16) >> 1));
			}
			// Sombreado parte inferior
			if (line >= 143 && line <= 175) {
				REG_BLDY_SUB = (16 - ((175 - line) >> 1));
			}

			// Calcula el valor del ZOOM
			s32 zoom = (512 - (line << 1));

			// Aplicalo al fondo affine
			NF_AffineBgMove(1, 3, ((MENU_WAVEFLAG >> 8) + wave), ((MENU_WAVEFLAG >> 8) + wave), 0);
			NF_AffineBgTransform(1, 3, zoom, zoom, 0, 0);

		}

		// Vuelve a habilitar el HBLANK
		irqEnable(IRQ_HBLANK);

	}





#ifdef __cplusplus
}
#endif
