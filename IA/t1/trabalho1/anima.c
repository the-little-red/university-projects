#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "mapa.h"

int main(int argc, char **argv) {
  int i;
  tmapa m;
  tplano *p;
  int intervalo;

  if(argc > 1)
    intervalo = atoi(argv[1]);
  else
    intervalo = 100;

  carrega_mapa(&m);
  p = aloca_plano(&m);
  carrega_plano(p);

  printf("\033c");
  mostra_mapa_cor(&m);
  for(i = 0; i < p->passos; i++) {
    usleep(1000 * intervalo);
    pinta_mapa(&m, p->cor[i]);
    printf("\033c");
    mostra_mapa_cor(&m);
  }    
  return 0;
}
