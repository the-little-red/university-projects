#include <stdio.h>
//## estrutura dos tempos ##
typedef struct {

  double Tempo_pc;
  double Tempo_iter;
  double Tempo_residuo;

} Tempo;

//## estrutura com todas as variaveis necessária s##
typedef struct {

  //matrizes implementadas em vetor, menos pagefault
  double * matriz_I; //matriz identidade/inversa
  double * matriz_A;
  double * matriz_ATrans;
  double * matriz_ATransB;
  double * vetor_b;
  double * vetor_x;
  double * vetor_r;

  FILE* file; //arquivo de saida -o
  unsigned int MAX_iTERACOES; //parâmetro obrigatório definindo o número máximo de iterações a serem executadas. -i
  long int N; //parâmetro obrigatório definindo a dimensão do Sistema Linear. -n
  long int K; //parâmetro obrigatório definindo o número de diagonais da matriz -k
  double PC; //parâmetro obrigatório indicando o pré-condicionador a ser utilizado: -p
  double E; // parâmetro opcional definindo o erro aproximado absoluto máximo -e
  double residuo;
} Dados;
