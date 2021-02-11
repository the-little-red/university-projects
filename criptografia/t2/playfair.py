from copy import deepcopy
import os
from sys import platform
from collections import OrderedDict
import string
import re
import unicodedata
import itertools
import multiprocessing
from math import log
import difflib

# Split list in a num of lists


def chunkIt(seq, num):
    avg = len(seq) / float(num)
    out = []
    last = 0.0

    while last < len(seq):
        out.append(seq[int(last):int(last + avg)])
        last += avg

    return out


###DECRYPTION###
def break_keys(keys, decif, path, nome, num):
    print("Thread: " + str(num))
    for k in keys:
        decifrado = open(path + "/" + nome + "_" + k, "w")
        txt_decifrado = dec(str(decif), k)
        decifrado.write(txt_decifrado)
        decifrado.close()

    print("End thread: " + str(num))

###CIPHER MATRIX CREATION###
def create_matrix(keyword):

    # Define base alphabet matrix
    alphamat = ['A', 'B', 'C', 'D', 'E',
                'F', 'G', 'H', 'I', 'K',
                'L', 'M', 'N', 'O', 'P',
                'Q', 'R', 'S', 'T', 'U',
                'V', 'W', 'X', 'Y', 'Z']

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
    for x in range(len(unqkey), len(cipmat)):
        cipmat[x] = rmdr[x - len(unqkey)]

    return cipmat


###ENCRYPTION###
def enc(ciptxt, key):

    ciptxt = list(ciptxt)
    # Create new cipher table using given key
    cipmat = create_matrix(key)

    wid = 5  # key table matrix width
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
            if(a_x > 3):
                a_x = 0
            else:
                a_x += 1

            if(b_x > 3):
                b_x = 0
            else:
                b_x += 1
        # If both values are in the same column, assign to the value below
        elif(a_x == b_x):
            if(a_y > 3):
                a_y = 0
            else:
                a_y += 1

            if(b_y > 3):
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

###DECRYPTION###


def dec(plntxt, key):

    # Create new cipher table using given key
    cipmat = create_matrix(key)

    wid = 5  # key table matrix width
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
                a_x = 4
            else:
                a_x -= 1

            if(b_x < 1):
                b_x = 4
            else:
                b_x -= 1
        # If both values are in the same column, assign to the value above
        elif(a_x == b_x):
            if(a_y < 1):
                a_y = 4
            else:
                a_y -= 1

            if(b_y < 1):
                b_y = 4
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
##TREAT KEY FORMAT##


def key_treat():
    key = input("Insira a Chave \n")
    key = key.replace(" ", "")
    key = key.upper()
    return key


def strip_accents(s):  # remove acentuação
    return ''.join(c for c in unicodedata.normalize('NFD', s)
                   if unicodedata.category(c) != 'Mn')

##TREAT KEY FORMAT##


