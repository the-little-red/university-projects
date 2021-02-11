#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"

int main(int argc, char **argv) {
  int i;
  int cor;
  int ncor;
  int *ncorfront;
  tmapa m;
  tfronteira *f;
  tplano *p;

  carrega_mapa(&m);

  f = aloca_fronteira(&m);
  p = aloca_plano(&m);
  
  ncorfront = (int*) malloc((m.ncores + 1) * sizeof(int));
  cor = m.mapa[0][0];
  fronteira_mapa(&m, f);

  while(f->tamanho > 0) {
    for(i = 1; i <= m.ncores; i++)
      ncorfront[i] = 0;
    for(i = 0; i < f->tamanho; i++)
      ncorfront[m.mapa[f->pos[i].l][f->pos[i].c]]++;
    ncor = 0;
    for(i = 1; i <= m.ncores; i++)
      if(ncorfront[i] > ncor) {
	ncor = ncorfront[i];
	cor = i;
      }
    pinta_mapa(&m, cor);
    insere_plano(p, cor);
    fronteira_mapa(&m, f);
  }
  mostra_plano(p);
  
  return 0;
}
  
