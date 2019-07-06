#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de creacion de contenidos
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
	#include "create.h"
	#include "defines.h"





	// Funcion CreateStage();
	void CreateStage(u8 stage, u8 player) {

		CreateTrack(stage);			// Crea la pista
		CreateCars(stage, player);	// Crea los coches
		CreateMessageSprites();		// Sprites de los mensages
		CreateSpeedometer();		// Sprites del velocimetro

	}



	// Funcion CreateTack();
	void CreateTrack(u8 stage) {

		// Variables locales
		u8 n = 0;

		///////////////////////////////////
		// Creacion de elementos comunes //
		///////////////////////////////////

		// Fondos de los circuitos
		for (n = 0; n < 2; n ++) {
			NF_CreateTiledBg(n, 3, "bg3");
			NF_CreateTiledBg(n, 2, "bg2");
			NF_CreateTiledBg(n, 1, "bg1");
		}

		// Elementos especificos de cada circuito
		switch (stage) {

			// Spring Speed
			case 1:
				// Parametros
				TRACK.width = 512;
				TRACK.height = 16384;
				TRACK.start = 15872;
				TRACK.end = 512;
				TRACK.goback = (TRACK.start - TRACK.end);
				TRACK.checkpoint = 8192;
				TRACK.st_minutes = 0;
				TRACK.st_seconds = 59;
				TRACK.time_extended[0] = 24;	// Facil
				TRACK.time_extended[1] = 20;	// Normal
				TRACK.time_extended[2] = 16;	// Dificil
				// Effecto alpha para las sombras en el circuito
				REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG2 | (BLEND_DST_BG3 + BLEND_DST_SPRITE);
				REG_BLDALPHA = 0x06 | (0x09 << 8);
				REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | (BLEND_DST_BG3 + BLEND_DST_SPRITE);
				REG_BLDALPHA_SUB = 0x06 | (0x09 << 8);
				break;

			// Desert Trap
			case 2:
				// Parametros
				TRACK.width = 512;
				TRACK.height = 16384;
				TRACK.start = 15872;
				TRACK.end = 512;
				TRACK.goback = (TRACK.start - TRACK.end);
				TRACK.checkpoint = 8192;
				TRACK.st_minutes = 0;
				TRACK.st_seconds = 59;
				TRACK.time_extended[0] = 24;	// Facil
				TRACK.time_extended[1] = 20;	// Normal
				TRACK.time_extended[2] = 16;	// Dificil
				// Effecto alpha para las sombras en el circuito
				REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG2 | (BLEND_DST_BG3 + BLEND_DST_SPRITE);
				REG_BLDALPHA = 0x06 | (0x09 << 8);
				REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | (BLEND_DST_BG3 + BLEND_DST_SPRITE);
				REG_BLDALPHA_SUB = 0x06 | (0x09 << 8);
				break;

			// The Factory
			case 3:
				// Parametros
				TRACK.width = 512;
				TRACK.height = 16384;
				TRACK.start = 15872;
				TRACK.end = 512;
				TRACK.goback = (TRACK.start - TRACK.end);
				TRACK.checkpoint = 8192;
				TRACK.st_minutes = 0;
				TRACK.st_seconds = 59;
				TRACK.time_extended[0] = 24;	// Facil
				TRACK.time_extended[1] = 20;	// Normal
				TRACK.time_extended[2] = 16;	// Dificil
				// Effecto alpha para las sombras en el circuito
				REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG2 | (BLEND_DST_BG3 + BLEND_DST_SPRITE);
				REG_BLDALPHA = 0x02 | (0x0F << 8);
				REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | (BLEND_DST_BG3 + BLEND_DST_SPRITE);
				REG_BLDALPHA_SUB = 0x02 | (0x0F << 8);
				break;

			// Indefinido
			default:
				// Parametros
				TRACK.width = 512;
				TRACK.height = 16384;
				TRACK.start = 15872;
				TRACK.end = 512;
				TRACK.goback = (TRACK.start - TRACK.end);
				TRACK.checkpoint = 8192;
				TRACK.st_minutes = 0;
				TRACK.st_seconds = 59;
				TRACK.time_extended[0] = 24;	// Facil
				TRACK.time_extended[1] = 20;	// Normal
				TRACK.time_extended[2] = 16;	// Dificil
				// Effecto alpha para las sombras en el circuito
				REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG2 | (BLEND_DST_BG3 + BLEND_DST_SPRITE);
				REG_BLDALPHA = 0x06 | (0x09 << 8);
				REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | (BLEND_DST_BG3 + BLEND_DST_SPRITE);
				REG_BLDALPHA_SUB = 0x06 | (0x09 << 8);
				break;

		}

	}



	// Funcion CreateCars
	void CreateCars(u8 stage, u8 player) {

		// Variables
		u8 id = 0;
		u8 n = 0;

		for (n = 0; n < 2; n ++) {

			// -------------- SET 1 de coches (AZUL) + sombra ------------------------- //

			// Transfiere la paleta a la VRAM
			NF_VramSpritePal(n, CAR_BLUE_SPRITE_RAM_PAL, CAR_BLUE_SPRITE_VRAM_PAL);

			// -------------- SET 2 de coches (VERDE) -------------------------------- //

			// Transfiere la paleta a la VRAM
			NF_VramSpritePal(n, CAR_GREEN_SPRITE_RAM_PAL, CAR_GREEN_SPRITE_VRAM_PAL);

			// -------------- SET 3 de coches (ROJO) -------------------------------- //

			// Transfiere la paleta a la VRAM
			NF_VramSpritePal(n, CAR_RED_SPRITE_RAM_PAL, CAR_RED_SPRITE_VRAM_PAL);

			// -------------- SET 4 de coches (NEGRO) -------------------------------- //

			// Transfiere la paleta a la VRAM
			NF_VramSpritePal(n, CAR_BLACK_SPRITE_RAM_PAL, CAR_BLACK_SPRITE_VRAM_PAL);

		}


		// Parametros especificos de cada circuito
		switch (stage) {

			// Primer coche marioneta
			CAR_PUPPET = 1;

			// Circuito 1 (Spring Speed)
			case 1:
				// Crea al coche protagonista
				CreateCarSprite(player, 0, (224 + (CAR_HEIGHT >> 1)), (15896 + (CAR_WIDTH << 1)));
				// Segun el nivel de dificultad
				switch (OPTIONS[0].difficulty) {
					// Facil
					case 0:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 2;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (264 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						break;
					// Normal
					case 1:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 3;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (264 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						CreateCarSprite((GetRandomCar()), 2, (224 + (CAR_HEIGHT >> 1)), (16040 + (CAR_WIDTH << 1)));
						break;
					// Dificil
					case 2:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 4;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (264 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						CreateCarSprite((GetRandomCar()), 2, (224 + (CAR_HEIGHT >> 1)), (16040 + (CAR_WIDTH << 1)));
						CreateCarSprite((GetRandomCar()), 3, (264 + (CAR_HEIGHT >> 1)), (15896 + (CAR_WIDTH << 1)));
						break;
				}
				break;

			// Circuito 2 (Desert Trap)
			case 2:
				// Crea al coche protagonista
				CreateCarSprite(player, 0, (224 + (CAR_HEIGHT >> 1)), (15896 + (CAR_WIDTH << 1)));
				// Segun el nivel de dificultad
				switch (OPTIONS[0].difficulty) {
					// Facil
					case 0:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 2;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (264 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						break;
					// Normal
					case 1:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 2;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (264 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						break;
					// Dificil
					case 2:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 3;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (264 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						CreateCarSprite((GetRandomCar()), 2, (224 + (CAR_HEIGHT >> 1)), (16040 + (CAR_WIDTH << 1)));
						break;
				}
				break;

			// Circuito 3 (The Factory)
			case 3:
				// Crea al coche protagonista
				CreateCarSprite(player, 0, (228 + (CAR_HEIGHT >> 1)), (15896 + (CAR_WIDTH << 1)));
				// Segun el nivel de dificultad
				switch (OPTIONS[0].difficulty) {
					// Facil
					case 0:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 2;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (268 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						break;
					// Normal
					case 1:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 2;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (268 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						break;
					// Dificil
					case 2:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 3;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (268 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						CreateCarSprite((GetRandomCar()), 2, (228 + (CAR_HEIGHT >> 1)), (16040 + (CAR_WIDTH << 1)));
						break;
				}
				break;

			// Indefinido
			default:
				// Crea al coche protagonista
				CreateCarSprite(player, 0, (228 + (CAR_HEIGHT >> 1)), (15896 + (CAR_WIDTH << 1)));
				// Segun el nivel de dificultad
				switch (OPTIONS[0].difficulty) {
					// Facil
					case 0:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 2;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (264 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						break;
					// Normal
					case 1:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 2;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (268 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						break;
					// Dificil
					case 2:
						// Parametros de los coches en el circuito
						CAR_MAXNUM = 3;
						// Crea los dummies
						CreateCarSprite((GetRandomCar()), 1, (268 + (CAR_HEIGHT >> 1)), (15968 + (CAR_WIDTH << 1)));
						CreateCarSprite((GetRandomCar()), 2, (228 + (CAR_HEIGHT >> 1)), (16040 + (CAR_WIDTH << 1)));
						break;
				}
				break;

		}



		// --- Inicializaciones comunes a todos los coches ---
		for (n = 0; n < 2; n ++ ) {

			// Transfiere el grafico de la sombra a la VRAM
			NF_VramSpriteGfx(n, CAR_SHADOW_SPRITE_RAM_GFX, (CAR_SPRITE + CAR_MAXNUM), true);

			for (id = 0; id < CAR_MAXNUM; id ++ ) {

				// Capa donde se dibuja
				NF_SpriteLayer(n, CAR[id].sprite, 3);
				// Habilita el rotscale del coche
				NF_EnableSpriteRotScale(n, CAR[id].sprite, CAR[id].rotscale, true);
				// Define el frame por defecto
				NF_SpriteFrame(n, CAR[id].sprite, 0);

				// Creacion del Sprite de la sombra
				NF_CreateSprite(n, (CAR[id].sprite + CAR_MAXNUM), (CAR_SPRITE + CAR_MAXNUM), CAR_SHADOW_SPRITE_VRAM_PAL, -64, -64);
				// Capa donde se dibuja la sombra
				NF_SpriteLayer(n, (CAR[id].sprite + CAR_MAXNUM), 3);
				// Habilita el rotscale de la sombra
				NF_EnableSpriteRotScale(n, (CAR[id].sprite + CAR_MAXNUM), CAR[id].rotscale, true);

				// Parametros iniciales del rotscale usado
				NF_SpriteRotScale(n, CAR[id].rotscale, 0, 256, 256);

			}

		}


	}


	// Funcion CreateCarSprite();
	void CreateCarSprite(u8 car, u8 id, s32 x, s32 y) {

		// Variables locales
		u8 gfx = 0;
		u8 pal = 0;
		u8 screen = 0;

		// Parametros comunes
		CAR[id].PARAM_maxspeed = CARPARAM[car].speed;
		CAR[id].PARAM_acc = CARPARAM[car].acc;
		CAR[id].PARAM_inertia = CARPARAM[car].inertia;
		CAR[id].PARAM_brake = CARPARAM[car].brk;
		CAR[id].PARAM_turn = CARPARAM[car].turn;
		CAR[id].PARAM_sight = 160;

		// Crea el coche segun el tipo y la ID
		switch (car) {
			
			case 0:	// BLUE01
				gfx = CAR_BLUE01_SPRITE_RAM_GFX;
				pal = CAR_BLUE_SPRITE_VRAM_PAL;
				break;
			
			case 1:	// BLUE02
				gfx = CAR_BLUE02_SPRITE_RAM_GFX;
				pal = CAR_BLUE_SPRITE_VRAM_PAL;
				break;

			case 2:	// GREEN01
				gfx = CAR_GREEN01_SPRITE_RAM_GFX;
				pal = CAR_GREEN_SPRITE_VRAM_PAL;
				break;

			case 3:	// GREEN02
				gfx = CAR_GREEN02_SPRITE_RAM_GFX;
				pal = CAR_GREEN_SPRITE_VRAM_PAL;
				break;

			case 4:	// RED01
				gfx = CAR_RED01_SPRITE_RAM_GFX;
				pal = CAR_RED_SPRITE_VRAM_PAL;
				break;

			case 5:	// RED02
				gfx = CAR_RED02_SPRITE_RAM_GFX;
				pal = CAR_RED_SPRITE_VRAM_PAL;
				break;

			case 6:	// RED03
				gfx = CAR_RED03_SPRITE_RAM_GFX;
				pal = CAR_RED_SPRITE_VRAM_PAL;
				break;

			case 7:	// BLACK01
				gfx = CAR_BLACK01_SPRITE_RAM_GFX;
				pal = CAR_BLACK_SPRITE_VRAM_PAL;
				break;

			case 8:	// BLACK02
				gfx = CAR_BLACK02_SPRITE_RAM_GFX;
				pal = CAR_BLACK_SPRITE_VRAM_PAL;
				break;

			case 9:	// BLACK03
				gfx = CAR_BLACK03_SPRITE_RAM_GFX;
				pal = CAR_BLACK_SPRITE_VRAM_PAL;
				break;

		}

		// Si no es el coche del jugador, ajusta las caractaristicas
		if (id > 0) {
			// Segun el nivel de dificultad, resta velocidad
			switch (OPTIONS[0].difficulty) {
				// Facil
				case 0:
					CAR[id].PARAM_maxspeed -= 384;
					break;
				// Normal
				case 1:
					CAR[id].PARAM_maxspeed -= 256;
					break;
				// Dificil
				case 2:
					CAR[id].PARAM_maxspeed -= 128;
					break;
			}
		}


		// Parametros iniciales comunes
		CAR[id].angle = 0;
		CAR[id].fx = (x << 8);
		CAR[id].fy = (y << 8);
		CAR[id].speed = 0;
		CAR[id].turn = 0;
		CAR[id].sprite = (CAR_SPRITE + id);
		CAR[id].rotscale = id;
		CAR[id].model = car;
		CAR[id].shadow = false;
		CAR[id].on_screen[0] = false;
		CAR[id].on_screen[1] = false;

		// Creacion del Sprites del coche
		for (screen = 0; screen < 2; screen ++) {
			NF_VramSpriteGfx(screen, gfx, CAR[id].sprite, true);
			NF_CreateSprite(screen, CAR[id].sprite, CAR[id].sprite, pal, -64, -64);
		}

	}



	// Funcion DeleteCarSprite();
	void DeleteCarSprite(u8 id) {

		// Variables
		u8 screen = 0;

		for (screen = 0; screen < 2; screen ++) {
			NF_DeleteSprite(screen, CAR[id].sprite); 
			NF_FreeSpriteGfx(screen, CAR[id].sprite);
		}

	}



	// Funcion CreateMessageSprites();
	void CreateMessageSprites(void) {

		// Variables
		u8 n = 0;

		// Carga las paletas en VRAM
		NF_VramSpritePal(0, CHRONO_SPRITE_RAM_PAL, CHRONO_SPRITE_VRAM_PAL);
		NF_VramSpritePal(1, MSG_SPRITE_RAM_PAL, MSG_SPRITE_VRAM_PAL);

		// Transfiere los graficos para el contador de tiempo (pantalla superior)
		// 0 - Minutos
		// 1 & 2 - Segundos
		// 3 & 4 - Centesimas
		// 5 - Dos puntos
		for (n = 0; n < 6; n ++) {
			NF_VramSpriteGfx(0, CHRONO_SPRITE_RAM_GFX , (CHRONO_SPRITE + n), true);
		}

		// Crea el marcador
		NF_CreateSprite(0, CHRONO_SPRITE, CHRONO_SPRITE, CHRONO_SPRITE_VRAM_PAL, 72, 16);	// Minutos
		NF_CreateSprite(0, (CHRONO_SPRITE + 1), (CHRONO_SPRITE + 5), CHRONO_SPRITE_VRAM_PAL, 88, 16);	// Dos puntos
		NF_SpriteFrame(0, (CHRONO_SPRITE + 1), 10);
		NF_CreateSprite(0, (CHRONO_SPRITE + 2), (CHRONO_SPRITE + 1), CHRONO_SPRITE_VRAM_PAL, 104, 16);	// Segundos
		NF_CreateSprite(0, (CHRONO_SPRITE + 3), (CHRONO_SPRITE + 2), CHRONO_SPRITE_VRAM_PAL, 120, 16);	// Segundos
		NF_CreateSprite(0, (CHRONO_SPRITE + 4), (CHRONO_SPRITE + 5), CHRONO_SPRITE_VRAM_PAL, 136, 16);	// Dos puntos
		NF_SpriteFrame(0, (CHRONO_SPRITE + 4), 10);
		NF_CreateSprite(0, (CHRONO_SPRITE + 5), (CHRONO_SPRITE + 3), CHRONO_SPRITE_VRAM_PAL, 152, 16);	// Segundos
		NF_CreateSprite(0, (CHRONO_SPRITE + 6), (CHRONO_SPRITE + 4), CHRONO_SPRITE_VRAM_PAL, 168, 16);	// Segundos


		// Crea los sprites en la parte oculta de la pantalla (mensages pantalla inferior)
		for (n = 0; n < 3; n ++) {
			NF_VramSpriteGfx(1, (MSG_SPRITE_RAM_GFX + n) , (MSG_SPRITE + n), true);
			NF_CreateSprite(1, (MSG_SPRITE + n), (MSG_SPRITE + n), MSG_SPRITE_VRAM_PAL, -64, -64);
			NF_SpriteFrame(1, (MSG_SPRITE + n), 0);
			NF_SpriteLayer(1, (MSG_SPRITE + n), 0);
		}

	}



	// Funcion CreateSpeedometer();
	void CreateSpeedometer(void) {

		// Variables
		u8 n = 0;

		NF_VramSpritePal(1, SPEED_SPRITE_RAM_PAL, SPEED_SPRITE_VRAM_PAL);

		// Crea los digitos del velocimetro
		for (n = 0; n < 3; n ++) {
			NF_VramSpriteGfx(1, CHRONO_SPRITE_RAM_GFX, (SPEED_SPRITE + n), true);
			NF_CreateSprite(1, (SPEED_SPRITE + n), (SPEED_SPRITE + n), SPEED_SPRITE_VRAM_PAL, (200 + (n << 4)), 168);
			NF_SpriteFrame(1, (SPEED_SPRITE + n), 0);
			NF_SpriteLayer(1, (SPEED_SPRITE + n), 0);
		}

		// Crea el texto del Velocimetro
		NF_VramSpriteGfx(1, SPEED_SPRITE_RAM_GFX, (SPEED_SPRITE + 3), true);
		NF_CreateSprite(1, (SPEED_SPRITE + 3), (SPEED_SPRITE + 3), SPEED_SPRITE_VRAM_PAL, 216, 152);
		NF_SpriteFrame(1, (SPEED_SPRITE + 3), 0);
		NF_SpriteLayer(1, (SPEED_SPRITE + 3), 0);

	}



	// Funcion CreateSemaphore();
	void CreateSemaphore(void) {

		// Transfiere los graficos a la VRAM
		NF_VramSpriteGfx(1, SEMAPHORE_SPRITE_RAM_GFX, SEMAPHORE_SPRITE_VRAM_GFX, true);
		NF_VramSpriteGfx(1, (SEMAPHORE_SPRITE_RAM_GFX + 1), (SEMAPHORE_SPRITE_VRAM_GFX + 1), true);

		// Transfiere la paleta a la VRAM
		NF_VramSpritePal(1, SEMAPHORE_SPRITE_RAM_PAL, SEMAPHORE_SPRITE_VRAM_PAL);

		// Crea los Sprites fuera de la pantalla
		NF_CreateSprite(1, SEMAPHORE_SPRITE, SEMAPHORE_SPRITE_VRAM_GFX, SEMAPHORE_SPRITE_VRAM_PAL, 64, -64);
		NF_CreateSprite(1, (SEMAPHORE_SPRITE + 1), (SEMAPHORE_SPRITE_VRAM_GFX + 1), SEMAPHORE_SPRITE_VRAM_PAL, 128, -64);

		// Posicionalos en la capa y frame correspondiente.
		NF_SpriteLayer(1, SEMAPHORE_SPRITE, 0);
		NF_SpriteLayer(1, (SEMAPHORE_SPRITE + 1), 0);
		NF_SpriteFrame(1, SEMAPHORE_SPRITE, 0);
		NF_SpriteFrame(1, (SEMAPHORE_SPRITE + 1), 0);

	}



	// Funcion DeleteSemaphore();
	void DeleteSemaphore(void) {

		// Borra los Sprites
		NF_DeleteSprite(1, SEMAPHORE_SPRITE); 
		NF_DeleteSprite(1, (SEMAPHORE_SPRITE + 1)); 

		// Borra los graficos de la VRAM
		NF_FreeSpriteGfx(1, SEMAPHORE_SPRITE_VRAM_GFX);
		NF_FreeSpriteGfx(1, (SEMAPHORE_SPRITE_VRAM_GFX + 1));

	}



	// Funcion GetRandomCar();
	u8 GetRandomCar(void) {

		// Variables
		u8 car = 0;
		bool seek_car = true;

		// Busca un nuevo modelo de coche
		while (seek_car) {
			car = (int)(rand() % CAR_TOTALNUM);
			// Si es diferente del coche protagonista y del ultimo coche usado...
			if ((car != CAR[0].model)) seek_car = false;
		}

		// Devuelve el resultado
		return car;

	}





#ifdef __cplusplus
}
#endif