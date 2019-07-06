#ifndef __CUSTOMCAR_H__
#define __CUSTOMCAR_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de parametrizacion de los coches
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion CustomCarMenu();
	void CustomCarMenu(void);
	// Menu de personalizacion del coche



	// Funcion LoadCustomCarFiles();
	void LoadCustomCarFiles(void);
	// Carga los archivos necesarios



	// Funcion CreateCustomCarMenu();
	void CreateCustomCarMenu(void);
	// Crea el menu



	// Funcion ShowCustomCarBar();
	void ShowCustomCarBar(void);
	// Muestra la barra de paramatros minimos



	// Funcion HideCustomCarBar();
	void HideCustomCarBar(void);
	// Oculta la barra de parametros minimos



	// Funcion UpdateParamBar();
	void UpdateParamBar(u8 id);
	// Actualiza la barra de minimos



	// Funcion UpdatePointsBar();
	void UpdatePointsBar(u8 value);
	// Actualiza la barra de puntos disponibles





#ifdef __cplusplus
}
#endif


#endif