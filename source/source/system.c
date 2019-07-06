#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de sistema
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010



	// Includes c/c++
	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>
	#include <math.h>
	#include <time.h>

	// Includes propietarios NDS
	#include <nds.h>

	// Includes NightFox's Lib
	#include <nf_lib.h>

	// Includes del juego
	#include "system.h"
	#include "sound.h"
	#include "defines.h"





	// Funcion BootGame();
	void BootGame(void) {

		// Pantalla de espera inicializando la EFS
		NF_Set2D(0, 0);
		NF_Set2D(1, 0);	
		consoleDemoInit();
		iprintf("\n NitroFS init. Please wait.\n\n");
		iprintf(" Iniciando NitroFS,\n por favor, espere.\n");
		swiWaitForVBlank();

		// Define el ROOT e inicializa el sistema de archivos
		NF_SetRootFolder("NITROFS");	// Define la carpeta ROOT para usar NitroFS

		// Inicializa el motor 2D
		NF_Set2D(0, 0);				// Modo 2D_0 en ambas pantallas
		NF_Set2D(1, 0);	

		// Inicializa el sonido
		InitSound();

		// Inicializa los fondos tileados
		NF_InitTiledBgBuffers();	// Inicializa los buffers para almacenar fondos
		NF_InitTiledBgSys(0);		// Inicializa los fondos Tileados para la pantalla superior
		NF_InitTiledBgSys(1);		// Iniciliaza los fondos Tileados para la pantalla inferior

		// Inicializa los Sprites
		NF_InitSpriteBuffers();		// Inicializa los buffers para almacenar sprites y paletas
		NF_InitSpriteSys(0);		// Inicializa los sprites para la pantalla superior
		NF_InitSpriteSys(1);		// Inicializa los sprites para la pantalla inferior

		// Inicializa los mapas de colision
		NF_InitCmapBuffers();

		// Inicializa el engine de texto
		NF_InitTextSys(0);
		NF_InitTextSys(1);

		// Inicializa las buffers de audio
		// NF_InitRawSoundBuffers();

		// Inicializa los buffers para guardar fondos en formato BITMAP
		NF_Init16bitsBgBuffers();

		// Inicializa el BackBuffer de 16 bits
		// Usalo solo una vez antes de habilitarlo
		NF_Init16bitsBackBuffer(0);
		NF_Init16bitsBackBuffer(1);


		////////////////////////////////////////////////
		// Inicializaciones especificas de este juego //
		////////////////////////////////////////////////

		// Inicializa el primer circuito del AutoPlay (modo demo) y creditos
		AUTOPLAY_STAGE = 1;
		CREDITS_STAGE = 1;

	}




	// Funcion ReadKeypad();
	void ReadKeypad(void) {

		// Variables
		u16 keys = 0;

		// Lee el teclado via Libnds
		scanKeys();

		// Teclas "HELD"
		keys = keysHeld();
		if (keys & KEY_UP) {KEYHELD.up = true;} else {KEYHELD.up = false;}
		if (keys & KEY_DOWN) {KEYHELD.down = true;} else {KEYHELD.down = false;}
		if (keys & KEY_LEFT) {KEYHELD.left = true;} else {KEYHELD.left = false;}
		if (keys & KEY_RIGHT) {KEYHELD.right = true;} else {KEYHELD.right = false;}
		if (keys & KEY_A) {KEYHELD.a = true;} else {KEYHELD.a = false;}
		if (keys & KEY_B) {KEYHELD.b = true;} else {KEYHELD.b = false;}
		if (keys & KEY_X) {KEYHELD.x = true;} else {KEYHELD.x = false;}
		if (keys & KEY_Y) {KEYHELD.y = true;} else {KEYHELD.y = false;}
		if (keys & KEY_L) {KEYHELD.l = true;} else {KEYHELD.l = false;}
		if (keys & KEY_R) {KEYHELD.r = true;} else {KEYHELD.r = false;}
		if (keys & KEY_START) {KEYHELD.start = true;} else {KEYHELD.start = false;}
		if (keys & KEY_SELECT) {KEYHELD.select = true;} else {KEYHELD.select = false;}

		// Teclas "PRESS"
		keys = keysDown();
		if (keys & KEY_UP) {KEYPRESS.up = true;} else {KEYPRESS.up = false;}
		if (keys & KEY_DOWN) {KEYPRESS.down = true;} else {KEYPRESS.down = false;}
		if (keys & KEY_LEFT) {KEYPRESS.left = true;} else {KEYPRESS.left = false;}
		if (keys & KEY_RIGHT) {KEYPRESS.right = true;} else {KEYPRESS.right = false;}
		if (keys & KEY_A) {KEYPRESS.a = true;} else {KEYPRESS.a = false;}
		if (keys & KEY_B) {KEYPRESS.b = true;} else {KEYPRESS.b = false;}
		if (keys & KEY_X) {KEYPRESS.x = true;} else {KEYPRESS.x = false;}
		if (keys & KEY_Y) {KEYPRESS.y = true;} else {KEYPRESS.y = false;}
		if (keys & KEY_L) {KEYPRESS.l = true;} else {KEYPRESS.l = false;}
		if (keys & KEY_R) {KEYPRESS.r = true;} else {KEYPRESS.r = false;}
		if (keys & KEY_START) {KEYPRESS.start = true;} else {KEYPRESS.start = false;}
		if (keys & KEY_SELECT) {KEYPRESS.select = true;} else {KEYPRESS.select = false;}

		// Teclas "UP"
		keys = keysUp();
		if (keys & KEY_UP) {KEYUP.up = true;} else {KEYUP.up = false;}
		if (keys & KEY_DOWN) {KEYUP.down = true;} else {KEYUP.down = false;}
		if (keys & KEY_LEFT) {KEYUP.left = true;} else {KEYUP.left = false;}
		if (keys & KEY_RIGHT) {KEYUP.right = true;} else {KEYUP.right = false;}
		if (keys & KEY_A) {KEYUP.a = true;} else {KEYUP.a = false;}
		if (keys & KEY_B) {KEYUP.b = true;} else {KEYUP.b = false;}
		if (keys & KEY_X) {KEYUP.x = true;} else {KEYUP.x = false;}
		if (keys & KEY_Y) {KEYUP.y = true;} else {KEYUP.y = false;}
		if (keys & KEY_L) {KEYUP.l = true;} else {KEYUP.l = false;}
		if (keys & KEY_R) {KEYUP.r = true;} else {KEYUP.r = false;}
		if (keys & KEY_START) {KEYUP.start = true;} else {KEYUP.start = false;}
		if (keys & KEY_SELECT) {KEYUP.select = true;} else {KEYUP.select = false;}

	}



	// Funcion ReadTouchpad();
	void ReadTouchpad(void) {

		// Variables
		u16 keys = 0;
		touchPosition touchscreen;

		// Lee el teclado via Libnds
		scanKeys();

		// Verifica el estado del touchscreen
		keys = keysHeld();
		if (keys & KEY_TOUCH) {TOUCHPAD.held = true;} else {TOUCHPAD.held = false;}

		// Lee el TOUCHPAD via Libnds
		touchRead(&touchscreen); 

		// Guarda el estado del touchscreen
		TOUCHPAD.pos_x = touchscreen.px;
		TOUCHPAD.pos_y = touchscreen.py;
		TOUCHPAD.raw_x = touchscreen.rawx;
		TOUCHPAD.raw_y = touchscreen.rawy;
		TOUCHPAD.press_a = touchscreen.z1;
		TOUCHPAD.press_b = touchscreen.z2;

	}



	// Funcion FadeIn();
	void FadeIn(u8 screen, u16 speed) {
		// Brillo Inicial
		s16 brillo = (16 << 8);
		// Mientras el brillo no llegue a 0
		while (brillo > 0) {
			brillo -= speed;
			// Si ya has subido el brillo
			if (brillo <= 0) {
				brillo = 0;
			}
			// Aplica el valor del brillo 
			setBrightness(screen, (0 - (brillo >> 8)));
			swiWaitForVBlank();
		}
	}



	// Funcion FadeOut();
	void FadeOut(u8 screen, u16 speed) {
		// Brillo Inicial
		s16 brillo = 0;
		// Mientras el brillo no llegue a 0
		while (brillo < (16 << 8)) {
			brillo += speed;
			// Si ya has subido el brillo
			if (brillo >= (16 << 8)) {
				brillo = (16 << 8);
			}
			// Aplica el valor del brillo 
			setBrightness(screen, (0 - (brillo >> 8)));
			swiWaitForVBlank();
		}
	}



	// Funcion WaitTime();
	void WaitTime(u16 wait) {
		s16 timer = 0;
		while (timer < wait) {
			timer ++;
			if (timer >= wait) {
				timer = wait;
			}
			swiWaitForVBlank();
		}
	}



	// Funcion ResetBuffers();
	void ResetAll(void) {

		// Variables locates
		u8 screen = 0;
		u8 layer = 0;

		// Resetea el modo grafico a 0 en ambas pantallas
		NF_Set2D(0, 0);
		NF_Set2D(1, 0);

		// Borra los buffers de Fondos
		NF_ResetTiledBgBuffers();

		// Borra los buffers de Sprites
		NF_ResetSpriteBuffers();

		// Reinicializa los fondos
		NF_InitTiledBgSys(0);
		NF_InitTiledBgSys(1);

		// Reinicializa los Sprites
		NF_InitSpriteSys(0);
		NF_InitSpriteSys(1);

		// Reinicia los mapas de colisiones
		NF_ResetCmapBuffers();

		// Reinicializa las capas de texto
		NF_InitTextSys(0);
		NF_InitTextSys(1);

		// Elimina los posibles efectos de Alpha Blending
		REG_BLDCNT = BLEND_NONE;
		REG_BLDCNT_SUB = BLEND_NONE;
		REG_BLDALPHA = 0x0F | (0x0F << 8);
		REG_BLDALPHA_SUB = 0x0F | (0x0F << 8);

		// Resetea la posicion de todas las capas de fondo
		for (screen = 0; screen < 2; screen ++) {
			for (layer = 0; layer < 4; layer ++) {
				NF_ScrollBg(screen, layer, 0, 0);
			}
		}

	}



	// Funcion UpdateBothScreens();
	void UpdateBothScreens(void) {

		// Actualiza los sprites en pantalla
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);

		// Espera al sincronismo vertical
		swiWaitForVBlank();

		// Actualiza el OAM
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

	}



	// Funcion GetAngle();
	s16 GetAngle(s16 x1, s16 y1, s16 x2, s16 y2) {

		// Variables locales
		s16 angle = 0;
		float fangle = 0;
		bool negative = false;

		// Calcula el angulo
		fangle = atan2((y1 - y2), (x1 - x2));
		if (fangle < 0) {
			negative = true;
			fangle = -fangle;
		}

		// Conviertelo a base 512
		angle = (int)((fangle * 512) / PI);
		if (negative) angle = -angle;

		// Devuelve el valor
		return angle;

	}





#ifdef __cplusplus
}
#endif
