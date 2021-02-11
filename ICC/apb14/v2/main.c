#define INLINE
#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include "main.h"
#include <likwid.h>

// ############ Funções referentes a operações com matrizes e vetores ############ //

// imprime a matriz (função pra debug)
void imprime_m (double* matriz, int N){
  int i,j;
  for(i=0; i < N; i++){
    for(j=0; j < N; j++){
      printf("%lf ", matriz[(i*N)+j]);
    }
    printf("\n");
  }
}
// calcula a matriz transposta com vetores, matriz_r é a matriz transposta calculada
//multiplica um vetor com uma constante e soma com outro vetor
void mult_soma_vetor(double* vetor_r, double* vetor_a, double* vetor_b, double variavel, int N){
  int i;
  for(i=0; i < N -(N%4); i+=4){
    vetor_r[i]= vetor_a[i] + (variavel* vetor_b[i]);
    vetor_r[i+1]= vetor_a[i] + (variavel* vetor_b[i+1]);
    vetor_r[i+2]= vetor_a[i] + (variavel* vetor_b[i+2]);
    vetor_r[i+3]= vetor_a[i] + (variavel* vetor_b[i]+3);
  }
  for(i=N -(N%4); i < N; i++){
    vetor_r[i]= vetor_a[i] + (variavel* vetor_b[i]);
  }
}
void calcula_transpostaV(double* vetor_r,double* vetor_rb, double* matriz_a ,int N){
  int i,j;
  for(i=0; i < N ; i++){
    vetor_r[i]=0.0;
    for(j=0; j < N ; j++){
      vetor_r[i] = vetor_r[i] + (matriz_a[(j*N)+i] *vetor_rb[j]);
    }
  }
}
// calcula a matriz transposta com matriz, matriz_r é a matriz transposta calculada
void calcula_transpostaM(double* matriz_r, double* matriz_a ,int N){
  int i,j,calc;
  for(i=0; i < N; i++){
    for(j=0; j < N; j++){
      matriz_r[(i*N)+j] = 0.0;
      for(calc=0; calc < N; calc++){
        matriz_r[(i*N)+j] += matriz_a[(calc*N)+i] * matriz_a[(calc*N)+j];
      }
    }
  }
}
//realiza a multiplicação entre duas matrizes, matriz_r eh o resultado da multiplicação
void multiplica_matrizes(double* matriz_r, double* matriz_a, double* matriz_b, int N){
  int i,j,calc;
  for(i=0; i < N ; i++){
    for(j=0; j < N ;j++){
      matriz_r[(i*N)+j] = 0.0;
      for(calc=0; calc < N ; calc++){
        matriz_r[(i*N)+j] += matriz_a[(i*N)+calc] * matriz_b[(calc*N)+j];
      }
    }
  }
}
// produto escalar entre vetores
double vetores_produto(double* vetor_x, double* vetor_y, int N){
  double r = 0;
  int i;
  for(i=0; i < N; i++) {
    r= r + (vetor_x[i] * vetor_y[i]);
  }
  return r;
}
//calcula a norma euclidiana de um vetor
double norma_euclidiana(double* x, int N){
  double r = 0;
  int i;
  for(i=0; i < N; i++){
    r = r + (x[i]*x[i]);
  }
  r = sqrt(r);
  return r;
}
// multiplica matriz com o vetor xb, vetor_r que é gerado, é o resultado da multiplicação
void matriz_multi_vetor(double* matriz_x, double* vetor_xb, double* vetor_r, int N){
  int i,j;
  for(i=0; i < N; i++){
    vetor_r[i] = 0.0;
    for(j=0; j < N; j++) {
      vetor_r[i] = vetor_r[i] + (matriz_x[(i*N)+j] * vetor_xb[j]);
    }
  }
}
// ############ Funções referente a inicializar variaveis ############ //
/***********************
 * Função que gera os coeficientes de um sistema linear k-diagonal
 * i,j: coordenadas do elemento a ser calculado (0<=i,j<n)
 * k: numero de diagonais da matriz A
 ***********************/
static inline double generateRandomA( unsigned int i, unsigned int j, unsigned int k )
{
  double invRandMax = 1.0 / (double)RAND_MAX;
  return ( (i==j)?(double)(k<<1) : 1.0 )  * (double)rand() * invRandMax;
}
/***********************
 * Função que gera os termos independentes de um sistema linear k-diagonal
 * k: numero de diagonais da matriz A
 ***********************/
