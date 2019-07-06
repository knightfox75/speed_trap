#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de sonido (MAXMOD)
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

	// Includes de MAXMOD
	#include <maxmod9.h>

	// Include del banco de sonidos
	#include "speedtrap_sound.h"

	// Includes NightFox's Lib
	#include <nf_lib.h>

	// Includes del juego
	#include "sound.h"
	#include "system.h"
	#include "defines.h"




	// Funcion InitSound();
	void InitSound(void) {

		// Inicializa MAXMOD con en banco de sonido seleccionado
		mmInitDefault("snd/speedtrap_sound.sbf");
		swiWaitForVBlank();	

		// Modo Hardware
		mmSelectMode(MM_MODE_A);
		swiWaitForVBlank();	

	}



	// Funcion PlayBgm();
	void PlayBgm(u32 bgm) {

		// Variables
		u32 track = 0;

		// Obten el ID del Track
		track = GetBgmId(bgm);

		// Guarda el ID del track
		SOUNDTRACK.track_id = track;

		// Carga el track
		mmLoad(track);
		swiWaitForVBlank();	

		// Reproduce el mod
		mmStart(track, MM_PLAY_LOOP);
		swiWaitForVBlank();	

		// Ajusta y guarda el volumen
		BgmVolume(1024);
		SOUNDTRACK.volume = 1024;

	}



	// Funcion StopBgm();
	void StopBgm(void) {

		// Deten el BGM actual
		mmStop();
		swiWaitForVBlank();	

		// Y descargalo de la memoria
		mmUnload(SOUNDTRACK.track_id);
		swiWaitForVBlank();	
		
	}



	// Funcion LoadSfx();
	void LoadSfx(u32 id) {

		// Variables
		u32 sfx = 0;

		// Obten el ID del SFX
		sfx = GetSfxId(id);

		// Guarda el ID del SFX
		SFX[id].sfx_id = sfx;

		// Carga el SFX en RAM
		mmLoadEffect(sfx);
		swiWaitForVBlank();	

	}



	// Funcion UnloadSfx();
	void UnloadSfx(u32 id) {

		// Descarga el SFX en RAM
		mmUnloadEffect(SFX[id].sfx_id);
		swiWaitForVBlank();	

	}



	// Funcion PlaySfx();
	void PlaySfx(u32 id) {

		// Reproduce el SFX del ID indicado
		SFX[id].handle = mmEffect(SFX[id].sfx_id);

		// Al volumen indicado
		SfxVolume(id, 1024);

	}



	// Funcion StopSfx();
	void StopSfx(u32 id) {

		// Deten el SFX del ID indicado
		mmEffectCancel(SFX[id].handle);

	}



	// Funcion StopAllSfx();
	void StopAllSfx(void) {

		// Deten todos los SFX
		mmEffectCancelAll();
		swiWaitForVBlank();

	}



	// Funcion SfxFreq();
	void SfxFreq(u32 id, u32 freq) {

		// Cambia la frecuencia del sample
		mmEffectRate(SFX[id].handle, freq);

	}



	// Funcion GetBgmId();
	u32 GetBgmId(s32 bgm) {

		s32 track = 0;

		switch (bgm) {
			// Musica del Menu / GUI
			case 0:
				track = MOD_MENU;
				break;
			// Musica de los resultados / records
			case 1:
				track = MOD_RESULTS;
				break;
			// Musica de los creditos
			case 2:
				track = MOD_CREDITS;
				break;
			// Track 1 - Speed Spring
			case 11:
				track = MOD_STAGE01;
				break;
			// Track 2 - Desert Trap
			case 12:
				track = MOD_STAGE02;
				break;
			// Track 3 - The Factory
			case 13:
				track = MOD_STAGE03;
				break;
		}

		return track;

	}



	// Funcion GetSfxId();
	u32 GetSfxId(s32 sfx) {

		s32 id = 0;

		switch (sfx) {
			// Motor del coche del jugador
			case 0:
				id = SFX_ENGINE;
				break;
			// BEEP
			case 1:
				id = SFX_BEEP;
				break;
			// Tone 500hz
			case 2:
				id = SFX_TONE500;
				break;
			// Tone 1000hz
			case 3:
				id = SFX_TONE1000;
				break;
		}

		return id;

	}



	// Funcion BgmVolume();
	void BgmVolume(u32 volume) {

		// Variables
		s32 level = ((volume * OPTIONS[0].bgm_volume) >> 10);

		// Fix del volumen
		if (level < 0) level = 0;
		if (level > 1024) level = 1024;

		// Aplica el volumen
		mmSetModuleVolume(level);

		// Guarda el volumen actual
		SOUNDTRACK.volume = volume;

	}



	// Funcion SfxVolume();
	void SfxVolume(u32 id, u32 volume) {

		// Variables
		s32 level = 0;

		// Calcula el volumen maestro a usar
		if (id == 0) {
			level = (((volume * OPTIONS[0].engine_volume) >> 10) >> 2);
		} else {
			level = (((volume * OPTIONS[0].sfx_volume) >> 10) >> 2);
		}

		// Fix del volumen
		if (level < 0) level = 0;
		if (level > 255) level = 255;

		// Aplica el volumen
		mmEffectVolume(SFX[id].handle, level);

		// Guarda el volumen actual
		SFX[id].volume = volume;

	}



	// Funcion SoundSetup();
	void SoundSetup(void) {

		// Borralo todo
		ResetAll();

		// Variables
		s16 fade = (16 << 8);		// Brillo
		bool fade_in = true;		// Flag Fade in
		bool fade_out = false;		// Flag Fade Out

		s32 bg3_x = 0;				// Coordenadas del fondo mobil
		s32 bg3_y = 0;

		s8 id = 0;					// ID de la opcion seleccionada actual

		bool update = false;		// Flag de actualizacion de marcadores

		bool engine_is_playing = false;		// Esta sonando el sonido del motor?
		bool engine_is_stoped = true;		// Esta detenido el sonido del motor?

		bool exit = false;			// Flag de salida;

		bool loop = true;			// Loop del menu

		// Pantalla en negro
		setBrightness(3, -16);

		// Carga los archivos de datos
		LoadSoundSetupFiles();

		// Crea el menu de opciones
		CreateSoundSetupMenu();

		// Bucle
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
				setBrightness(3, (0 - (fade >> 8)));
			}

			// Si no hay ningun efecto pendiente
			if (!fade_in && !fade_out) {
				// Si se pulsa arriba
				if (KEYPRESS.up) {
					id --;
					if (id < 0) id = 0;
					NF_MoveSprite(1, 12, 56, (64 + (id << 5)));
				}
				// Si se pulsa abajo
				if (KEYPRESS.down) {
					id ++;
					if (id > 2) id = 2;
					NF_MoveSprite(1, 12, 56, (64 + (id << 5)));
				}
				// Si se pulsa derecha
				if (KEYPRESS.right) {
					// Segun la ID
					switch (id) {
						// Volumen de la BGM
						case 0:
							OPTIONS[0].bgm_volume += 64;
							if (OPTIONS[0].bgm_volume > 1024) OPTIONS[0].bgm_volume = 1024;
							BgmVolume(1024);
							break;
						// Volumen del motor
						case 1:
							OPTIONS[0].engine_volume += 64;
							if (OPTIONS[0].engine_volume > 1024) OPTIONS[0].engine_volume = 1024;
							SfxVolume(0, 1024);
							break;
						// Volumen del resto de SFX
						case 2:
							OPTIONS[0].sfx_volume += 64;
							if (OPTIONS[0].sfx_volume > 1024) OPTIONS[0].sfx_volume = 1024;
							PlaySfx(1);
							break;
					}
					update = true;
				}
				// Si se pulsa izquierda
				if (KEYPRESS.left) {
					// Segun la ID
					switch (id) {
						// Volumen de la BGM
						case 0:
							OPTIONS[0].bgm_volume -= 64;
							if (OPTIONS[0].bgm_volume < 0) OPTIONS[0].bgm_volume = 0;
							BgmVolume(1024);
							break;
						// Volumen del motor
						case 1:
							OPTIONS[0].engine_volume -= 64;
							if (OPTIONS[0].engine_volume < 0) OPTIONS[0].engine_volume = 0;
							SfxVolume(0, 1024);
							break;
						// Volumen del resto de SFX
						case 2:
							OPTIONS[0].sfx_volume -= 64;
							if (OPTIONS[0].sfx_volume < 0) OPTIONS[0].sfx_volume = 0;
							PlaySfx(1);
							break;
					}
					update = true;
				}
				// Si pulsas A
				if (KEYPRESS.a) {
					fade_out = true;
					exit = true;
				}
				// Si pulsas A
				if (KEYPRESS.b) {
					fade_out = true;
					exit = true;
				}
			}

			// Calcula el movimiento del fondo BG3
			bg3_x += 128;
			bg3_y += 64;
			if (bg3_x > (256 << 8)) bg3_x -= (256 << 8);
			if (bg3_y > (256 << 8)) bg3_y -= (256 << 8);

			// Actualiza las barras, si es necesario
			if (update) UpdateSoundBar();

			// Debe de sonar el sonido del motor?
			if (!exit) {
				if (id == 1) {
					// Si no esta sonando, haz que suene
					if (!engine_is_playing) {
						PlaySfx(0);
						engine_is_playing = true;
						engine_is_stoped = false;
					}
				} else {
					if (!engine_is_stoped) {
						StopSfx(0);
						engine_is_playing = false;
						engine_is_stoped = true;
					}
				}
			} else {
				// Deten el sonido del motor si esta activo
				if (engine_is_playing) {
					StopSfx(0);
					engine_is_playing = false;
				}
			}

			// Actualiza el OAM
			NF_SpriteOamSet(1);

			// Espera al sincronismo
			swiWaitForVBlank();

			// Actualiza el VRAM del OAM
			oamUpdate(&oamSub);

			// Mueve el fondo BG3
			NF_ScrollBg(0, 3, (bg3_x >> 8), (bg3_y >> 8));
			NF_ScrollBg(1, 3, (bg3_x >> 8), (bg3_y >> 8));

		}

		// Al salir, descarga los SFX de la RAM
		StopAllSfx();
		UnloadSfx(0);
		UnloadSfx(1);

	}



	// Funcion LoadSoundSetupFiles();
	void LoadSoundSetupFiles(void) {

		// Archivos de fondos
		NF_LoadTiledBg("bg/options/bg_top", "bg_top", 256, 256);		// Logo
		NF_LoadTiledBg("bg/options/bg3", "bg3", 512, 512);				// Fondo mobil
		NF_LoadTiledBg("bg/carcustom/bot_bg2", "bot_bg2", 256, 256);	// Mascara
		NF_LoadTiledBg("bg/carcustom/bot_bg1", "bot_bg1", 256, 256);	// Marco

		// Sprites
		NF_LoadSpriteGfx("sprite/carselect/status_bar", 0, 32, 16);
		NF_LoadSpritePal("sprite/carselect/status_bar", 0);
		NF_LoadSpriteGfx("sprite/carparam/select_bar", 1, 64, 32);
		NF_LoadSpritePal("sprite/carparam/carparam", 1);

		// Carga las fuentes
		NF_LoadTextFont16("font/carparam", "sound", 256, 256, 0);

		// Carga los sonidos
		LoadSfx(0);		// Sonido del motor
		LoadSfx(1);		// Sonido del BEEP

	}



	// Funcion CreateSoundSetupMenu();
	void CreateSoundSetupMenu(void) {

		// Variables
		s16 x = 0;
		s16 y = 0;
		u8 n = 0;

		// Pantalla superior
		NF_CreateTiledBg(0, 3, "bg3");
		NF_CreateTiledBg(0, 2, "bg_top");

		// Pantalla inferior
		NF_CreateTiledBg(1, 3, "bg3");
		NF_CreateTiledBg(1, 2, "bot_bg2");
		NF_CreateTiledBg(1, 1, "bot_bg1");

		// Crea la capa de texto en la pantalla inferior
		NF_CreateTextLayer16(1, 0, 0, "sound");

		// Textos
		NF_WriteText16(1, 0, 10, 2, "SOUND  SETUP");
		NF_WriteText16(1, 0, 8, 4, "BGM");
		NF_WriteText16(1, 0, 8, 6, "ENGINE");
		NF_WriteText16(1, 0, 8, 8, "SFX");
		NF_UpdateTextLayers();

		// Transfiere los sprites y las paletas
		NF_VramSpriteGfx(1, 0, 0, false);
		NF_VramSpritePal(1, 0, 0);
		NF_VramSpriteGfx(1, 1, 1, true);
		NF_VramSpritePal(1, 1, 1);

		// Coloca los sprites (Barras)
		for (y = 0; y < 3; y ++) {
			for (x = 0; x < 4; x ++) {
				NF_CreateSprite(1, n, 0, 0, ((x << 5) + 64), ((y << 5) + 80));
				NF_SpriteLayer(1, n, 1);
				NF_SpriteFrame(1, n, 0);
				n ++;
			}
		}
		UpdateSoundBar();

		// Marcador de poscion
		NF_CreateSprite(1, n, 1, 1, 56, 64);
		NF_SpriteLayer(1, n, 1);

		// Efectos alpha
		REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
		REG_BLDALPHA_SUB = 0x06 | (0x06 << 8);

	}



	// Funcion UpdateSoundBar();
	void UpdateSoundBar(void) {

		// Variables
		s16 value = 0;
		u8 id = 0;

		// Actualiza todas las barras
		for (id = 0; id < 3; id ++) {

			// Obten el valor de la barra actual
			switch (id) {
				case 0:
					value = (OPTIONS[0].bgm_volume >> 6);
					break;
				case 1:
					value = (OPTIONS[0].engine_volume >> 6);
					break;
				case 2:
					value = (OPTIONS[0].sfx_volume >> 6);
					break;
			}

			// Calcula los valores de cada tramo
			s8 part_a = value;
			if (part_a < 0) part_a = 0;
			if (part_a > 4) part_a = 4;

			s8 part_b = (value - 4);
			if (part_b < 0) part_b = 0;
			if (part_b > 4) part_b = 4;

			s8 part_c = (value - 8);
			if (part_c < 0) part_c = 0;
			if (part_c > 4) part_c = 4;

			s8 part_d = (value - 12);
			if (part_d < 0) part_d = 0;
			if (part_d > 4) part_d = 4;

			// Actualiza la barra
			NF_SpriteFrame(1, (id << 2), part_a);
			NF_SpriteFrame(1, ((id << 2) + 1), part_b);
			NF_SpriteFrame(1, ((id << 2) + 2), part_c);
			NF_SpriteFrame(1, ((id << 2) + 3), part_d);

		}

	}






#ifdef __cplusplus
}
#endif
