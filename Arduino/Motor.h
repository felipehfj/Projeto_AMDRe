/**
 Biblioteca Motor
 Controlar do movimento de um motor de CC utilizando o circuito integrado L293D ou ponte H.
 Considera-se a utilização do pino Enable ligado ao VCC, tornando a ponte constantemente habilitada.
 A velocidade e controlada pela aplicacao de pulsos PWM em um dos pinos de entradas de controle da ponte.

 @author Felipe Ferreira
 @version 1.0 18AGO2013
*/

#if (ARDUINO >= 100)
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#ifndef MOTOR_H
#define MOTOR_H

class Motor{
	private:
        #atributos
        uint8_t _pinoA;
        uint8_t _pinoB;
        bool _inverso;
        uint8_t _velocidade; // PWM
        #metodos
		Motor(uint8_t pinoA, uint8_t pinoB, bool inverso = false, uint8_t velocidade = 128){
            setPinoA(pinoA);
            setPinoB(pinoB);
            setInverso(inverso);
            setVelocidade(velocidade);
		};

		~Motor(){};

		void setPinoA(uint8_t pino){
            this->_pinoA = pino;
		};

		uint8_t getPinoA(){
            return this->_pinoA;
		};

		void setPinoB(uint8_t pino){
            this->_pinoB = pino;
		};

		uint8_t getPinoB(){
            return this->_pinoB;
		};

		void setInverso(bool inverso){
            this->_inverso = inverso;
		};

		bool isInverso(){
            return this->_inverso;
		};

	public:
        void setVelocidade(uint8_t velocidade = 128){
            this->_velocidade = velocidade;
        };

        uint8_t getVelocidade(){
            return this->_velocidade;
        };
		void frente(){
            if(isInverso()){
                digitalWrite(getPinoA(), 0);
                analogWrite(getPinoB(), getVelocidade());
            } else {
                analogWrite(getPinoA(), getVelocidade());
                digitalWrite(getPinoB(), 0);
            }
		};

		void tras(){
             if(isInverso()){
                digitalWrite(getPinoA(), 1);
                analogWrite(getPinoB(), getVelocidade());
            } else {
                analogWrite(getPinoA(), getVelocidade());
                digitalWrite(getPinoB(), 1);
            }
		};

		void parar(){
            digitalWrite(getPinoA(), 0);
            digitalWrite(getPinoB(), 0)
		};

};
#endif
