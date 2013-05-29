#include <LM35.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_VC0706.h>
#include <SoftwareSerial.h>   
#include <SD.h>
#include <Motor.h>
#include <Diferencial.h>

// Motor
/* == Declaracao dos motores == */
Motor motorDireito(38, 2, false, 254);
Motor motorEsquerdo(40, 3, true, 254);

Diferencial diferencial(& motorDireito, & motorEsquerdo);
//Fim motor

// Sensor LDR
int ldrInicial = 0, ldrEsquerda = 0, ldrDireita = 0, ldrParada = 0;
// Fim sensor

// tempo para ignorar a parada
long _ignoraParada = 0;
// fim tempo

// Ethernet
byte mac[] = { 
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; // Endereco fisico do robo
byte ipLocal[] = { 
  192, 168, 1, 2 }; // Endereco IP do robo
byte ipServidor[] = { 
  192, 168, 1, 3 }; // Endereco do servidor
int portaServidor = 2000; // Porta de conexao do servidor
EthernetClient cliente;
// Fim ethernet

// Arquivo
char nome[13];
// Fim arquivo

// Hora
RTC_DS1307 RTC;
//Fim hora

// Camera
Adafruit_VC0706 cam = Adafruit_VC0706(&Serial1);
// Fim Camera

// Ajuste LDR
#define AJLDRE 200
#define AJLDRD 200
#define AJLDRI 200
#define AJLDRP 200
//

//Ponto
int ponto = 0;
//Declara temperatura
LM35 temperatura(A5);
//fim temperatura
void setup(){
  Serial.begin(9600); //Inicia log via bluetooth
  //Contador
  Serial.print("Inicio em ");
  for (int i = 0; i < 60; i++){
    Serial.print(i, DEC);
    Serial.print(" ");
    delay(1000);
  }

  // Ignora parada
  _ignoraParada = millis();
  // Fim ignora

  // Ethernet shield
  pinMode(53, OUTPUT); // SS on Mega
  pinMode(10, OUTPUT);    // Necessário para o funcionamento do EthernetShield(pino SS 10) e o Leitor de cartão (pino SS 4).
  digitalWrite(10, HIGH); // Desliga o chip W5100
  // Fim Ethernet shield

  //iniciando cartao de memoria
  if (!SD.begin(4)) {
    Serial.println("Falha no leitor SD!");
  } 
  else {
    Serial.println("Leitor inicializado!");
  }
  // fim cartao

  // Camera
  if (cam.begin()) {
    Serial.println("Camera incializada!");
  } 
  else {
    Serial.println("Camera nao encontrada");
    return;
  }
  cam.setImageSize(VC0706_640x480);
  // Fim camera

  // Iniciando conexao de rede
  if(!Ethernet.begin(mac)){ // Tenta conectar via DHCP
    Serial.println("DHCP falhado. Forcando IP...");
    Ethernet.begin(mac, ipLocal); // Forcando IP
    delay(1000);
    Serial.print("IP Local:");
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
      Serial.print(Ethernet.localIP()[thisByte], DEC);
      Serial.print("."); 
    }
    Serial.println("----------------");
  } 
  else {
    Serial.print("IP Local:");
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
      Serial.print(Ethernet.localIP()[thisByte], DEC);
      Serial.print("."); 
    }
    Serial.println();
  }
  // Fim conexao rede

  // Iniciando relogio do sistema
  Wire.begin();
  RTC.begin();
  // Fim relogio
}  

void loop(){
  atualizaIP();
  atualizaLdr();
  atualizaIgnoraParada();

  if(ldrEsquerda > AJLDRE){
    ePontoParada();
    ePontoInicial();
    diferencial.direita();
  } 
  else if ( ldrDireita > AJLDRD){
    ePontoParada();
    ePontoInicial();
    diferencial.esquerda();
  } 
  else {
    ePontoParada();
    ePontoInicial();
    diferencial.afrente();
  }
}

