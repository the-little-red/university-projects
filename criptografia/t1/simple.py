# -*- coding: utf-8 -*-
from copy import deepcopy
import os
import itertools
import numpy as np
import sys
from sys import platform
from collections import OrderedDict
import string
import re
import unicodedata
import itertools
import multiprocessing
from math import log
import difflib


###CIPHER MATRIX CREATION###
def create_matrix(keyword):

    # Define base alphabet matrix
    alphamat = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                'U', 'V', 'W', 'X', 'Y', 'Z', 'Á', 'É', 'Í', 'Ó',
                'Ú', 'Ã', 'Õ', 'Â', 'Ê', 'Î', 'Ô', 'Û', '0', '1',
                '2', '3', '4', '5', '6', '7', '8', '9', 'Ç', '!',
                '@', '#', '$', '%', '"', '&', '*', '(', ')', '-',
                ',', '.', '<', '>', '/', '\\', 'ç', ':', '{', '}',
                '\'', ';', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                'i', 'j', 'k', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']

    alphamat.sort()
    # Prepare cipher matrix with initial values
    cipmat = deepcopy(alphamat)

    # Remove repeated characters from keyword
    unqkey = ''.join(OrderedDict.fromkeys(keyword))


    # Create list from key
    key = list(unqkey)

    # Assign unique letters of key to cipher matrix
    for x in range(0, len(unqkey)):
        cipmat[x] = unqkey[x]

    # Create list of remaining letters not in the key
    rmdr = list(set(alphamat) - set(key))

    # Order remaining letters alphabetically
    rmdr.sort()

    # Assign remaining letters to cipher matrix
    for x in range(len(unqkey), len(rmdr)):
        cipmat[x] = rmdr[x - len(unqkey)]
    return cipmat


