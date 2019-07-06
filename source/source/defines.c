#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Definiciones y variables globales
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
	#include "defines.h"





	////////////////////////////////////
	// Defines del control de entrada //
	////////////////////////////////////

	// Array para almacenar las teclas "HELD"
	keypad_info KEYHELD;

	// Array para almacenar las teclas "DOWN"
	keypad_info KEYPRESS;

	// Array para almacenar las teclas "UP"
	keypad_info KEYUP;

	// Array para almacenar la info de la pantalla tactil
	touchscreen_info TOUCHPAD;



	////////////////////////
	// Defines los coches //
	////////////////////////

	// Estructura de control de los coches
	car_info CAR[CAR_TOTALNUM];

	// Control del Take over
	takeover_info TAKEOVER[CAR_TOTALNUM];

	// Parametros de los coches para esta partida
	carparam_info CARPARAM[CAR_TOTALNUM];



	///////////////////////////
	// Defines los circuitos //
	///////////////////////////

	// Estructura de control del circuito actual
	track_info TRACK;

	u8 CAR_MAXNUM;	// Numero de coches en pista
	u8 CAR_PUPPET;	// Primer coche "marioneta"

	// Cronometros
	laptime_info LAPTIME[TOTAL_TIMERS];

	// Sistema de mensages emergentes
	msgpopup_info MSGPOPUP;



	/////////////////////////////////
	// Variables globales del menu //
	/////////////////////////////////

	s32 MENU_BIGFLAG_X;
	s32 MENU_BIGFLAG_Y;
	s32 MENU_BIGFLAG_ANGLE;
	s32 MENU_WAVEFLAG;

	u8 AUTOPLAY_STAGE;		// Control del stage del autoplay (modo demo)

	u8 CREDITS_STAGE;		// Control del stage de los creditos



	/////////////////////////////////////////////////////////////
	// Variables globales del roller menu (circuitos y coches) //
	/////////////////////////////////////////////////////////////

	rollermenu_info ROLLERMENU[10];



	////////////////////////////////////////////////
	// Variables de almacenamiento de los records //
	////////////////////////////////////////////////

	records_info RECORDS[3][RECORDS_SIZE];



	//////////////////////////////////////
	// Variables adiciones del savegame //
	//////////////////////////////////////

	char SAVEFILEPATH[256];		// Guarda el PATH del savegame
	bool IS_EMULATOR;			// Guarda si es la consola o un emulador
	options_info OPTIONS[1];	// Opciones del juego
	checksum_info CHECKSUM[1];	// Checksum del savedata



	//////////////////////////////////////
	// Variables del control de sonidos //
	//////////////////////////////////////

	soundtrack_info SOUNDTRACK;		// BGM que esta sonando
	sfx_info SFX[SFX_MAXNUM];		// Control de los SFX





#ifdef __cplusplus
}
#endif