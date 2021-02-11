#!/bin/bash
EXEMPLOS="Exemplo5 Exemplo7.1 Exemplo7.2 Exemplo7.3 Exemplo7.4 Exemplo7.5 Exemplo8.10 Exemplo8.12 Exemplo8.5 Exemplo8.6 Exemplo8.7 Exemplo8.8 Exemplo8.9 ExemploErro1 ExemploErro2"


echo "Usage: avaliaTrab.sh <arquivo executavel compilador"
echo "Avalia��o: Para os primeiros quatro erros, meio ponto por erro."
echo "Avalia��o: Para os demais, um ponto por erro."

desconto=0
for exemplo in $EXEMPLOS; do
    echo -n $exemplo "... "
    cp $exemplo/pgma.pas .
    cp $exemplo/MEPA MEPA-Res
	
    
    $1 pgma.pas > res
    diff MEPA MEPA-Res  -bBt 
    echo $exemplo "encerrado teste exemplo"
done

