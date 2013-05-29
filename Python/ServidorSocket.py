# -*- coding: utf-8 -*-                 #Comentario mágico para codificação utf

import socket                           #Importacao da biblioteca de socket
from datetime import datetime           #Importacao da biblioteca de data e hora
import MySQLdb

ENDERECO = ''                           #Endereco do servidor ''=localhost
PORTA = 2000                            #Porta do servidor
ADDR = (ENDERECO, PORTA)                #Tupla com o endereco e porta do servidor
EOF = "\n\r##"                          #Caracteres de terminação de envio de arquivo
EOF1 = "\n\r**" 
CAMINHO = "/var/www/amdre/imagensMonitoradas/"  #Caminho onde as fotos serao armazenadas
LOGT = "logTemp.txt"                    #dados da temperatura lida

socketServidor = socket.socket(socket.AF_INET, socket.SOCK_STREAM)      #Familia do protocolo de comunicação e definicao de stream
socketServidor.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)    #Possibilidade de reutilizar o mesmo endereço para outras conexões
socketServidor.bind(ADDR)

try:
        socketServidor.listen(5)                        #Servidor tratando apenas uma conexão
        print(">> Servidor ouvindo a porta %d" %PORTA)    #Log
        
        while 1:
                socketCliente, endereco = socketServidor.accept()
                inicioTransmissao = datetime.now()      #Marcação do tempo de inicio da transmissao do arquivo
                
                print(">> Conexao recebida (%s:%d)" %(endereco[0],endereco[1])) #log

                dado = ""

                while(dado[-4:] != EOF):
                        dado += socketCliente.recv(1024)
                        #print ("."),

                temp = dado[-18:]               #obtenção das informações da imagem
                dado = dado.replace(temp, "")   #retirada das informações terminais da imagem, o mesmo que as informações da imagem

                temp = temp.replace(EOF, "")    #retirada dos caracteres terminadores
                print (temp)
                
                ponto = temp[-2:]           #obtenção do ponto de parada
                extensao = temp[-6:-2]      #obtenção da extensao do arquivo
                tnome = temp[:-6]           #obtenção do nome do arquivo
           
                nome = tnome + ponto + extensao     #remontagem do nome do arquivo no formato DDHHMMSSPP.EXT, onde D=dia, H=hora, M=minuto, S=segundo, P=ponto de parada
                nomeArquivo = CAMINHO + str(nome)   #montagem do nome do arquivo: composto pelo endereço de armazenagem + nome do arquivo

                imagem = open(nomeArquivo, "wb")   #abertura do arquivo em modo de escrita de bytes
                print(">> Armazenandos %.2f KB do arquivo %s em %s" % ((len(dado)/1024.0),nome, CAMINHO))   #log
                imagem.write(dado)     #armazenagem dos dados do arquivo
                imagem.close()         #fechamento do arquivo

                dadotemperatura = ""
                while(dadotemperatura[-4:] != EOF1):
                        dadotemperatura += socketCliente.recv(1024)

                dadotemperatura = dadotemperatura.replace(EOF1, "")
                temperatura = float(dadotemperatura)
                print (">> Ponto: %s, temperatura: %.2f" %(ponto, temperatura))
                logtemperatura = open(CAMINHO+LOGT, "a+")
                logtemperatura.write("Ponto: " + ponto + " temperatura: " + str(temperatura) + "\n")
                logtemperatura.close() 

                conexao = MySQLdb.connect(host='localhost', user='root', passwd='102415', db='amdre')
                cursor = conexao.cursor()
                try:
                        cursor.execute("INSERT INTO pontomonitorado (numponto, temperatura, caminhoimagem, nomeimagem, datahora) VALUES (%s, %s, %s, %s, %s)", (str(ponto), str(temperatura), nomeArquivo, str(tnome+extensao), datetime.now()))
                        conexao.commit()
                except:
                        conexao.rollback()

                conexao.close()

                socketCliente.close()   #fechamento do socket corrente
                fimTransmissao = datetime.now()     #Marcação do tempo de finalização da transmissao do arquivo
                print(">> Recepcao terminada em %ss" % str(fimTransmissao - inicioTransmissao))     #log
                
        
except KeyboardInterrupt:
    print(">> Fechando Conexao")
    socketServidor.shutdown(socket.SHUT_RDWR)
    socketServidor.close()
    print(">> Servidor encerrado")