###ENCRYPTION PLAYFAIR###
def encP(ciptxt, key):

    ciptxt = list(ciptxt)
    # Create new cipher table using given key
    cipmat = create_matrix(key)

    wid = 10  # key table matrix width
    buf = 'X'  # buffer character for ciphertext preparation

    # Insert buffer between repeated letters
    for x in range(1, len(ciptxt)):
        if ciptxt[x] == ciptxt[x - 1]:
            ciptxt.insert(x, buf)

    # ensure that blocks of 2 can be created
    if (len(ciptxt) % 2) == 1:
        ciptxt.append(buf)

    # split up the ciphertext into blocks of 2
    tmp = []
    for x in range(0, len(ciptxt) // 2):
        tmp.append(ciptxt[(x * 2)] + ciptxt[(x * 2) + 1])
    ciptxt = tmp

    # transform the text based on cipher matrix
    for x in range(0, len(ciptxt)):

        block = ciptxt[x]  # select block

        # seperate values of chosen block
        a = block[0]
        b = block[1]

        # determine position of each value in cipher matrix
        a_x = cipmat.index(a) % wid
        a_y = cipmat.index(a) // wid
        b_x = cipmat.index(b) % wid
        b_y = cipmat.index(b) // wid

        # If each value is on the same row, assign to the value to the right
        if(a_y == b_y):
            if(a_x > 8):
                a_x = 0
            else:
                a_x += 1

            if(b_x > 8):
                b_x = 0
            else:
                b_x += 1
        # If both values are in the same column, assign to the value below
        elif(a_x == b_x):
            if(a_y > 8):
                a_y = 0
            else:
                a_y += 1

            if(b_y > 8):
                b_y = 0
            else:
                b_y += 1
        # If both x and y values are different, swap x values
        else:
            a_x, b_x = b_x, a_x

        # Replace existing block with newly ciphered one
        a = cipmat[(a_y * wid) + a_x]
        b = cipmat[(b_y * wid) + b_x]
        ciptxt[x] = a + b

    return "".join(ciptxt)


###DECRYPTION PLAYFAIR###
def decP(plntxt, key):

    # Create new cipher table using given key
    cipmat = create_matrix(key)

    wid = 10  # key table matrix width
    buf = 'X'  # buffer character for ciphertext preparation

    # split up the ciphertext into blocks of 2
    tmp = []
    for x in range(0, len(plntxt) // 2):
        tmp.append(plntxt[(x * 2)] + plntxt[(x * 2) + 1])
    plntxt = tmp

    # transform the text based on cipher matrix
    for x in range(0, len(plntxt)):

        block = plntxt[x]  # select block

        # seperate values of chosen block
        a = block[0]
        b = block[1]

        # determine position of each value in cipher matrix
        a_x = cipmat.index(a) % wid
        a_y = cipmat.index(a) // wid
        b_x = cipmat.index(b) % wid
        b_y = cipmat.index(b) // wid

        # If each value is on the same row, assign to the value to the left
        if(a_y == b_y):
            if(a_x < 1):
                a_x = 8
            else:
                a_x -= 1

            if(b_x < 1):
                b_x = 9
            else:
                b_x -= 1
        # If both values are in the same column, assign to the value above
        elif(a_x == b_x):
            if(a_y < 1):
                a_y = 8
            else:
                a_y -= 1

            if(b_y < 1):
                b_y = 9
            else:
                b_y -= 1
        # If both x and y values are different, swap x values
        else:
            a_x, b_x = b_x, a_x

        # Replace existing block with newly ciphered one
        a = cipmat[(a_y * wid) + a_x]
        b = cipmat[(b_y * wid) + b_x]
        plntxt[x] = a + b

    return "".join(plntxt)

###ENCRYPTION CESAR###
def encC(texto, chave, modo):
    texto_encript = ''
    alphamat = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                'U', 'V', 'W', 'X', 'Y', 'Z', 'Á', 'É', 'Í', 'Ó',
                'Ú', 'Ã', 'Õ', 'Â', 'Ê', 'Î', 'Ô', 'Û', '0', '1',
                '2', '3', '4', '5', '6', '7', '8', '9', 'Ç', '!',
                '@', '#', '$', '%', '"', '&', '*', '(', ')', '-',
                ',', '.', '<', '>', '/', '\\', 'ç', ':', '{', '}',
                '\'', ';', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                'i', 'j', 'k', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']

    alphamat.sort()

    alphabet=list(alphamat)
    texto_list = list(texto);

    for char in texto_list:
        indice = alphabet.index(char)
        if indice+chave <= 99:
            texto_encript = texto_encript + str(alphabet[indice+chave])
        else:
            texto_encript = texto_encript + str(alphabet[(indice+chave)-100])


    return "".join(texto_encript)

###DECRYPTION CESAR###
def decC(texto, chave, modo):
    texto_decript = ""
    alphamat = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                'U', 'V', 'W', 'X', 'Y', 'Z', 'Á', 'É', 'Í', 'Ó',
                'Ú', 'Ã', 'Õ', 'Â', 'Ê', 'Î', 'Ô', 'Û', '0', '1',
                '2', '3', '4', '5', '6', '7', '8', '9', 'Ç', '!',
                '@', '#', '$', '%', '"', '&', '*', '(', ')', '-',
                ',', '.', '<', '>', '/', '\\', 'ç', ':', '{', '}',
                '\'', ';', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                'i', 'j', 'k', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']

    alphamat.sort()

    alphabet=list(alphamat)
    texto_list = list(texto);

    for char in texto_list:
        indice = alphabet.index(str(char))
        if indice-chave >= 0:
            texto_decript = texto_decript + str(alphabet[indice-chave])
        else:
            texto_decript = texto_decript + str(alphabet[99-chave])
    return "".join(texto_decript)

###ENCRYPTION RAILFENCE###
def encR(inputString,rails):
    listoflines = list()
    outputList = list()

    for i in range(rails):
        listoflines.append(list())

    index = 0
    direction = 0 #direction. 0= down 1= up

    for letter in inputString:
        (listoflines[index]).append(letter)

        if index==0:
            direction=0
            index=1
            continue
        elif index==(rails-1):
            direction=1
            index=rails-2
            continue
        elif direction==0:
            index=index+1
            continue
        elif direction==1:
            index=index-1
            continue
        else:
            quit()

    for line in listoflines:
        for c in line:
            outputList.append(c)

    return ''.join(outputList)

def decR(inputString,rails):
    outputList = list()
    listoflines = list()
    listofindexes = list()
    for i in range(rails):
        listoflines.append(list())
        listofindexes.append(0)
    index = 0
    direction = 0 #direction. 0= down 1= up

    for letter in inputString:
	    (listoflines[index]).append(0)
	    if index==0:
		    direction=0
		    index=1
		    continue
	    elif index==(rails-1):
		    direction=1
		    index=rails-2
		    continue
	    elif direction==0:
		    index=index+1
		    continue
	    elif direction==1:
		    index=index-1
		    continue
	    else:
		    quit()

    index=0
    lineindex=0

    for line in listoflines:
	    lineindex=0
	    for c in line:
		    line[lineindex]=inputString[index]
		    lineindex+=1
		    index+=1
    index = 0
    direction = 0 #direction. 0= down 1= up
    for letter in inputString:
	    outputList.append((listoflines[index])[listofindexes[index]])
	    listofindexes[index]+=1
	    if index==0:
		    direction=0
		    index=1
		    continue
	    elif index==(rails-1):
		    direction=1
		    index=rails-2
		    continue
	    elif direction==0:
	        index=index+1
	        continue
	    elif direction==1:
		    index=index-1
		    continue
	    else:
		    quit()
    return ''.join(outputList)

def cifrar():
    todas_chaves = []
    times=int(input("Inserir o número de vezes a ser cifrado: \n"))
    # if times < 2:
    #     print("Menor que duas rodadas, não é possivel cifrar, saindo do modo cifragem")
    #     return
    try:
        nome = str(input('Digite o nome do arquivo para cifrar: \n'))
    except ValueError:
        print("Nome de arquivo invalido")

    if os.path.exists(nome):
        cifrar_fd = open(nome, "r")
        cifrado = open(nome + "_cifrado", "w")
        cif = cifrar_fd.read()
        cifrar_fd.close()
        cif= cif.replace(' ','')
        cif= cif.replace('[','{')
        cif= cif.replace(']','}')
        cif= cif.replace('?','')
        cif= cif.replace('_','')
        cif= cif.replace("\n",'')
        cif= cif.replace("\b",'')

        for x in range(0, times):
            chaves = {}
            print("Chave de número:",x+1)
            print("\n")
            play = str(input("Inserir chave da PLAYFAIR:\n"))
            cesar = int(input("Inserir chave de CESAR: \n"))
            if(cesar < 3):
                print("chave de cesar menor que 3, saindo do modo de cifragem \n")
                return
            # rail = int(input("Inserir chave da RAILFENCE: \n"))
            # if(rail < 2):
            #     print("chave da railfence menor que 2, saindo do modo de cifragem \n")
            #     return
            play = play.replace(" ","")
            chaves['play']=play
            chaves['cesar']=cesar
            # chaves['rail']=rail
            todas_chaves.append(chaves)
            txt_cifrado=ascii(cif)
        for x in todas_chaves:
            # txt_cifrado=encR(txt_cifrado,x['rail'])
            txt_cifrado=encC(txt_cifrado,x['cesar'],0)
            txt_cifrado=encP(txt_cifrado,x['play'])

        cifrado.write(txt_cifrado)
        cifrado.close()
    else:
        print("Arquivo não existe")

def decifrar():
    todas_chaves = []
    times=int(input("Inserir o número de vezes a ser decifrado: \n"))
    # if times < 2:
    #     print("Menor que duas rodadas, não é possivel cifrar, saindo do modo cifragem")
        # return
    try:
        nome = str(input('Digite o nome do arquivo para decifrar: \n'))
    except ValueError:
        print("Nome de arquivo invalido")

    if os.path.exists(nome):
        decifrar_fd = open(nome, "r")
        decifrado = open(nome + "_decifrado", "w")
        decif = decifrar_fd.read()
        decifrar_fd.close()
        decif= decif.replace(' ','')

        for x in range(0, times):
            chaves = {}
            print("Chave de número:",x+1)
            play = str(input("Inserir chave da PLAYFAIR:\n"))
            cesar = int(input("Inserir chave de CESAR: \n"))
            if(cesar < 3):
                print("chave de cesar menor que 3, saindo do modo de decifragem \n")
                return
            # rail = int(input("Inserir chave da RAILFENCE: \n"))
            # if(rail < 2):
            #     print("chave da railfence menor que 2, saindo do modo de cifragem \n")
            #     return
            play = play.replace(" ","")
            chaves['play']=play
            chaves['cesar']=cesar
            # chaves['rail']=rail
            todas_chaves.append(chaves)
            txt_decifrado=decif

        for x in todas_chaves:
             txt_decifrado=decP(txt_decifrado,x['play'])
             txt_decifrado=decC(txt_decifrado,x['cesar'],1)
            #  txt_decifrado=decR(txt_decifrado,x['rail'])

        decifrado.write(txt_decifrado)
        decifrado.close()
    else:
        print("Arquivo não existe")


#====== Código principal =====#

while(1):
    try:
        choice = int(
            input("\nModos: \n1.Cifrar: \n2.Decifra: \n3.Sair:\n"))
    except ValueError:
        print("Modo invalido")

    if choice == 1:
        cifrar()

    if choice == 2:
        decifrar()

    if choice == 3:
        exit()