static inline double generateRandomB( unsigned int k )
{
  double invRandMax = 1.0 / (double)RAND_MAX;
  return (double)(k<<2) * (double)rand() * invRandMax;
}
// Função de tempo
double timestamp(void){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}
// Função de zerar
void zera_vetores(double *vetor, int tam){
  //não sei tamanho que será a matriz ou vetor, por isso uso memset para zerar
  memset(vetor, 0, tam*sizeof(double));
}
// ############ Função que inicializa os atributos das structs ############ //
void inicializa(Tempo *dados_tempos, Dados *dados_variaveis){
  int i,j;
  long int tam_dimensao = dados_variaveis->N;

  srand(20182);

  dados_tempos->Tempo_pc = dados_tempos->Tempo_iter = dados_tempos->Tempo_residuo = 0.0;

  dados_variaveis->matriz_A = (double*) malloc (sizeof(double)*tam_dimensao*tam_dimensao);
  assert(dados_variaveis->matriz_A != NULL);
  dados_variaveis->matriz_I = (double*) malloc (sizeof(double)*tam_dimensao*tam_dimensao);
  assert(dados_variaveis->matriz_I != NULL);
  dados_variaveis->matriz_ATrans = (double*) malloc (sizeof(double)*tam_dimensao*tam_dimensao);
  assert(dados_variaveis->matriz_ATrans != NULL);
  dados_variaveis->matriz_ATransB = (double*)  malloc (tam_dimensao* (sizeof(double)));
  assert(dados_variaveis->matriz_ATransB != NULL);
  dados_variaveis->vetor_b =(double*)  malloc (tam_dimensao * (sizeof(double)));
  assert(dados_variaveis->vetor_b != NULL);
  dados_variaveis->vetor_x =(double*)  malloc (tam_dimensao * (sizeof(double)));
  assert(dados_variaveis->vetor_x != NULL);
  dados_variaveis->vetor_r =(double*)  malloc (tam_dimensao * (sizeof(double)));
  assert(dados_variaveis->vetor_r != NULL);

  // inicializa com 0 todos os vetores e dados_variaveis que ainda não tem dados para previnir de trabalhar com lixo da memoria ou segfault
  zera_vetores(dados_variaveis->vetor_b, tam_dimensao);
  zera_vetores(dados_variaveis->vetor_x, tam_dimensao);
  zera_vetores(dados_variaveis->vetor_r, tam_dimensao);
  zera_vetores(dados_variaveis->matriz_A, tam_dimensao*tam_dimensao);
  zera_vetores(dados_variaveis->matriz_I, tam_dimensao*tam_dimensao); //por default crio a matriz I como identidade pra uso no caso de PC = 0.0
  zera_vetores(dados_variaveis->matriz_ATrans, tam_dimensao*tam_dimensao);
  zera_vetores(dados_variaveis->matriz_ATransB, tam_dimensao);


  for(i=0; i< dados_variaveis->N ; i++){
      //gera vetor b
      dados_variaveis->vetor_b[i] = generateRandomB((unsigned int)dados_variaveis->K);
    for(j=0; j < dados_variaveis->N ; j++){

      //insere valores matriz A
      dados_variaveis->matriz_A[(i*dados_variaveis->N)+j] = generateRandomA((unsigned int) i, (unsigned int) j,(unsigned int)dados_variaveis->N); //elementos não diagonais

      //insere valores matriz I
      if(i==j){
        dados_variaveis->matriz_I[(i*dados_variaveis->N)+j] = 1.0;
      }
    }
  }
  //gera transpostas
  calcula_transpostaM(dados_variaveis->matriz_ATrans, dados_variaveis->matriz_A, dados_variaveis->N);
  calcula_transpostaV(dados_variaveis->matriz_ATransB,dados_variaveis->vetor_b,  dados_variaveis->matriz_A, dados_variaveis->N);
  // imprime_m(dados_variaveis->matriz_A, dados_variaveis->N);
  // imprime_m(dados_variaveis->matriz_I, dados_variaveis->N);
  // imprime_m(dados_variaveis->matriz_ATrans, dados_variaveis->N);s
  // imprime_m(dados_variaveis->matriz_ATransB, dados_variaveis->N);

}
// ############ Funções referentes as pré condicionantes ############ //