// Funcao para conexao com o servidor
void conectaServidor(){
  Serial.println("Conectando Servidor...");
  if(cliente.connect(ipServidor, portaServidor)){ // Tenta conexao com o servidor
    Serial.println("Conectado!");
    while(cliente.connected()){
      cam.setImageSize(VC0706_640x480);
      if (! cam.takePicture()){
        Serial.println("Falha ao capturar!");
      }
      else{
        Serial.println("Imagem capturada!");
      }

      montaNome();

      File imgFile = SD.open(nome, FILE_WRITE);
      uint16_t jpglen = cam.frameLength();
      Serial.println(imgFile.name());
      Serial.print("Armazenando ");
      Serial.print(jpglen, DEC);
      Serial.print(" bytes da imagem.");

      byte wCount = 0; // For counting # of writes
      while (jpglen > 0) {
        // read 32 bytes at a time;
        uint8_t *buffer;
        uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
        buffer = cam.readPicture(bytesToRead);
        imgFile.write(buffer, bytesToRead);
        if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
          Serial.print('.');
          wCount = 0;
        }
        //Serial.print("Read ");  Serial.print(bytesToRead, DEC); Serial.println(" bytes");
        jpglen -= bytesToRead;
      }

      imgFile.close();
      cam.resumeVideo();
      Serial.println("Armazenado");

      uint16_t temp;
      File imagem = SD.open(nome); // Abre o arquivo de imagem a ser enviado
      if(imagem){
        wCount = 0; // For counting # of writes
        Serial.println("Enviando imagem");
        while(imagem.available()){
          temp = imagem.read();
          cliente.print((char)temp);    //envia para o cliente a seguinte String
          if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
            Serial.print('.');
            wCount = 0;
          }
        }
	char pt1[3] = "00";
	pt1[0] = '0' + ponto / 10;
	pt1[1] = '0' + ponto % 10;
        Serial.print("Ponto: ");
	Serial.println(pt1);
	cliente.print(imagem.name());
	cliente.print(pt1);
        cliente.print("\n\r##"); // terminador de imagem para ser retirado no servidor
        imagem.close();
        Serial.println("Imagem enviada!");
        /*char tempT[6];
        sprintf(tempT, "%.2f", (float)temperatura.getC());
        cliente.print(tempT);*/
        
        cliente.print(temperatura.getC());
        cliente.print("\n\r**"); // terminador de temperatura para ser retirado no servidor
        Serial.println("Temperatura enviada!");
        cliente.stop();
      }
      else {
        Serial.println("Erro no cartao");
      }

      cliente.stop();
      break;
    }
  } 
  else {
    Serial.println("Conexao falhada!");
  }
}

void atualizaLdr(){
  ldrParada = analogRead(A0);
  ldrDireita = analogRead(A1);
  ldrEsquerda = analogRead(A2);
  ldrInicial = analogRead(A3);
}

void ePontoParada(){
  if(ldrParada > AJLDRP  && ignoraParada()){
    //Esta no ponto Inicial
    diferencial.parar();
    ponto++;
    delay(1000);
    conectaServidor();
  }
}

void ePontoInicial(){
  if(ldrInicial > AJLDRI && ignoraParada()){
    //Esta no ponto Inicial
    diferencial.parar();
    Serial.println("Ponto inicial encontrado!");
    ponto = 0;
    delay(10000);
  }
}

boolean ignoraParada(){
  long ig = millis();
  if((_ignoraParada - ig) < 2000){
    return true;
  } 
  else {
    return false;
  }
}

void atualizaIgnoraParada(){
  _ignoraParada = millis(); 
}

void montaNome(){
  DateTime agora = RTC.now();  
  strcpy(nome, "DDHHMMSS.JPG");  
  nome[0] = '0' + agora.day() / 10;
  nome[1] = '0' + agora.day() % 10;
  nome[2] = '0' + agora.hour() / 10;
  nome[3] = '0' + agora.hour() % 10;
  nome[4] = '0' + agora.minute() / 10;
  nome[5] = '0' + agora.minute() % 10;
  nome[6] = '0' + agora.second() / 10;
  nome[7] = '0' + agora.second() % 10;

  if (! SD.exists(nome)) {
    return;
  }
}

void imprimeDataHora(){
  DateTime agora = RTC.now();

  Serial.print(agora.day(), DEC);
  Serial.print('/');
  Serial.print(agora.month(), DEC);
  Serial.print('/');
  Serial.print(agora.year(), DEC);
  Serial.print(' ');
  Serial.print(agora.hour(), DEC);
  Serial.print(':');
  Serial.print(agora.minute(), DEC);
  Serial.print(':');
  Serial.print(agora.second(), DEC);
  Serial.println();
}

void atualizaIP(){
 /*
   0: nothing happened
   1: renew failed
   2: renew success
   3: rebind fail
   4: rebind success
   */
  byte t = Ethernet.maintain();

  switch (t){
  case 0:
    break;
  case 1:
    Serial.println("Falha na renovacao do IP");
    break;
  case 2:
    break;
  case 3:
    Serial.println("Falha na reassociacao do IP");
    break;
  case 4:
    break;
  }  
}
