#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"

int main(int argc, char **argv) {
  int i;
  tmapa m;
  tfronteira *f;
  tplano *p;

  carrega_mapa(&m);
  p = aloca_plano(&m);
  carrega_plano(p);
  f = aloca_fronteira(&m);

  for(i = 0; i < p->passos; i++) {
    pinta_mapa(&m, p->cor[i]);
    fronteira_mapa(&m, f);
  }    
  if(f->tamanho)
    return 1;
  return 0;
}