//calcula fatoracao cholesky da matriz triangular inferior
void fatoracao(double * matriz, int N){
  int i,j,k;
  for(i=0; i < N; i++){
    matriz[(i*N)+i] = sqrt(matriz[(i*N)+i]);

    for(j=i+1; j < N; j++){
      if(matriz[(j*N)+i] != 0.0){
        matriz[(j*N)+i] /=  matriz[(i*N)+i];
      }
    }

    for(k=i+1; k < N; k++){
      for(j=k; j < N; j++){
        if(((j*N)+i) > (N*N)){
          break;
        }
        if(matriz[(j*N)+k] != 0.0){
          matriz[(j*N)+k] = matriz[(j*N)+k] - (matriz[(j*N)+i] * matriz[(k*N)+i]);
        }
      }
    }
  }
  for(i=0; i < N; i++){
    for(j=i+1; j < N; j++){
      matriz[(i*N)+j] = 0.0;
    }
  }
}
// faz retrosubstituição
void retro_substitui(double* matriz_a, double * vetor_x, int index, int N){
  int i,j;
  double z;
  for(i=0; i < N; i++){
    if(i==index){
      z = 1.0;
    }else{
      z = 0.0;
    }
    for(j=0; j < N; j++){
      z = z - (matriz_a[(i*N)+j] * vetor_x[j]);
    }
    vetor_x[i] = z / matriz_a[(i*N)+i];
  }
}
// calcula a matriz inversa
void inversa(double *matriz, int N){
  int i;
  double* matriz_aux = (double*) malloc (sizeof(double)*N*N);
  memset(matriz_aux,0,sizeof(double)*N*N);
  fatoracao(matriz, N);
  for(i = 0; i < N; i++){
    retro_substitui(matriz,matriz_aux + (i * N), i, N);
  }
  calcula_transpostaM(matriz, matriz_aux, N);
}

//função que gera matriz pré condicionador de jacobi
void gera_pc_jacobi(Dados * dados_variaveis){
  int i, tam = dados_variaveis->N * dados_variaveis->N;
  memset(dados_variaveis->matriz_I, 0, tam * sizeof(double));

  for(i=0; i < dados_variaveis->N; i++){
    dados_variaveis->matriz_I[(i*dados_variaveis->N)+i] = 1 / dados_variaveis->matriz_A[(i*dados_variaveis->N)+i] ;
  }
}

//função que gera pré condicionador SSOR
void gera_pc_ssor(Dados* dados_variaveis){
  int i,j;
  int tam = dados_variaveis->N * dados_variaveis->N;
  double * L = (double*) malloc (sizeof(double)*tam);
  double * U = (double*) malloc (sizeof(double)*tam);

  memset(L,0,sizeof(double)*tam);
  memset(U,0,sizeof(double)*tam);

  for(i=0; i < dados_variaveis->N; i++){
    for(j=0; j < dados_variaveis->N; j++){
      if(i == j){
        U[(i*dados_variaveis->N)+j]=1.0;
      }else if(i < j){
        U[(i*dados_variaveis->N)+j]= (dados_variaveis->PC * dados_variaveis->matriz_A[(i*dados_variaveis->N)+j]) / dados_variaveis->matriz_A[(i*dados_variaveis->N)+j];
      }else{
        U[(i*dados_variaveis->N)+j]=0.0;
      }
    }
  }
  for(i=0; i < dados_variaveis->N; i++){
    for(j=0; j < dados_variaveis->N; j++){
      if(i == j){
        L[(i*dados_variaveis->N)+j]=1.0;
      }else if(i > j){
        L[(i*dados_variaveis->N)+j]= dados_variaveis->PC * dados_variaveis->matriz_A[(i*dados_variaveis->N)+j];
      }else{
        L[(i*dados_variaveis->N)+j]=0.0;
      }
    }
  }
  multiplica_matrizes(dados_variaveis->matriz_I,L,U,dados_variaveis->N);
  inversa(dados_variaveis->matriz_I,dados_variaveis->N);
}

// ############ Funções referentes a gradiente conjugados ############ //
//calcula o erro residuo
double erro_residuo(double *vetor_r, int N){
  return vetores_produto(vetor_r,vetor_r, N);
}

