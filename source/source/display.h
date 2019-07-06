#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Speed Trap - Funciones de temporizadores
	// Requiere DevkitARM R30
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 5 de Marzo del 2010





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion SetTimer();
	void SetTimer(u8 id, s8 minutes, s8 seconds, s8 thousandths);
	// Cambia el valor absoluto de un temporizador



	// Funcion CopyTimer();
	void CopyTimer(u8 id_a, u8 id_b);
	// Copia el valor de un temporizador a otro



	// Funcion CountUpTimer();
	void CountUpTimer(u8 id);
	// Actualiza el temporizador (cuenta progresiva)



	// Funcion CountDownTimer();
	void CountDownTimer(u8 id);
	// Actualiza el temporizador (cuenta regresiva)



	// Funcion LapsControl();
	void LapsControl(void);
	// Control de las vueltas dadas



	// Funcion CheckPoints();
	void CheckPoints(void);
	// Control de los checkpoints



	// Funcion TimeExtended();
	void TimeExtended(void);
	// Añade tiempo al contador de cuentaregresiva



	// Funcion
	void BestLap(void);
	// Calcula la mejor vuelta



	// Funcion TimeOut();
	void TimeOut(void);
	// Controla el tiempo restante de partida



	// Funcion TimersScreen();
	void UpdateDisplay(void);
	// Actualiza los marcadores de la pantalla



	// Funcion EncueMessage();
	void EncueMessage(u8 id, u8 mode);
	// Coloca un mensage en cola
	

	// Funcion PopupMessage();
	extern u8 PopupMessage(void);
	// Ejecuta los mensages pendientes en cola



	// Funcion UpdateCountDownChrono();
	void UpdateCountDownChrono(void);
	// Actualiza el marcador para el cronometro regresivo



	// Funcion UpdateSpeedometer();
	void UpdateSpeedometer(void);
	// Actualiza el velocimetro





#ifdef __cplusplus
}
#endif


#endif