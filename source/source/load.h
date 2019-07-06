#ifndef __LOAD_H__
#define __LOAD_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de carga de archivos
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion LoadStageFiles();
	void LoadStageFiles(u8 stage);
	// Carga los archivos necesarios para cada pantalla


	
	// Funcion LoadTrackFiles();
	void LoadTrackFiles(u8 stage);
	// Carga los archivos de cada Track



	// Funcion LoadCarSprites();
	void LoadCarSprites(void);
	// Carga los archivos de los Sprites de los coches



	// Funcion LoadCommonSprites();
	void LoadCommonSprites(void);
	// Carga Sprites de uso general en el circuito



	// Funcion LoadSfxFiles();
	void LoadSfxFiles(void);
	// Carga los archivos SFX



	// Funcion UnloadSfxFiles();
	void UnloadSfxFiles(void);
	// Borra de la RAM los archivos SFX





#ifdef __cplusplus
}
#endif


#endif