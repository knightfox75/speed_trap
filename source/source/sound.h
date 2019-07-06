#ifndef __SOUND_H__
#define __SOUND_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de sonido (MAXMOD)
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion InitSound();
	void InitSound(void);
	// Inicializa la libreria de sonido MAXMOD con el banco de sonido seleccionado



	// Funcion PlayBgm();
	void PlayBgm(u32 bgm);
	// Carga un MOD en RAM y reproducelo en loop



	// Funcion StopBgm();
	void StopBgm(void);
	// Para la reproduccion de un MOD y descargalo de la memoria



	// Funcion LoadSfx();
	void LoadSfx(u32 id);
	// Carga en RAM el SFX con el ID especificado



	// Funcion UnloadSfx();
	void UnloadSfx(u32 id);
	// Descarga en RAM el SFX con el ID especificado



	// Funcion PlaySfx();
	void PlaySfx(u32 id);
	// Reproduce el SFX con el ID indicado



	// Funcion StopSfx();
	void StopSfx(u32 id);
	// Deten el SFX con el ID indicado



	// Funcion StopAllSfx();
	void StopAllSfx(void);
	// Deten todos los sonidos y reinicia el sistema de SFX de MAXMOD



	// Funcion SfxFreq();
	void SfxFreq(u32 id, u32 freq);
	// Cambia la fecuencia del sample con la ID dada



	// Funcion GetBgmId();
	extern u32 GetBgmId(s32 bgm);
	// Obtiene la ID de la musica solicitada



	// Funcion GetSfxId();
	extern u32 GetSfxId(s32 sfx);
	// Obtiene la ID de el sonido especificado



	// Funcion BgmVolume();
	void BgmVolume(u32 volume);
	// Cambia el volumen de la musica de fondo



	// Funcion SfxVolume();
	void SfxVolume(u32 id, u32 volume);
	// Cambia el volumen de los efectos de sonido



	// Funcion SoundSetup();
	void SoundSetup(void);
	// Menu para configurar los niveles de sonido



	// Funcion LoadSoundSetupFiles();
	void LoadSoundSetupFiles(void);
	// Carga los archivos para el menu



	// Funcion CreateSoundSetupMenu();
	void CreateSoundSetupMenu(void);
	// Crea el menu



	// Funcion UpdateSoundBar();
	void UpdateSoundBar(void);
	// Actualiza las barras de volumenes





#ifdef __cplusplus
}
#endif


#endif