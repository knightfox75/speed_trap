#ifndef __SAVE_H__
#define __SAVE_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de guardado
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>


	// Funcion CheckSavegame();
	void CheckSavegame(const char* path);
	// Verifica el archivo de guardado y crealo si no existe



	// Funcion WriteSavegame();
	void WriteSavegame(void);
	// Guarda los datos del juego



	// Funcion ReadSavegame();
	void ReadSavegame(void);
	// Carga los datos del juego



	// Funcion CheckSavegameFile();
	void CheckSavegameFile(void);
	// Verifica si el archivo de guardado.
	// Si existe, cargalo, si no, crea uno de nuevo.



	// Funcion WriteSavegame();
	void WriteSavegameFile(void);
	// Guarda los datos en un archivo



	// Funcion ReadSavegame();
	void ReadSavegameFile(void);
	// Carga los datos desde un archivo



	// Funcion CheckSavegameSRAM();
	void CheckSavegameSRAM(void);
	// EMULADOR - SRAM
	// Verifica si el archivo de guardado.
	// Si existe, cargalo, si no, crea uno de nuevo.



	// Funcion WriteSRAM();
	void WriteSRAM(void);
	// Guarda los datos en SRAM (Emulador)



	// Funcion WriteSRAM();
	void ReadSRAM(void);
	// Lee los datos en SRAM (Emulador)


	// Funcion InitGameData();
	void InitGameData(void);
	// Inicializa todos los datos del juego



	// Funcion InitRecords();
	void InitRecords(void);
	// Inicializa la tabla de records del juego



	// InitCarParams();
	void InitCarParams(void);
	// Inicializa los parametros de los coches



	// Funcion CheckFATWrite();
	bool CheckFATWrite(const char* path);
	// Verifica si puedes escribir en FAT



	// Funcion SavedataChecksum();
	u32 SavedataChecksum(void);
	// Verifica la integridad de los datos






#ifdef __cplusplus
}
#endif


#endif