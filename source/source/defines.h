#ifndef __DEFINES_H__
#define __DEFINES_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Definiciones y variables globales
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>





	////////////////////////
	// Defines de sistema //
	////////////////////////

	// Numero PI
	#define PI 3.141593

	// Obtiene via registro la linea actual del dibujado
	#define VLINE *(vu16*)0x04000006



	////////////////////////////////////
	// Defines del control de entrada //
	////////////////////////////////////

	// Define la estructura de lectura del KeyPad
	typedef struct {
		bool up;
		bool down;
		bool left;
		bool right;
		bool a;
		bool b;
		bool x;
		bool y;
		bool l;
		bool r;
		bool start;
		bool select;
	} keypad_info;

	// Array para almacenar las teclas "HELD"
	extern keypad_info KEYHELD;

	// Array para almacenar las teclas "DOWN"
	extern keypad_info KEYPRESS;

	// Array para almacenar las teclas "UP"
	extern keypad_info KEYUP;

	// Define la estructura de lectura del Touchscreen
	typedef struct {
		u16 pos_x;
		u16 pos_y;
		u16 raw_x;
		u16 raw_y;
		u16 press_a;
		u16 press_b;
		bool held;
	} touchscreen_info;

	// Array para almacenar la info de la pantalla tactil
	extern touchscreen_info TOUCHPAD;



	////////////////////////
	// Defines los coches //
	////////////////////////

	#define CAR_TOTALNUM 10			// Numero total de coches dados de alta	
	#define CAR_SPRITE_HEIGHT 64	// Distancia centro / altura del sprite (Cuidado, Double size habilitado)
	#define CAR_SPRITE_WIDTH 32		// Distancia centro / ancho del sprite  (Cuidado, Double size habilitado)
	#define CAR_HEIGHT 24			// Distancia centro / altura del coche
	#define CAR_WIDTH 12			// Distancia centro / ancho del coche

	#define CAR_COLISION_RADIUS 12	// Radio para calcular las colisiones

	#define CAR_MINANGLE -120		// Tope giro izquierda
	#define CAR_MAXANGLE 120		// Tope giro derecha
	#define CAR_MAXMINSPEED 512		// Tope velocidad fuera de pista

	#define CAR_SPRITE 50				// Primer Sprite a usar por los coches
	#define CAR_RAM_GFX 0				// Primer Slot de GFX en RAM

	// Parametros de comportamiento 
	#define CAR_MIN_SPEED 1536			// Velocidad minima
	#define CAR_MIN_ACC 4				// Factor de aceleracion
	#define CAR_MIN_BRAKE 8				// Factor de frenado
	#define CAR_MIN_TURN 12				// Factor de giro del coche
	#define CAR_MIN_INERTIA 8			// Factor de inercia

	#define CAR_MAX_SPEED 1792			// Velocidad maxima
	#define CAR_MAX_ACC 8				// Factor de aceleracion
	#define CAR_MAX_BRAKE 16			// Factor de frenado
	#define CAR_MAX_TURN 24				// Factor de giro del coche
	#define CAR_MAX_INERTIA 4			// Factor de inercia


	// -------------- Comun a todos ---------------------------------- //

	#define CAR_SHADOW_SPRITE_VRAM_PAL 0						// La paleta usada es la del SET 1
	#define CAR_SHADOW_SPRITE_RAM_GFX CAR_RAM_GFX

	// -------------- SET 1 de coches (AZUL) -------------------------------- //

	#define CAR_BLUE_SPRITE_RAM_PAL 0							// Paleta del SET 1 de coches (Azul)
	#define CAR_BLUE_SPRITE_VRAM_PAL 0

	#define CAR_BLUE01_SPRITE_RAM_GFX (CAR_RAM_GFX + 1)			// Gfx del coche BLUE01
	#define CAR_BLUE02_SPRITE_RAM_GFX (CAR_RAM_GFX + 2)			// Gfx del coche BLUE02

	// -------------- SET 2 de coches (VERDE) -------------------------------- //

	#define CAR_GREEN_SPRITE_RAM_PAL 1							// Paleta del SET 2 de coches (Verde)
	#define CAR_GREEN_SPRITE_VRAM_PAL 1

	#define CAR_GREEN01_SPRITE_RAM_GFX (CAR_RAM_GFX + 3)		// Gfx del coche GREEN01
	#define CAR_GREEN02_SPRITE_RAM_GFX (CAR_RAM_GFX + 4)		// Gfx del coche GREEN02

	// -------------- SET 3 de coches (ROJO) -------------------------------- //

	#define CAR_RED_SPRITE_RAM_PAL 2						// Paleta del SET 3 de coches (Rojo)
	#define CAR_RED_SPRITE_VRAM_PAL 2

	#define CAR_RED01_SPRITE_RAM_GFX (CAR_RAM_GFX + 5)		// Gfx del coche RED01
	#define CAR_RED02_SPRITE_RAM_GFX (CAR_RAM_GFX + 6)		// Gfx del coche RED0
	#define CAR_RED03_SPRITE_RAM_GFX (CAR_RAM_GFX + 7)		// Gfx del coche RED03

	// -------------- SET 4 de coches (NEGRO) -------------------------------- //

	#define CAR_BLACK_SPRITE_RAM_PAL 3						// Paleta del SET 4 de coches (Negro)
	#define CAR_BLACK_SPRITE_VRAM_PAL 3

	#define CAR_BLACK01_SPRITE_RAM_GFX (CAR_RAM_GFX + 8)	// Gfx del coche RED01
	#define CAR_BLACK02_SPRITE_RAM_GFX (CAR_RAM_GFX + 9)	// Gfx del coche RED02
	#define CAR_BLACK03_SPRITE_RAM_GFX (CAR_RAM_GFX + 10)	// Gfx del coche RED03

	// --- Ultimo Slot de GFX usado en RAM --- //
	#define CAR_LAST_SPRITE_RAM_GFX CAR_BLACK03_SPRITE_RAM_GFX
	#define CAR_LAST_SPRITE_PAL	3


	// Estructura de control de los coches
	typedef struct {
		s32 x;			// Posicion X del coche
		s32 y;			// Posicion Y del coche
		s32 fx;			// Posicion X del coche (fix float 256)
		s32 fy;			// Posicion Y del coche (fix float 256)
		s32 old_fx;		// Posicion X del coche (fix float 256)
		s32 old_fy;		// Posicion Y del coche (fix float 256)
		s32 speed;		// Aceleracion del coche (fix float 256)
		s32 sx[2];		// Posicion X del coche en pantalla
		s32 sy[2];		// Posicion Y del coche en pantalla
		s32 turn;		// Velocidad de giro
		s32 angle;		// Angulo de giro del coche
		s32 old_angle;	// Angulo de giro del coche
		s32 cx[3];		// Coordenada X del circulo de colision
		s32 cy[3];		// Coordenada Y del circulo de colision
		s32 kx[3];		// Coordenada X del extremo [0][1] Ejes  [2] Linia de vision 
		s32 ky[3];		// Coordenada Y del extremo [0][1] Ejes  [2] Linia de vision 
		s32 kt[3];		// Tile del extremo
		s32 wx[4];		// Coordenadas de las ruedas
		s32 wy[4];		// Coordenadas de las ruedas
		u16 wt[4];		// Tile bajo la rueda
		u8 sprite;		// nº de sprite asignado
		u8 rotscale;	// nº de rotscale asignado
		u8 model;		// Modelo de coche asignado
		bool shadow;	// Estado de la sombra
		bool on_screen[2];	// Esta en pantalla?
		bool AI_left;		// IA gira izquierda
		bool AI_right;		// IA gira a la derecha
		bool AI_acc;		// IA acelera
		bool AI_brake;		// IA frena
		u16 PARAM_maxspeed;	// Parametro velocidad maxima
		u16 PARAM_acc;		// Parametro de aceleracion
		u16 PARAM_inertia;	// Parametro de inercia
		u16 PARAM_brake;	// Parametro de frenada
		u16 PARAM_turn;		// Parametro de giro
		u16 PARAM_sight;	// Parametro distancia de vision/reaccion
	} car_info;
	extern car_info CAR[CAR_TOTALNUM];

	// Control del Take over
	typedef struct {
		s16 timer;			// Tiempo (en frames) del Take over
		bool AI_left;		// IA gira izquierda
		bool AI_right;		// IA gira a la derecha
		bool AI_acc;		// IA acelera
		bool AI_brake;		// IA frena
	} takeover_info;
	extern takeover_info TAKEOVER[CAR_TOTALNUM];

	// Parametros de los coches para esta partida
	typedef struct {
		u16 speed;
		u16 acc;
		u16 inertia;
		u16 brk;
		u16 turn;
	} carparam_info;
	extern carparam_info CARPARAM[CAR_TOTALNUM];



	///////////////////////////
	// Defines los circuitos //
	///////////////////////////

	#define TOTAL_TRACKS 3		// Numero de circuitos

	#define MSG_SPRITE 20										// Primer Sprite a usar por los mensages
	#define MSG_SPRITE_RAM_PAL (CAR_LAST_SPRITE_PAL + 1)		// Paleta a usar por los mensages
	#define MSG_SPRITE_VRAM_PAL (CAR_LAST_SPRITE_PAL + 1)
	#define MSG_SPRITE_RAM_GFX (CAR_LAST_SPRITE_RAM_GFX + 1)	// Primer GFX a usar por los mensages

	#define CHRONO_SPRITE 20	// Primer Sprite para el chronometro
	#define CHRONO_SPRITE_RAM_GFX (MSG_SPRITE_RAM_GFX + 3)	// Gfx donde almacenar los numeros
	#define CHRONO_SPRITE_RAM_PAL MSG_SPRITE_RAM_PAL		// Paleta a usar por los numeros
	#define CHRONO_SPRITE_VRAM_PAL MSG_SPRITE_VRAM_PAL

	#define SPEED_SPRITE 16		// Primer sprite para el velocimetro
	#define SPEED_SPRITE_RAM_GFX (CHRONO_SPRITE_RAM_GFX + 1)	// Slots donde esta almacenado el GFX del velocimetro
	#define SPEED_SPRITE_RAM_PAL CHRONO_SPRITE_RAM_PAL			// Slot donde esta la paleta en RAM
	#define SPEED_SPRITE_VRAM_PAL (MSG_SPRITE_VRAM_PAL + 1)		// Slot donde guardar la paleta en VRAM

	#define SEMAPHORE_SPRITE 10				// Sprite del semaforo
	#define SEMAPHORE_SPRITE_RAM_GFX 120	// Gfx en RAM
	#define SEMAPHORE_SPRITE_VRAM_GFX 120	// Gfx en VRAM
	#define SEMAPHORE_SPRITE_RAM_PAL 15		// PAL en RAM
	#define SEMAPHORE_SPRITE_VRAM_PAL 15	// PAL en VRAM

	#define TILE_MAGENTA 0
	#define TILE_RED 1
	#define TILE_GREEN 2
	#define TILE_BLUE 3
	#define TILE_YELLOW 4
	#define TILE_WHITE 6
	#define TILE_GRAY 8
	#define TILE_BLACK 10

	#define TRACK_LAPS 4						// Numero de vueltas a dar en el circuito
	#define TRACK_TIME TRACK_LAPS				// Timer asignado al tiempo total
	#define TRACK_COUNTDOWN	(TRACK_LAPS + 1)	// Cuenta atras
	#define TRACK_BESTLAP (TRACK_LAPS + 2)		// Mejor vuelta

	// Caracteristicas del circuito
	typedef struct {
		s32 width;				// Ancho del circuito actual
		s32 height;				// Altura del circuito actual
		s32 x[2];				// Posicion X del circuito en la pantalla
		s32 y[2];				// Posicion Y del circuito en la pantalla
		s32 start;				// Posicion Y de inicio del circuito
		s32 end;				// Posicion Y del final del circuito
		s32 goback;				// Diferencial entre el inicio y el final
		s32 checkpoint;			// Posicion del checkpoint
		u16 st_minutes;			// Tiempo inicial (minutos)
		u16 st_seconds;			// Tiempo inicial (segundos)
		u16 time_extended[3];	// Segundos añadidos al paso por checkpoint
		bool checkpoint_flag;	// Bandera para el conteo de vueltas
		u8 lap;					// Vuelta actual
		bool lap_flag;			// Bandera para el conteo de vueltas
		bool game_on;			// Estado de la partida
		bool hurry;				// Flag control mensage HURRY!!!
		u8 result;				// Resultado de la carrera (Ganador, perdedor, abortada)
	} track_info;
	extern track_info TRACK;

	extern u8 CAR_MAXNUM;	// Numero de coches en pista
	extern u8 CAR_PUPPET;	// Primer coche "marioneta"

	// Cronometros
	#define TOTAL_TIMERS (TRACK_BESTLAP + 1)
	typedef struct {
		s32 ticks;		// Base 256 // Sumando 427
		s8 hundredths;	// Centesimas	0 - 99
		s8 seconds;		// Segundos		0 - 59
		s8 minutes;		// Minutos		0 - 127
	} laptime_info;
	extern laptime_info LAPTIME[TOTAL_TIMERS];


	
	// Sistema de mensages emergentes
	#define TOTAL_POPUPS 10
	typedef struct {
		u8 id[TOTAL_POPUPS];		// Id del mensage a ejecutar
		u8 action[TOTAL_POPUPS];	// Accion a ejecutar
		s32 x;						// X 
		s32 y;						// Y
		u8 cue;						// Numero de acciones en cola
		s32 timer;					// Temporizador
		bool running;				// Action en curso
		bool next;					// Ejecuta la siguiente accion
	} msgpopup_info;
	extern msgpopup_info MSGPOPUP;



	/////////////////////////////////
	// Variables globales del menu //
	/////////////////////////////////

	extern s32 MENU_BIGFLAG_X;		// Variables efecto banderas
	extern s32 MENU_BIGFLAG_Y;
	extern s32 MENU_BIGFLAG_ANGLE;
	extern s32 MENU_WAVEFLAG;

	extern u8 AUTOPLAY_STAGE;		// Control del stage del autoplay (modo demo)
	
	extern u8 CREDITS_STAGE;		// Control del stage de los creditos



	/////////////////////////////////////////////////////////////
	// Variables globales del roller menu (circuitos y coches) //
	/////////////////////////////////////////////////////////////

	typedef struct {
		s16 x;		// Posicion X del sprite
		s16 y;		// Posicion Y del sprite
		s8 gfx;		// Gfx del sprite
	} rollermenu_info;
	extern rollermenu_info ROLLERMENU[10];



	////////////////////////////////////////////////
	// Variables de almacenamiento de los records //
	////////////////////////////////////////////////

	// Records
	#define TOTAL_RECORDS 4		// Records por pagina
	#define RECORDS_SIZE 16		// Total de paginas

	typedef struct {
		char name[TOTAL_RECORDS][4];	// Iniciales
		u8 hundredths[TOTAL_RECORDS];	// Centesimas	0 - 99
		u8 seconds[TOTAL_RECORDS];		// Segundos		0 - 59
		u8 minutes[TOTAL_RECORDS];		// Minutos		0 - 127
	} records_info;
	extern records_info RECORDS[3][RECORDS_SIZE];



	//////////////////////////////////////
	// Variables adiciones del savegame //
	//////////////////////////////////////

	extern char SAVEFILEPATH[256];		// Guarda el PATH del savegame
	extern bool IS_EMULATOR;			// Guarda si es la consola o un emulador

	// Menu de opciones
	typedef struct {
		s8 difficulty;		// Dificultad (0 - Facil, 1 - Medio, 2 - Dificil)
		s16 bgm_volume;		// Volumen BGM
		s16 sfx_volume;		// Volumen SFX
		s16 engine_volume;	// Volumen Motor
	} options_info;
	extern options_info OPTIONS[1];		// Opciones del juego

	// Checksum del savedata
	typedef struct {
		u32 value;		// Valor Checksum
		char magic[12];	// SPEEDTRAP
	} checksum_info;
	extern checksum_info CHECKSUM[1];	// Checksum del savedata



	//////////////////////////////////////
	// Variables del control de sonidos //
	//////////////////////////////////////

	// Escructura de control de la musica
	typedef struct {
		u16 track_id;	// ID de la musica que esta sonando
		s16 volume;		// Volumen de la musica
	} soundtrack_info;
	extern soundtrack_info SOUNDTRACK;		// BGM que esta sonando

	// Escructura de control de los sonidos
	#define SFX_MAXNUM 10
	typedef struct {
		u16 sfx_id;		// ID del SFX
		u16 handle;		// ID asignado al reproducir este SFX
		s16 volume;		// Volumen del SFX
		s16 freq;		// Frecuencia del SFX
	} sfx_info;
	extern sfx_info SFX[SFX_MAXNUM];		// Control de los SFX









#ifdef __cplusplus
}
#endif


#endif