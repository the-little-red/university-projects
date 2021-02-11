def compress(uncompressed):
    """Compressor."""

    # Constroi o dicionario.
    dict_size = 256
    dictionary = dict((chr(i), chr(i)) for i in xrange(dict_size))
    # Caso Python 3: dictionary = {chr(i): chr(i) for i in range(dict_size)}

    w = ""
    result = []
    for c in uncompressed:
        wc = w + c
        if wc in dictionary:
            w = wc
        else:
            result.append(dictionary[w])
            # Adiciona wc ao dicionario.
            dictionary[wc] = dict_size
            dict_size += 1
            w = c

    if w:
        result.append(dictionary[w])
    return result



def decompress(compressed):
    """Decomprensor."""

    # Constroi o dicionario.
    dict_size = 256
    dictionary = dict((chr(i), chr(i)) for i in xrange(dict_size))
    # Caso Python 3: dictionary = {chr(i): chr(i) for i in range(dict_size)}

    w = result = compressed.pop(0)
    for k in compressed:
        if k in dictionary:
            entry = dictionary[k]
        elif k == dict_size or k == '':
            entry = w + w[0]
        else:
            k = int(k)
            if k in dictionary:
                entry = dictionary[k]
            elif k == dict_size:
                entry = w + w[0]
        result += entry

        # Adiciona w+entry[0] ao dicionario.
        dictionary[dict_size] = w + entry[0]
        dict_size += 1

        w = entry
    return result

def main():
    try:
        mode=str(raw_input('Digite d para descompactar ou c para compactar '))
    except ValueError:
        print "Operacao invalida"

    if mode == "c" :

        try:
            nome=str(raw_input('Digite o nome do arquivo para compactar: '))
        except ValueError:
            print "Nome invalido"

        liscomp = []
        uncom = open(nome)
        comp = open(nome +"_lzw","w")
        liscomp = uncom.read()
        compressed = compress(liscomp)

        for item in compressed:
            comp.write("%s\t" % item)

        comp.write("")
        uncom.close()
        comp.close()
        print "Arquivo Comprimido!"
    elif mode == "d":
        try:
            nome=str(raw_input('Digite o nome do arquivo para descompactar: '))
        except ValueError:
            print "Nome invalido"

        liscomp = []
        c = open(nome+"d","w")
        liscomp = open(nome).read().split('\t')
        #print(liscomp)
        decomp = decompress(liscomp)

        print "Arquivo Descomprimido!"
        #print (decomp)
        c.write(decomp)

        c.close()
main()
