/**
 Biblioteca diferencial
 Utilizada para abstrair o controle do movimento diferencial de rodas
 de veiculos moveis terrestres que utilizam ponte H.

 @author Felipe Ferreira
 @version 1.0 18AGO2013
*/

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
        #atributos
		Motor _motorDireito;
		Motor _motorEsquerdo;
        #metodos
		Diferencial(Motor direito, Motor esquerdo){
            this->_motorDireito = direita;
            this->_motorEsquerdo = esquerdo;
		};
		~Diferencial(){};

	public:
		void direita(){};
		void esquerda(){};
		void frente(){};
		void tras(){};
		void parar(){};

};
#endif
