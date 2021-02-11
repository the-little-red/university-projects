Main Page {#mainpage}
=========
Implementado por: Arianne de Paula Bortolan GRR:20140220

O trabalho funciona para matrizes de dimensão 150x150, matrizes maiores o sistema do dinf mata a execução devido ao espaço de memoria alocado.
Por não haver especificação de dimensão mínima para a matriz a ser executada, o trabalho foi mantido assim. (possivel otimização no futuro).
A pré-condicionantes jacobi e SSOR foram implementadas.

*ATENÇÂO*: SSOR executa com erros, não sendo recomendado seu uso (bug de over/underflow).

*ATENÇÂO 2*: Todo o código foi comentado com //, porém o doxy não pegou os comentarios, recomendo olhar o código fonte que está comentado caso aja alguma dúvida.
