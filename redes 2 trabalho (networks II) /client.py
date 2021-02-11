#!/usr/bin/python3
#|**********************************************************************;
#* Project           : Tweet Mining via UDP stream
#*
#* Program name      : server.py
#*
#* Author            : Arianne de Paula Bortolan GRR20140220
#                    : Dante da Silva Aleo GRR20171593
#*
#* Date created      : 24/05/2019
#*
#* Purpose           : Stream tweets do clients who can do data mining on it
#*
#* Last Edit         : 24/05/2019
#*
#|**********************************************************************;
import socket
import sys
import datetime

#Variables of control of package flow for log
package_index = 0 #index do pacote
package_lost = 0  #variavel de contagem de pacotes perdidos
package_received = 0 #variavel de contagem de pacotes recebidos
package_late = 0 #variavel de contagem de pacotes atrasados
packages_on_time = 1 #variavel de contagem de pacotes que chegaram no tempo certo

if __name__ == '__main__':

    try:
        HOST = str(sys.argv[1])
        PORT = int(sys.argv[2])
        NUMT = int(sys.argv[3])
    except:
        print("To run client: python3 client.py host-ip port number-of-tweets")
        sys.exit(1)

    try:
        #pega o nome da máquina do cliente via linha de comando
        data = str(input("Insert a Name for your machine: \n"))
    except ValueError:
        print("Error while reading input's")

    #pega o tempo do incio da execução da stream
    atual = datetime.datetime.now()

    # SOCK_DGRAM is the socket type to use for UDP sockets
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # As you can see, there is no connect() call; UDP has no connections.
    # Instead, data is directly sent to the recipient via sendto().
    sock.sendto(bytes(data + "\n", "utf-8"), (HOST, PORT))

    #cria o arquivo que iria armazenar os tweets recebidos por um determinado cliente
    file = open(str(data + '_tweets'),'w+')
    file.writelines("%s_received: \n" % data)

    #cria o arquivo de log que irá conter as estatísticas da stream
    log = open(str(data + '_log'),'a')
    i = 1

    #fica no while até NUMT, onde NUMT é o numero de tweets que o cliente deseja receber
    while (i <= NUMT):
        #received recebe a string de um tweet
        received = str(sock.recv(1024), "utf-8")

        print("Received: {}".format(received))

        i = i +1
        aux = j = aux = 1
        number = k = l =  0
        d = []
        a = [10]
        d = received
        #fica no while até pegar o index do pacote e converte a string para int
        while ((d[k]) != '〄'):
            num = d[k]
            num = int (num,10)
            a.insert(j, num)
            # print(num)
            k = k+1
        a.remove(10)


        # multiplica cada casa do vetor pela potencia de 10 correspondente e soma tudo no final
        for x in a:
            a[l] = a[l]*aux
            aux = aux*10
            l = l +1

        number = l = 0
        for x in a:     
            number = number + a[l]
            l = l+ 1


        #Checks for log
        if (package_index  == 0):
            package_index = number
        else:
        # caso não seja o primeiro pacote:
            # se o ID do tweet for maior do que o index+1, quer dizer que algum pacote se perdeu/ainda não chegou
            if (number > (package_index)):
                package_lost = package_lost + 1
            else:
                if (number == package_index):
                    #pacote chegou na hora/ordem certa
                    packages_on_time  = packages_on_time + 1
                # se o ID do tweet for menor do que o index quer dizer que esse pacote chegou atrasado
                if (number < package_index):
                    if(package_lost != 0):
                        package_lost = package_lost - 1
                    package_late = package_late + 1
        # o index é a atualizado com o ID do proximo pacote que deve vir na sequencia
        package_index = package_index + 1
        package_received = package_received + 1
        atual_tt = datetime.datetime.now()

        file.writelines("%s, recebido as %s \n" %(received, str(atual_tt)) )
    print("Encerrando stream...")
    #cuida do log
    log.write("Estatísticas Máquina %s : " % data)
    log.write("Inicio da execução = %s , " % str(atual))
    log.write("Total Packages Received = %d, " % package_received)
    log.write("Total Packages Late = %d,  " % package_late)
    log.write("Total Packages Lost = %d,  " % package_lost)
    log.write("Total Packages Received on time = %d, " % packages_on_time)

    atual = datetime.datetime.now()
    log.write("Fim da execução = %s  \n" % str(atual))