//calcula o erro aproximado
double calcula_erro(double* vetor_x, double* vetor_xa, int N){
  double aux;
  double max = 0;
  int i;
  for(i=0; i < N; i++){
    aux = fabs((vetor_x[i] - vetor_xa[i])/vetor_x[i]);
    if(max < aux){
      max=aux;
    }
  }
  return max;
}
// verifica se a matriz dada atende ao critério das linhas
int criterio(double * matriz, int N){
  int i,j;
  double aux;
  double max;
  for(i=0; i < N; i++){
    max = 0;
    for(j=0; j < N; j++){
      aux = matriz[(i*N)+j];
      if(max < aux){
        max = aux;
      }
    }
    if(max > matriz[(i*N)+i]){
      return 0; //não atende criterio
    }
  }
  return 1; //atendeu o criterio
}
// função do gradienteConjugado
void gradiente_conjugado(Dados* dados_variaveis){
  int tam = dados_variaveis->N;
  int cont;
  unsigned int i;
  double rp, aux, erro, produto, w, h;
  double* v = (double*) malloc (sizeof(double)*tam);
  double* y = (double*) malloc (sizeof(double)*tam);
  double* r = (double*) malloc (sizeof(double)*tam);
  double* z = (double*) malloc (sizeof(double)*tam);
  double* xa = (double*) malloc (sizeof(double)*tam);

  memset(v,0,sizeof(double)*tam);
  memset(y,0,sizeof(double)*tam);
  memset(r,0,sizeof(double)*tam);
  memset(z,0,sizeof(double)*tam);
  memset(xa,0,sizeof(double)*tam);

  matriz_multi_vetor(dados_variaveis->matriz_I, dados_variaveis->matriz_ATransB, v, dados_variaveis->N);

  memcpy(y, v, tam * sizeof(double));
  memcpy(r, dados_variaveis->matriz_ATransB, tam * sizeof(double));

  rp = vetores_produto(y, dados_variaveis->matriz_ATransB, dados_variaveis->N);

  for (i=0; i < dados_variaveis->MAX_iTERACOES; i++){
    memcpy(xa, dados_variaveis->vetor_x, tam * sizeof(double));
    matriz_multi_vetor(dados_variaveis->matriz_ATrans, v, z, dados_variaveis->N);
    produto = vetores_produto(v,z,tam);

    if(produto != 0.0){
      w= rp/produto;
    }else{
      w= rp;
    }

    //mult_soma_vetor otimizado
    for(cont = 0; cont < tam; cont++){
      dados_variaveis->vetor_x[cont] = xa[cont] + (w * v[cont]);
      r[cont] = r[cont] + ((-w) * z[cont]);
    }

    matriz_multi_vetor(dados_variaveis->matriz_I,r,y,dados_variaveis->N);
    erro = calcula_erro(dados_variaveis->vetor_x,xa,dados_variaveis->N);
    fprintf(dados_variaveis->file, "# iter %d: %.15g \n",i,erro);
    if (dados_variaveis->E > erro){
      break;
    }
    aux = vetores_produto(y,r,tam);
    if(rp != 0){
      h = aux / rp;
    }else{
      h = aux;
    }
    rp = aux;
    mult_soma_vetor(v,y,v,h,tam);
  }
  if (dados_variaveis->MAX_iTERACOES != i ){
    dados_variaveis->MAX_iTERACOES = i;
  }

}