def break_text():
    try:
        nome = str(input('Digite o nome do arquivo para quebrar: \n'))
    except ValueError:
        print("Nome invalido")

    # try:
    #     min = int(input('Digite o tamanho minimo da chave: \n'))
    # except ValueError:
    #     print("Tamanho invalido")
    #
    # try:
    #     max = int(input('Digite o tamanho maximo da chave: \n'))
    # except ValueError:
    #     print("Tamanho invalido")

    # try:
    #     choice = int(input('Escolha um tipo:\n 1): \n'))
    # except ValueError:
    #     print("Tamanho invalido")

    if os.path.exists(nome):
        decifrar_fd = open(nome, "r")
        decif = decifrar_fd.read().strip()
        decifrar_fd.close()
        decif = re.sub("[^a-zA-Z]+", "", decif).replace('J', 'I').replace(' ', '').upper()

        if platform == "linux" or platform == "linux2":
            if os.path.exists("/usr/share/dict/brazilian"):
                dict = open("/usr/share/dict/brazilian",
                            "r", encoding="ISO-8859-1")
            else:
                dict = open("dict", "r", encoding="ISO-8859-1")
        elif platform == "darwin":
            dict = open("dict", "r", encoding="ISO-8859-1")
        elif platform == "win32":
            print("Você está usando windows, para de fazer isso")
            dict = open("dict", "r", encoding="ISO-8859-1")

        keys = []
        all_keys = []
        all = dict.readlines()
        dict.close()
        for k in all:
            key_k = strip_accents(k).upper().replace('J', 'I').strip()
            all_keys.append(key_k)

        # keys = re.findall(r'\b[A-Z]{min,max}\b',' '.join(all_keys))
        keys = re.findall(r'\b[A-Z]{4,6}\b', ' '.join(all_keys))
        path = str(nome + "_quebrados")
        if not os.path.exists(path):
            try:
                os.makedirs(path)
            except OSError:
                print("não foi possível criar diretório:" + path)

        # for i in range(min, max):
        #     chars = "ABCDEFGHIKLMNOPQRSTUVWXYZ"
        #     for nchar in itertools.product(chars, repeat=i):
        #         k = "".join(nchar)
        #         decifrado=open(path+"/"+nome+"_"+k,"w")
        #         txt_decifrado = dec(str(decif),k)
        #         decifrado.write(txt_decifrado)
        #         decifrado.close()

        # for k in keys:
        #     decifrado = open(path + "/" + nome + "_" + k, "w")
        #     txt_decifrado = dec(str(decif), k)
        #     decifrado.write(txt_decifrado)
        #     decifrado.close()

        if __name__ == '__main__':
            keys_4 = []
            max_process = multiprocessing.cpu_count() - 1
            keys_4 = chunkIt(keys, max_process)
            pool = multiprocessing.Pool()
            for i in range(0, max_process):
                pool.apply_async(break_keys, args=(
                    keys_4[i], decif, path, nome, i))
            pool.close()
            pool.join()

            #p0 = Process(target=break_keys, args=(keys_4[0], decif, path, nome))
            # p0.start()
            # p0.join()

            print("Terminou")


def infer_spaces(s,max_len):
    #"""Uses dynamic programming to infer the location of spaces in a string without spaces."""

    # Find the best match for the i first characters, assuming cost has
    # been built for the i-1 first characters.
    # Returns a pair (match_cost, match_length).
    def best_match(i):
        candidates = enumerate(reversed(cost[max(0, i - maxword):i]))
        return min((c + wordcost.get(s[i - k - 1:i], 9e999), k + 1) for k, c in candidates)

    # Build the cost array.
    cost = [0]
    for i in range(1, int(max_len) + 1):
        c, k = best_match(i)
        cost.append(c)

    # Backtrack to recover the minimal-cost string.
    out = []
    i = int(max_len)
    while i > 0:
        c, k = best_match(i)
        assert c == cost[i]
        out.append(s[i - k:i])
        i -= k

    return " ".join(reversed(out))

def paralell_filter(files,nome,num):

    print("Thread: " + str(num))
    resul_fd = open("result_pt_"+nome+"_"+str(num),"w")
    for file in files:
        file_fd = open(nome + "_quebrados/"+file, "r")
        text_dec = file_fd.read()
        file_fd.close()
        possible_words = re.findall(r'\b[A-Z]{4,6}\b',infer_spaces(text_dec,300))[:250]
        # possible_words = re.findall(r'\b[A-Z]{4,6}\b',infer_spaces(text_dec))[:250]
        num_found = 0
        for word in words:
            num_found = num_found + len(difflib.get_close_matches(str(word),possible_words,cutoff=0.9,n=10))
        resul_fd.write(file+":"+str(num_found)+"\n")
    resul_fd.close()
    print("End thread: " + str(num))

