/*
-------------------------------------------------

	// Speed Trap - Main();
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010

-------------------------------------------------
*/





/*
-------------------------------------------------
	Includes
-------------------------------------------------
*/

// Includes c/c++
#include <stdio.h>

// Includes propietarios NDS
#include <nds.h>

// Includes librerias propias
#include <nf_lib.h>

//Incluye las librerias del juego
#include "includes.h"
#include "debug.h"





/*
-------------------------------------------------
	Main() - Bloque general del programa
-------------------------------------------------
*/

int main(int argc, char **argv) {

	// Inicializa el hardware de la DS y la libreria
	BootGame();

	// Verifica el archivo de guardado de datos
	CheckSavegame(argv[0]);

	// Bucle infinito de ejecucion
	while (1) {

		// Pantallas de bienvenida
		SplashScreens();

		// Nuecleo de ejecucion
		Kernel();

	}

	return 0; 

}
