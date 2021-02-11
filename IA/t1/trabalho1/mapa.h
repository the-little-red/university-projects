#ifndef __MAPA_H__
#define __MAPA_H__

typedef struct {
  int nlinhas;
  int ncolunas;
  int ncores;
  int lmax;
  int cmax;
  int **mapa;
} tmapa;

typedef struct {
  int l;
  int c;
  int v;
} tpos;

typedef struct {
  int tamanho;
  tpos *pos;
} tfronteira;

typedef struct {
  int solucao;
  int passos;
  int *cor;
} tplano;

void insere_plano(tplano*, int);
void remove_plano(tplano*);
void copia_plano(tplano*, tplano*);
tplano* aloca_plano(tmapa*);
void libera_plano(tplano*);
void carrega_plano(tplano*);
void mostra_plano(tplano*);
void simula_plano(tmapa*, tplano*);

tfronteira* aloca_fronteira(tmapa*);
void libera_fronteira(tfronteira*);
void mostra_fronteira(tfronteira *);

void copia_mapa(tmapa*, tmapa*);
tmapa* aloca_mapa(tmapa*);
void libera_mapa(tmapa*);
void gera_mapa(tmapa*, int);
void carrega_mapa(tmapa*);
void mostra_mapa(tmapa*);
void mostra_mapa_cor(tmapa*);
void pinta_mapa(tmapa*, int);

void fronteira_mapa(tmapa*, tfronteira*);

#endif
