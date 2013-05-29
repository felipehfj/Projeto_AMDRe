/*************************
 Biblioteca Motor
 Utilizada para abstrair o controle do movimento de
 um motor de veiculo movel terrestre que utilizam ponte H.
 
 Criada por Felipe Ferreira, 28MAI2013
 
 
**************************/
#if (ARDUINO >= 100)
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#ifndef MOTOR_H
#define MOTOR_H

class Motor{
	private:
		Motor(uint8_t a, uint8_t b, bool inverso, uint8_t velocidade){};
		~Motor(){};
	
	public:
		void frente(){};
		void tras(){};
		void parar(){};

};
#endif