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
        #atributos
        uint8_t _pinoa;
        uint8_t _pinob;
        bool _inverso;
        uint8_t _velocidade; // PWM
        #metodos
		Motor(uint8_t pinoa, uint8_t pinob, bool inverso = false, uint8_t velocidade = 128){
            setpinoa(pinoa);
            setpinob(pinob);
            setinverso(inverso);
            setvelocidade(velocidade);
		};

		~Motor(){};

		void setpinoa(uint8_t pino){
            this->_pinoa = pino;
		};

		uint8_t getpinoa(){
            return this->_pinoa;
		};

		void setpinob(uint8_t pino){
            this->_pinob = pino;
		};

		uint8_t getpinob(){
            return this->_pinob;
		};

		void setinverso(bool inverso){
            this->_inverso = inverso;
		};

		bool isinverso(){
            return this->_inverso;
		};

	public:
        void setvelocidade(uint8_t velocidade = 128){
            this->_velocidade = velocidade;
        };

        uint8_t getvelocidade(){
            return this->_velocidade;
        };
		void frente(){
            if(isinverso()){
                digitalWrite(getpinoa(), 0);
                analogWrite(getpinob(), getvelocidade());
            } else {
                analogWrite(getpinoa(), getvelocidade());
                digitalWrite(getpinob(), 0);
            }
		};

		void tras(){
             if(isinverso()){
                digitalWrite(getpinoa(), 1);
                analogWrite(getpinob(), getvelocidade());
            } else {
                analogWrite(getpinoa(), getvelocidade());
                digitalWrite(getpinob(), 1);
            }
		};

		void parar(){
            digitalWrite(getpinoa(), 0);
            digitalWrite(getpinob(), 0)
		};

};
#endif
