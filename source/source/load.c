#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de carga de archivos
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
	#include "load.h"
	#include "sound.h"
	#include "defines.h"





	// Funcion LoadStageFiles();
	void LoadStageFiles(u8 stage) {

		LoadTrackFiles(stage);
		LoadCarSprites();
		LoadCommonSprites();
		LoadSfxFiles();

	}


	
	// Funcion LoadTrackFiles();
	void LoadTrackFiles(u8 stage) {

		// Carga los archivos segun el Stage
		switch (stage) {
			
			// Stage 1
			case 1:
				// Fondos para la pantalla inferior
				NF_LoadTiledBg("bg/stage01/st01bg3", "bg3", 512, 16384);
				NF_LoadTiledBg("bg/stage01/st01bg2", "bg2", 512, 16384);
				NF_LoadTiledBg("bg/stage01/st01bg1", "bg1", 512, 16384);
				// Mapas de colisiones del circuito
				NF_LoadColisionBg("cmap/cmst01", 0, 512, 16384);
				break;

			case 2:
				// Fondos para la pantalla inferior
				NF_LoadTiledBg("bg/stage02/st02bg3", "bg3", 512, 16384);
				NF_LoadTiledBg("bg/stage02/st02bg2", "bg2", 512, 16384);
				NF_LoadTiledBg("bg/stage02/st02bg1", "bg1", 512, 16384);
				// Mapas de colisiones del circuito
				NF_LoadColisionBg("cmap/cmst02", 0, 512, 16384);
				break;

			case 3:
				// Fondos para la pantalla inferior
				NF_LoadTiledBg("bg/stage03/st03bg3", "bg3", 512, 16384);
				NF_LoadTiledBg("bg/stage03/st03bg2", "bg2", 512, 16384);
				NF_LoadTiledBg("bg/stage03/st03bg1", "bg1", 512, 16384);
				// Mapas de colisiones del circuito
				NF_LoadColisionBg("cmap/cmst03", 0, 512, 16384);
				break;

		}

	}



	// Funcion LoadCarSprites();
	void LoadCarSprites(void) {

		// -------------- SET 1 de coches (AZUL) + Sombra ----------------- //

		// Carga la paleta para el SET de coches
		NF_LoadSpritePal("sprite/cars/blue", CAR_BLUE_SPRITE_RAM_PAL);

		// Sombra del coche
		NF_LoadSpriteGfx("sprite/cars/car_shadow", CAR_SHADOW_SPRITE_RAM_GFX, 32, 64);

		// Carga el coche BLUE01
		NF_LoadSpriteGfx("sprite/cars/car_blue_01", CAR_BLUE01_SPRITE_RAM_GFX, 32, 64);
		// Carga el coche BLUE02
		NF_LoadSpriteGfx("sprite/cars/car_blue_02", CAR_BLUE02_SPRITE_RAM_GFX, 32, 64);

		// -------------- SET 2 de coches (VERDE) -------------------------------- //

		// Carga la paleta para el SET de coches
		NF_LoadSpritePal("sprite/cars/green", CAR_GREEN_SPRITE_RAM_PAL);

		// Carga el coche GREEN01
		NF_LoadSpriteGfx("sprite/cars/car_green_01", CAR_GREEN01_SPRITE_RAM_GFX, 32, 64);
		// Carga el coche GREEN02
		NF_LoadSpriteGfx("sprite/cars/car_green_02", CAR_GREEN02_SPRITE_RAM_GFX, 32, 64);

		// -------------- SET 3 de coches (ROJO) -------------------------------- //

		// Carga la paleta para el SET de coches
		NF_LoadSpritePal("sprite/cars/red", CAR_RED_SPRITE_RAM_PAL);

		// Carga el coche RED01
		NF_LoadSpriteGfx("sprite/cars/car_red_01", CAR_RED01_SPRITE_RAM_GFX, 32, 64);
		// Carga el coche RED02
		NF_LoadSpriteGfx("sprite/cars/car_red_02", CAR_RED02_SPRITE_RAM_GFX, 32, 64);
		// Carga el coche RED03
		NF_LoadSpriteGfx("sprite/cars/car_red_03", CAR_RED03_SPRITE_RAM_GFX, 32, 64);

		// -------------- SET 4 de coches (NEGRO) -------------------------------- //

		// Carga la paleta para el SET de coches
		NF_LoadSpritePal("sprite/cars/black", CAR_BLACK_SPRITE_RAM_PAL);

		// Carga el coche BLACK01
		NF_LoadSpriteGfx("sprite/cars/car_black_01", CAR_BLACK01_SPRITE_RAM_GFX, 32, 64);
		// Carga el coche BLACK02
		NF_LoadSpriteGfx("sprite/cars/car_black_02", CAR_BLACK02_SPRITE_RAM_GFX, 32, 64);
		// Carga el coche BLACK03
		NF_LoadSpriteGfx("sprite/cars/car_black_03", CAR_BLACK03_SPRITE_RAM_GFX, 32, 64);


	}



	// Funcion LoadCommonSprites();
	void LoadCommonSprites(void) {

		// --- Sprites de los mensages --- //

		// Paleta
		NF_LoadSpritePal("sprite/messages/messages", MSG_SPRITE_RAM_PAL);
		// Sprites
		NF_LoadSpriteGfx("sprite/messages/msg_left", MSG_SPRITE_RAM_GFX, 64, 32);
		NF_LoadSpriteGfx("sprite/messages/msg_mid", (MSG_SPRITE_RAM_GFX + 1), 64, 32);
		NF_LoadSpriteGfx("sprite/messages/msg_right", (MSG_SPRITE_RAM_GFX + 2), 64, 32);
		NF_LoadSpriteGfx("sprite/messages/numbers", CHRONO_SPRITE_RAM_GFX, 16, 16);
		NF_LoadSpriteGfx("sprite/messages/speedometer", SPEED_SPRITE_RAM_GFX, 32, 16);


		// --- Sprites del semaforo --- //

		// Paleta
		NF_LoadSpritePal("sprite/messages/semaphore", SEMAPHORE_SPRITE_RAM_PAL);
		// Sprites
		NF_LoadSpriteGfx("sprite/messages/smph_left", SEMAPHORE_SPRITE_RAM_GFX, 64, 32);
		NF_LoadSpriteGfx("sprite/messages/smph_right", (SEMAPHORE_SPRITE_RAM_GFX + 1), 64, 32);

	}



	// Funcion LoadSfxFiles();
	void LoadSfxFiles(void) {
		// Carga los sonidos
		if (OPTIONS[0].engine_volume > 0) {
			LoadSfx(0);		// Sonido del motor
		}
		if (OPTIONS[0].sfx_volume > 0) {
			LoadSfx(1);		// Sonido del BEEP
			LoadSfx(2);		// Sonido TONE500
			LoadSfx(3);		// Sonido TONE1000
		}
	}



	// Funcion UnloadSfxFiles();
	void UnloadSfxFiles(void) {
		// Carga los sonidos
		if (OPTIONS[0].engine_volume > 0) {
			UnloadSfx(0);		// Sonido del motor
		}
		if (OPTIONS[0].sfx_volume > 0) {
			UnloadSfx(1);		// Sonido del BEEP
			UnloadSfx(2);		// Sonido TONE500
			UnloadSfx(3);		// Sonido TONE1000
		}	
	}





#ifdef __cplusplus
}
#endif