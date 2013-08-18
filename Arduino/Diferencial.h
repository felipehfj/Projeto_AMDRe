/*************************
 Biblioteca diferencial
 Utilizada para abstrair o controle do movimento diferencial de rodas
 de veiculos moveis terrestres que utilizam ponte H.

 Criada por Felipe Ferreira

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
        #atributos
		Motor _motordireito;
		Motor _motoresquerdo;
        #metodos
		Diferencial(Motor direito, Motor esquerdo){
            this->_motordireito = direita;
            this->_motoresquerdo = esquerdo;
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
