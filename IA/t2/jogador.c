#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "campo.h"

#define MAXSTR 512


// Prepara variaveis para escolha da IA
void prepara_cenario(char buffer[], char campo[], int jogadores[], int *k, int *bolap){
  int i=0;

  if(buffer[2]=='n'){
    return;
  }else{
    *k=(int)buffer[1];
    for(i=0; buffer[i] != '\0'; i++){
      campo[i]=buffer[i+5];
      if(campo[i]=='f'){
        jogadores[i]=1;
      }else
        jogadores[i]=0;
      if(campo[i]=='o'){
        *bolap=i;
      }
    }
  }
}

// A escolha de jogadores de maneira é feito de maneira ALEATORIA,
// ele analisa a quantidade de jogadores proximo a região que o jogador faz gol
// no caso se for o jogador d, os jogadores serão posicionados na esquerda vice versa pro jogador E
void escolherd(char campo[], int jogadores[], int k, int bolap,char escolha[], int *chute, int *anterior){
  int jog=0;
  int possiveis[MAXSTR];
  char sub[10];
  int i;
  int j=0;
  int m=0;
  int njogadores=0;

  for(i=0; i<=MAXSTR;i++){
      possiveis[i] = -2;
    }

  // caso jogador_e a iteração é invertida
  for(i=bolap; i<=k ;i++){
    if(campo[i]=='.'){
      possiveis[j]=i;
      j++;
    }
    if(campo[i]=='f')
      njogadores++;

  }

  // todos os espaços ocupados pelos jogadores, devo chuta pra gol
  if(j==0){
    strcpy(escolha, "d o 1 ");
    sprintf(sub, "%d", -1);
    strcat(escolha,sub);
    return;
  }
  //somente uma posição livre, chuto pra lá
  if(j==1 && (chute==0)){
    sprintf(sub, "%d", possiveis[0]);
    strcpy(escolha, "d o 1 ");
    strcat(escolha,sub);
    *chute=1;
    return;
  }else
  //se mais da metade do campo contem jogadores, vou tenta chutar o mais longe possivel
  if((njogadores>(k/2)) && (chute==0)){
    for(j=0; possiveis[j] != (-2);j++);
    sprintf(sub, "%d ", j);
    strcpy(escolha, "d o ");
    strcat(escolha,sub);
    for(m=0; m<j;m++){
      sprintf(sub, "%d ", possiveis[m]);
      strcat(escolha,sub);
    }
    *chute=1;
    return;
  }
  // caso não entre em nenhum dos casos, posiciona jogador aleatoriamente é o padrão
  //recebe o indice de possiveis locais como máximo para numero aleatorio;
  *chute=0;

  jog= rand() % (k + 1);
  if(jog == *anterior)
    jog= rand() % (k + 1 - bolap) + bolap +1;
  strcpy(escolha, "d f ");
  sprintf(sub, "%d", jog);
  strcat(escolha,sub);
  *anterior=jog;
}


// A escolha de jogadores de maneira é feito de maneira ALEATORIA,
// ele analisa a quantidade de jogadores proximo a região que o jogador faz gol
// no caso se for o jogador d, os jogadores serão posicionados na esquerda vice versa pro jogador E
void escolhere(char campo[], int jogadores[], int k,int bolap,char escolha[], int *chute, int *anterior){
  int jog=0;
  int possiveis[MAXSTR];
  char sub[10];
  int i;
  int j=0;
  int m=0;
  int njogadores=0;

  for(i=0; i<=MAXSTR;i++){
      possiveis[i] = -2;
    }

  // caso jogador_e a iteração é invertida
  for(i=bolap; i<=k ;i++){
    if(campo[i]=='.'){
      possiveis[j]=i;
      j++;
    }
    if(campo[i]=='f')
      njogadores++;
  }
  // todos os espaços ocupados pelos jogadores, devo chuta pra gol
  if(j==0){
    strcpy(escolha, "e o 1");
    sprintf(sub, "%d", k+1);
    strcat(escolha,sub);
    return;
  }  //somente uma posição livre, chuto pra lá
  if(j==1 && (chute==0)){
    sprintf(sub, "%d", possiveis[0]);
    strcpy(escolha, "e o 1 ");
    strcat(escolha,sub);
    *chute=1;
    return;
  }
  //se mais da metade do campo contem jogadores, vou tenta chutar o mais longe possivel
  if((njogadores>(k/2)) && (chute==0)){
    for(j=0;possiveis[j]!=(-2);j++);
    sprintf(sub, "%d ", j);
    strcpy(escolha, "e o ");
    strcat(escolha,sub);
    for(m=0; m<j;m++){
      sprintf(sub, "%d ", possiveis[m]);
      strcat(escolha,sub);
      strcat(escolha," ");
    }
    *chute=1;
    return;
  }
  // caso não entre em nenhum dos casos, posiciona jogador aleatoriamente é o padrão
  //recebe o indice de possiveis locais como máximo para numero aleatorio;
  *chute=0;

  jog= rand() % (k + 1 - bolap) + bolap;
  if(jog == *anterior)
    jog= rand() % (k + 1 - bolap) + bolap +1;
  strcpy(escolha, "e f ");
  sprintf(sub, "%d", jog);
  strcat(escolha,sub);
  *anterior=jog;
}

int main(int argc,char *argv[]){
  char buf[MAXSTR];
  char campo[MAXSTR];
  int jogadores[MAXSTR];
  char escolha[MAXSTR];
  int k=0;
  int bolap=0;
  int chute=0;
  int anterior=0;
  campo_conecta(argc,argv);

  while(1){
    campo_recebe(buf);
    printf("%s", buf);
    prepara_cenario(buf,campo,jogadores,&k,&bolap);
    for(k=0; campo[k]!= ' '; k++);
    k=k-2;
    if(strcmp(argv[1],"d")==0){
      escolherd(campo,jogadores,k,bolap,escolha,&chute,&anterior);
    }else
      escolhere(campo,jogadores,k,bolap,escolha,&chute,&anterior);
    sprintf(buf, "%s\n", escolha);
    fprintf(stderr, "%s\n", escolha);
    campo_envia(buf);
  }

}