def filter(nome):
    files = os.listdir(nome + "_quebrados")

    if __name__ == '__main__':
        max_process = multiprocessing.cpu_count() - 1
        files_4 = chunkIt(files, max_process)


        pool = multiprocessing.Pool()
        for i in range(0, max_process):
            pool.apply_async(paralell_filter, args=(
                files_4[i], nome, i))
        pool.close()
        pool.join()
    resul_fd = open("result_pt_"+nome,"w")
    for i in range(0, max_process):
        filei = open("result_pt_"+nome+"_"+str(i),"r")
        texti = filei.read()
        resul_fd.write(texti)
        filei.close()
        os.remove("result_pt_"+nome+"_"+str(i))
    resul_fd.close()
    #
    # for file in files:
    #     decifrar_fd = open(nome + "_quebrados/"+file, "r")
    #     text_dec = decifrar_fd.read()
    #     decifrar_fd.close()
    #     possible_words = re.findall(r'\b[A-Z]{4,6}\b',infer_spaces(text_dec))
    #     num_found = 0
    #     print(file)
    #     for word in words:
    #         num_found = num_found + len(difflib.get_close_matches(str(word),possible_words,cutoff=0.9,n=10))
    #     print(file+":"+found)
def sort_score(nome):
    with open(nome, 'r') as f:
        scoreList = f.readlines()

    # this will sort the list based on the float that comes after " scored "
    scoreList.sort(key = lambda line: float(line.strip().split(":")[-1]))
    scores = scoreList[-10:]
    for score in reversed(scores):
        print(score.strip())

while(1):

    try:
        choice = int(
            input("\nModos: \n1.Cifrar: \n2.Decifra: \n3.Quebra: \n4.Filtrar: \n5.Sair:\n"))
    except ValueError:
        print("Nome invalido")

    if choice == 1:
        key = key_treat()
        if len(key) >= 3 or len(key) > 5:
            try:
                nome = str(input('Digite o nome do arquivo para cifrar: \n'))
            except ValueError:
                print("Nome invalido")
            if os.path.exists(nome):
                cifrar_fd = open(nome, "r")
                cifrado = open(nome + "_cifrado", "w")
                cif = cifrar_fd.read().upper()
                cifrar_fd.close()
                cif = re.sub("[^a-zA-Z]+", "", cif).replace('J', 'I')
                txt_cifrado = enc(str(cif), key)
                cifrado.write(txt_cifrado)
                cifrado.close()
            else:
                print("Arquivo não existe")

        else:
            print("Chave de tamanho, a chave de ter Tam entre 3 a 5 \n")

    elif choice == 2:
        key = key_treat()
        if len(key) >= 3 or len(key) > 5:
            try:
                nome = str(input('Digite o nome do arquivo para decifrar: \n'))
            except ValueError:
                print("Nome invalido")
            if os.path.exists(nome):
                decifrar_fd = open(nome, "r")
                decifrado = open(nome + "_decifrado", "w")
                decif = decifrar_fd.read().strip()
                decifrar_fd.close()
                txt_decifrado = dec(str(decif), key)
                decifrado.write(txt_decifrado)
                decifrado.close()
            else:
                print("Arquivo não existe")
        else:
            print("Chave de tamanho, a chave de ter Tam entre 3 a 5 \n")

    elif choice == 3:
        break_text()
    elif choice == 4:
        try:
            nome = str(input('Digite o nome do arquivo para filtrar: \n'))
        except ValueError:
            print("Nome invalido")
        if os.path.exists(nome + "_quebrados"):
            words = open("dict","r",encoding="ISO-8859-1").read().upper().replace('\'S', '').replace('J', 'I').strip().split()
            wordcost = dict((k, log((i + 1) * log(len(words))))
                            for i, k in enumerate(words))
            maxword = max(len(x) for x in words)
            filter(nome)
            sort_score("result_pt_"+nome)
        else:
            print("Arquivo não existe")

    elif choice == 5:
        exit()
    else:
        print("Opção inexistente \n")
