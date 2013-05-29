/*************************
 Biblioteca diferencial
 Utilizada para abstrair o controle do movimento diferencial de rodas
 de veiculos moveis terrestres que utilizam ponte H.
 
 Criada por Felipe Ferreira, 28MAI2013
 
 
**************************/
#if (ARDUINO >= 100)
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#ifndef DIFERENCIAL_H
#define DIFERENCIAL_H

#include <Motor.h>


class Diferencial{
	private:
		Diferencial(Motor d, Motor e){};
		~Diferencial(){};
	
	public:
		void direita(){};
		void esquerda(){};
		void frente(){};
		void tras(){};
		void parar(){};

};
#endif