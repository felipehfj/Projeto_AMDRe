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
		uint8_t _velocidade;

	public:
        #metodos
		Diferencial(Motor direito, Motor esquerdo){
            this->_motorDireito = direito;
            this->_motorEsquerdo = esquerdo;
		};

		~Diferencial(){
            delete(this->_motorDireito);
            delete(this->_motorEsquerdo);
		};
		void direita(){
            this->_motorDireito.frente();
            this->_motorEsquerdo.tras();
		};

		void esquerda(){
            this->_motorDireito.tras();
            this->_motorEsquerdo.frente();
		};

		void frente(){
            this->_motorDireito.frente();
            this->_motorEsquerdo.frente();
		};

		void tras(){
            this->_motorDireito.tras();
            this->_motorEsquerdo.tras();
		};

		void parar(){
            this->_motorDireito.parar();
            this->_motorEsquerdo.parar();
		};

		void alteraVelocidade(uint8_t velocidade){
            uint8_t velocidadeMotorDireito = this->_motorDireito.getVelocidade();
            uint8_t velocidadeMotorEsquerdo = this->_motorEsquerdo.getVelocidade();

            if((velocidadeMotorDireito && velocidadeMotorEsquerdo) != velocidade){
                this->_motorDireito.setVelocidade(velocidade);
                this->_motorEsquerdo.setVelocidade(velocidade);
            }
		};

};
#endif
