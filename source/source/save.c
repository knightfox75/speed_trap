#ifdef __cplusplus
extern "C" {
#endif





	// Speed Trap - Funciones de guardado
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010



	// Includes c/c++
	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>
	#include <time.h>

	// Includes propietarios NDS
	#include <nds.h>

	// Includes NightFox's Lib
	#include <nf_lib.h>

	// Includes del juego
	#include "save.h"
	#include "defines.h"



	// Funcion CheckSavegame();
	void CheckSavegame(const char* path) {

		// Verifica si es un emulador o el hardware real
		if (CheckFATWrite(path)) {
			// Operaciones en el hardware real
			sprintf(SAVEFILEPATH, path);
			IS_EMULATOR = false;
			CheckSavegameFile();
		} else {
			// Operaciones en el emulador
			IS_EMULATOR = true;
			CheckSavegameSRAM();
		}

	}


	// Funcion WriteSavegame();
	void WriteSavegame(void) {

		// Guardalos donde tocan
		if (IS_EMULATOR) {
			WriteSRAM();
		} else {
			WriteSavegameFile();
		}

	}



	// Funcion ReadSavegame();
	void ReadSavegame(void) {

		// Guardalos donde tocan
		if (IS_EMULATOR) {
			ReadSRAM();
		} else {
			ReadSavegameFile();
		}

	}




	// Funcion CheckSavegameFile();
	void CheckSavegameFile(void) {

		// Variables
		bool filefound = false;		// Existe el fichero
		u16 l = 0;					// Longitud del path al archivo
		FILE* savefile;				// Puntero de archivo
		u32 checksum = 0;			// Checksum del savedata

		// Añade la extension .SAV al archivo de guardado
		l = strlen(SAVEFILEPATH);
		SAVEFILEPATH[(l - 3)] = 's';
		SAVEFILEPATH[(l - 2)] = 'a';
		SAVEFILEPATH[(l - 1)] = 'v';

		// Verifica si existe el archivo de guardado
		savefile = fopen(SAVEFILEPATH, "rb");
		if (savefile) {
			filefound = true;
		} else {
			filefound = false;
		}
		fclose(savefile);

		// Si el archivo de guardado existe...
		if (filefound) {
			// Lee el archivo
			ReadSavegameFile();
			// Realiza el checksum de los datos cargados
			checksum = SavedataChecksum();
			// Verifica si hay discrepancias
			if (
				(checksum != CHECKSUM[0].value)
				||
				(strcmp(CHECKSUM[0].magic, "SPEEDTRAP") != 0)
				) {
				// Si las hay, inicializa los datos del juego
				InitGameData();
				// Graba el archivo
				WriteSavegameFile();
			}
		} else {	// Si no existe, crea los datos de guardado y crealo
			// Inicializa los datos del juego
			InitGameData();
			// Graba el archivo
			WriteSavegameFile();
		}

	}



	// Funcion WriteSavegameFile();
	void WriteSavegameFile(void) {

		// Variables
		FILE* savefile;				// Puntero de archivo

		// Alamacena el CHECKSUM
		CHECKSUM[0].value = SavedataChecksum();

		// Crea el Archivo
		savefile = fopen(SAVEFILEPATH, "wb");
		fwrite((void*)&RECORDS, sizeof(RECORDS), 1, savefile);
		fwrite((void*)&CARPARAM, sizeof(CARPARAM), 1, savefile);
		fwrite((void*)&OPTIONS, sizeof(OPTIONS), 1, savefile);
		fwrite((void*)&CHECKSUM, sizeof(CHECKSUM), 1, savefile);
		fclose(savefile);

	}



	// Funcion ReadSavegameFile();
	void ReadSavegameFile(void) {

		// Variables
		FILE* savefile;				// Puntero de archivo

		// Borra el contenido previo
		memset((void*)&RECORDS, 0, sizeof(RECORDS));
		memset((void*)&CARPARAM, 0, sizeof(CARPARAM));
		memset((void*)&OPTIONS, 0, sizeof(OPTIONS));
		memset((void*)&CHECKSUM, 0, sizeof(CHECKSUM));

		// Crea el Archivo
		savefile = fopen(SAVEFILEPATH, "rb");
		fread((void*)&RECORDS, sizeof(RECORDS), 1, savefile);
		fread((void*)&CARPARAM, sizeof(CARPARAM), 1, savefile);
		fread((void*)&OPTIONS, sizeof(OPTIONS), 1, savefile);
		fread((void*)&CHECKSUM, sizeof(CHECKSUM), 1, savefile);
		fclose(savefile);

	}



	// Funcion CheckSavegameSRAM();
	void CheckSavegameSRAM(void) {

		// Variables
		u32 checksum = 0;		// Checksum de los datos del juego

		// Carga los datos de la SRAM
		ReadSRAM();

		// Calcula el Checksum
		checksum = SavedataChecksum();

		// Si hay discrepancias
		if (
			(checksum != CHECKSUM[0].value)
			||
			(strcmp(CHECKSUM[0].magic, "SPEEDTRAP") != 0)
			) {
			// Inicializa los datos del juego
			InitGameData();
			// Guarda los datos
			WriteSRAM();
		}

	}



	// Funcion WriteSRAM();
	void WriteSRAM(void) {

		// Variables
		u16 adress = 0;			// Posicion en la SRAM
		u16 i = 0;				// Indice
		bool slot2 = false;		// Propietario actual del SLOT2

		// Alamacena el CHECKSUM
		CHECKSUM[0].value = SavedataChecksum();

		// Si no lo es, haz al ARM9 propietario del SLOT2 y guarda el propietario actual
		slot2 = (REG_EXMEMCNT & ARM7_OWNS_ROM) == 0;
		sysSetCartOwner(true);

		// Copia los datos del bloque RECORDS
		for (i = 0; i < sizeof(RECORDS); i ++) {
			((u8*)SRAM)[adress] = ((u8*)RECORDS)[i];
			adress ++;
		}

		// Copia los datos del bloque CARPARAM
		for (i = 0; i < sizeof(CARPARAM); i ++) {
			((u8*)SRAM)[adress] = ((u8*)CARPARAM)[i];
			adress ++;
		}

		// Copia los datos del bloque OPTIONS
		for (i = 0; i < sizeof(OPTIONS); i ++) {
			((u8*)SRAM)[adress] = ((u8*)OPTIONS)[i];
			adress ++;
		}

		// Copia los datos del bloque CHECKSUM
		for (i = 0; i < sizeof(CHECKSUM); i ++) {
			((u8*)SRAM)[adress] = ((u8*)CHECKSUM)[i];
			adress ++;
		}

		// Devuelve el propietario del SLOT2 al propietario anterior
		sysSetCartOwner(slot2);

	}



	// Funcion WriteSRAM();
	void ReadSRAM(void) {

		// Variables
		u16 adress = 0;			// Posicion en la SRAM
		u16 i = 0;				// Indice
		bool slot2 = false;		// Propietario actual del SLOT2

		// Borra el contenido previo
		memset((void*)&RECORDS, 0, sizeof(RECORDS));
		memset((void*)&CARPARAM, 0, sizeof(CARPARAM));
		memset((void*)&OPTIONS, 0, sizeof(OPTIONS));
		memset((void*)&CHECKSUM, 0, sizeof(CHECKSUM));

		// Si no lo es, haz al ARM9 propietario del SLOT2 y guarda el propietario actual
		slot2 = (REG_EXMEMCNT & ARM7_OWNS_ROM) == 0;
		sysSetCartOwner(true);


		// Copia los datos del bloque RECORDS
		for (i = 0; i < sizeof(RECORDS); i ++) {
			((u8*)RECORDS)[i] = ((u8*)SRAM)[adress];
			adress ++;
		}

		// Copia los datos del bloque CARPARAM
		for (i = 0; i < sizeof(CARPARAM); i ++) {
			((u8*)CARPARAM)[i] = ((u8*)SRAM)[adress];
			adress ++;
		}

		// Copia los datos del bloque OPTIONS
		for (i = 0; i < sizeof(OPTIONS); i ++) {
			((u8*)OPTIONS)[i] = ((u8*)SRAM)[adress];
			adress ++;
		}

		// Copia los datos del bloque CHECKSUM
		for (i = 0; i < sizeof(CHECKSUM); i ++) {
			((u8*)CHECKSUM)[i] = ((u8*)SRAM)[adress];
			adress ++;
		}

		// Devuelve el propietario del SLOT2 al propietario anterior
		sysSetCartOwner(slot2);

	}



	// Funcion InitGameData();
	void InitGameData(void) {

		// Inicializa el nivel de dificultad
		OPTIONS[0].difficulty = 1;

		// Inicializa los niveles de audio
		OPTIONS[0].bgm_volume = 1024;
		OPTIONS[0].sfx_volume = 512;
		OPTIONS[0].engine_volume = 0;
		
		// Magic String del checksum
		sprintf(CHECKSUM[0].magic, "SPEEDTRAP");
		CHECKSUM[0].magic[9] = '\0';

		// Inicializa los records
		InitRecords();

		// Inicializa los parametros por defecto de los coches
		InitCarParams();

	}



	// Funcion InitRecords();
	void InitRecords(void) {

		// Variables
		u8 r = 0;
		u8 t = 0;
		u8 d = 0;

		// Nombres predefinidos
		char name[TOTAL_RECORDS][4];
		sprintf(name[0], "FOX");
		sprintf(name[1], "KFR");
		sprintf(name[2], "TMX");
		sprintf(name[3], "FXI");
		for (t = 0; t < TOTAL_RECORDS; t ++) {
			name[t][3] = '\0';
		}

		// Segun el nivel de dificultad...
		for (d = 0; d < 3; d ++) {
			// Inicializa los records de cada circuito
			for (t = 0; t < RECORDS_SIZE; t ++) {
				// Inicializa el record BESTLAP
				sprintf(RECORDS[d][t].name[0], name[0]);
				RECORDS[d][t].name[0][3] = '\0';
				RECORDS[d][t].hundredths[0] = 0;
				RECORDS[d][t].seconds[0] = 0;
				RECORDS[d][t].minutes[0] = 1;
				// Inicializa los records del circuito
				for (r = 1; r < TOTAL_RECORDS; r ++) {
					sprintf(RECORDS[d][t].name[r], name[r]);
					RECORDS[d][t].name[r][3] = '\0';
					RECORDS[d][t].hundredths[r] = 0;
					RECORDS[d][t].seconds[r] = (r * 10);
					RECORDS[d][t].minutes[r] = 3;
				}
			}
		}


	}



	// InitCarParams();
	void InitCarParams(void) {

		// Cambia el random
		srand(time(NULL));

		// Variables
		u8 n = 0;
		s8 points = 0;
		s16 random = 0;

		for (n = 0; n < CAR_TOTALNUM; n ++) {

			// Puntos a repartir
			points = 12;

			// Valores por defecto
			CARPARAM[n].speed = CAR_MIN_SPEED;
			CARPARAM[n].acc = CAR_MIN_ACC;
			CARPARAM[n].brk = CAR_MIN_BRAKE;
			CARPARAM[n].turn = CAR_MIN_TURN;
			CARPARAM[n].inertia = CAR_MIN_INERTIA;

			// Repartelos
			while (points > 0) {

				// Calcula el aleatorio
				random = (int)(rand() % 5);

				// Segun el parametro seleccionado
				switch (random) {
					// Velocidad
					case 0:
						if (CARPARAM[n].speed < CAR_MAX_SPEED) {
							CARPARAM[n].speed += 32;
							points --;
						}
						break;
					// Aceleracion
					case 1:
						if (CARPARAM[n].acc < CAR_MAX_ACC) {
							CARPARAM[n].acc += 1;
							points --;
						}
						break;
					// Frenado
					case 2:
						if (CARPARAM[n].brk < CAR_MAX_BRAKE) {
							CARPARAM[n].brk += 2;
							points --;
						}
						break;
					// Giro
					case 3:
						if (CARPARAM[n].turn < CAR_MAX_TURN) {
							CARPARAM[n].turn += 2;
							points --;
						}
						break;
					// Inercia (Peso)
					case 4:
						if (CARPARAM[n].inertia > CAR_MAX_INERTIA) {
							CARPARAM[n].inertia -= 1;
							points --;
						}
						break;
				}
			}
		}

	}


	// Verifica si puedes escribir en FAT
	bool CheckFATWrite(const char* path) {

		// Variables
		bool filefound = false;		// Existe el fichero
		u16 l = 0;					// Longitud del path al archivo
		FILE* savefile;				// Puntero de archivo
		char testfile[256];			// Nombre de archivo

		// Almacena el path
		sprintf(testfile, path);

		// Añade la extension .CHK al archivo de verificacion
		l = strlen(testfile);
		testfile[(l - 3)] = 'c';
		testfile[(l - 2)] = 'h';
		testfile[(l - 1)] = 'k';

		// Verifica si puedes abrir en el archivo de verificacion
		savefile = fopen(testfile, "rb");
		if (savefile) {
			filefound = true;
		} else {
			filefound = false;
		}
		fclose(savefile);

		// Si no puedes, crea uno e intentalo abrir de nuevo
		if (!filefound) {
			savefile = fopen(testfile, "wb");
			fclose(savefile);
			savefile = fopen(testfile, "rb");
			if (savefile) filefound = true;
			fclose(savefile);
		}

		return filefound;

	}



	// Funcion SavedataChecksum();
	u32 SavedataChecksum(void) {

		// Variables
		u16 i = 0;
		u32 checksum = 0;

		// Copia los datos del bloque RECORDS
		for (i = 0; i < sizeof(RECORDS); i ++) {
			checksum += ((u8*)RECORDS)[i];
		}

		// Copia los datos del bloque CARPARAM
		for (i = 0; i < sizeof(CARPARAM); i ++) {
			checksum += ((u8*)CARPARAM)[i];
		}

		// Copia los datos del bloque OPTIONS
		for (i = 0; i < sizeof(OPTIONS); i ++) {
			checksum += ((u8*)OPTIONS)[i];
		}

		// Variables
		return checksum;

	}





#ifdef __cplusplus
}
#endif