// ############ Programa Principal ############ //
int main(int argc, char **argv) {
  int i;
  int arg;
  char * caminho_arquivo = (char*) malloc (sizeof(char[2000]));

  Tempo *dados_tempos = (Tempo*) malloc (sizeof(Tempo));
  Dados *dados_variaveis = (Dados*) malloc (sizeof(Dados));

  //inicializa com zero para verificar filtragem de parametros
  dados_variaveis->residuo = 0.0;
  dados_variaveis->N = dados_variaveis->K = dados_variaveis->MAX_iTERACOES = 0;
  dados_variaveis->PC = dados_variaveis->E = -1.0;

  while((arg = getopt(argc, argv, "n:k:p:i:e:o:")) > 0){   //verifica os parametros, man getopt()

    if(arg == -1){
      fprintf(stderr, "Argumento não inválido: %s \n", argv);
    }

    switch (arg) {
      case 'n':
        dados_variaveis->N = strtol(optarg, NULL, 10); //converte numero em string de argv em long int
        break;
      case 'k':
        dados_variaveis->K = strtol(optarg, NULL, 10);
        break;
      case 'p':
        dados_variaveis->PC = strtod(optarg, NULL); //converte numero em string de argv em double
        break;
      case 'i':
        dados_variaveis->MAX_iTERACOES = strtol(optarg, NULL, 10);
        break;
      case 'e':
        dados_variaveis->E = strtod(optarg, NULL);
        break;
      case 'o':
          strcpy(caminho_arquivo,optarg); //copia endereço para ponteiro
        break;
      case ':':
          fprintf(stderr, "Faltou o argumento na opção %c \n", optopt);
        break;
      default:
        fprintf(stderr, "Argumento %s é desconhecido\n",  optopt);
    }
  }

  // if-else que verifica se valores são válidos, se não forem, encerra programa com erro
  if(dados_variaveis->N < 10){
    fprintf(stderr, "Dimensão N deve ser maior que 10. Encerrando o programa \n");
    exit(-1);
  }else if(dados_variaveis->N == 0){
    fprintf(stderr, "Dimensão N não informada. Encerrando o programa \n");
    exit(-1);
  }
  if(dados_variaveis->K < 1 || dados_variaveis->K % 2 == 0){
    fprintf(stderr, "K deve ser maior ou igual a 1 e impar\n");
    exit(-1);
  }else if(dados_variaveis->K == 0){
    fprintf(stderr, "Número de diagonais não informado. Encerrando o programa \n");
    exit(-1);
  }
  if(dados_variaveis->MAX_iTERACOES < 1){
    fprintf(stderr, "Maximo de iterações deve ser maior ou igual a 1. Encerrando o programa \n");
    exit(-1);
  }else if(dados_variaveis->MAX_iTERACOES == 0){
    fprintf(stderr, "Maximo de iterações não informado. Encerrando o programa \n");
    exit(-1);
  }
  if(dados_variaveis->PC < 0.0 || dados_variaveis->PC > 2.0){
    fprintf(stderr, "Pré-condionador invalido. Encerrando o programa \n");
    exit(-1);
  }else if(dados_variaveis->PC == -1.0){
    fprintf(stderr, "Pré-condionandor não informado. Encerrando o programa \n");
    exit(-1);
  }
  if(!caminho_arquivo){
    fprintf(stderr, "Nome do arquivo de saida não informado. Encerrando o programa\n");
    exit(-1);
  }

  dados_variaveis->file = fopen(caminho_arquivo,"w");
  if(!dados_variaveis->file){
    fprintf(stderr, "não foi possivel abrir o arquivo informado\n");
    exit(-1);
  }

  fprintf(dados_variaveis->file, "# apb14 Arianne de Paula Bortolan\n");

  //termina a inicialização e geracao de variaveis e dados
  inicializa(dados_tempos,dados_variaveis);
  LIKWID_MARKER_INIT;
  //gera os pre-condicionadores
  dados_tempos->Tempo_pc = timestamp();
  if(dados_variaveis->PC < 1.0 && dados_variaveis->PC != 0.0){
    gera_pc_jacobi(dados_variaveis);
  }else if (dados_variaveis->PC > 1.0){
    gera_pc_ssor(dados_variaveis);
  }
  dados_tempos->Tempo_pc = timestamp() - dados_tempos->Tempo_pc;

  //realiza o metodoGradiente
  LIKWID_MARKER_START("gradiente");
  dados_tempos->Tempo_iter = timestamp();
  gradiente_conjugado(dados_variaveis);
  dados_tempos->Tempo_iter = timestamp() - dados_tempos->Tempo_iter;
  LIKWID_MARKER_STOP("gradiente");

  //calcula residuo
  LIKWID_MARKER_START("residuo");
  dados_tempos->Tempo_residuo = timestamp();
  matriz_multi_vetor(dados_variaveis->matriz_A,dados_variaveis->vetor_b,dados_variaveis->vetor_r,dados_variaveis->N);
  mult_soma_vetor(dados_variaveis->vetor_r,dados_variaveis->vetor_r,dados_variaveis->vetor_b,-1, dados_variaveis->N);
  dados_variaveis->residuo = norma_euclidiana(dados_variaveis->vetor_r,dados_variaveis->N);
  dados_tempos->Tempo_residuo = timestamp() - dados_tempos->Tempo_residuo;

  dados_tempos->Tempo_iter = dados_tempos->Tempo_iter / dados_variaveis->MAX_iTERACOES;
  LIKWID_MARKER_STOP("residuo");
  //imprime resultados
  fprintf(dados_variaveis->file, "# residuo: %.15g\n",dados_variaveis->residuo);
  fprintf(dados_variaveis->file, "# Tempo PC: %.15g\n",dados_tempos->Tempo_pc);
  fprintf(dados_variaveis->file, "# Tempo iter: %.15g\n",dados_tempos->Tempo_iter);
  fprintf(dados_variaveis->file, "# Tempo residuos: %.15g\n",dados_tempos->Tempo_residuo);

  fprintf(dados_variaveis->file, "#\n%ld\n", dados_variaveis->N);
  for(i=0; i < dados_variaveis->N; i++) {
    fprintf(dados_variaveis->file, "%.15g ", dados_variaveis->vetor_x[i]);
  }
  fprintf(dados_variaveis->file, "\n");
  fclose(dados_variaveis->file);
  LIKWID_MARKER_CLOSE;
  return 0;
}
