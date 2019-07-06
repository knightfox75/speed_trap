#ifndef __CREDITS_H__
#define __CREDITS_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de creditos
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>




	// Funcion Credits();
	void Credits(void);
	// Creditos del juego



	// Funcion LoadCreditsFiles();
	void LoadCreditsFiles(u8 stage);
	// Carga los archivos de los creditos



	// Funcion CreateCreditsStage();
	void CreateCreditsStage(u8 stage);
	// Crea el escenario de los creditos



	// Funcion EditCreditsPalette();
	void EditCreditsPalette(void);
	// Oscurece los colores originales de los decorados y sprites



	// Funcion CreditsKernel();
	void CreditsKernel(u32 height);
	// Kernel de los creditos



	// Funcion CreditsEngine();
	void CreditsEngine(void);
	// Engine de los creditos



	// Funcion LoadCreditsFile();
	extern u32 LoadCreditsFile(const char* file);





#ifdef __cplusplus
}
#endif


#endif