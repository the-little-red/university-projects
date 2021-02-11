#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {

  vertice v1,v2;
  grafo gf;
  grafo g = le_grafo(stdin);
  int d = diametro(g);

  if ( !g )
    return 1;

  printf("nome: %s\n", nome_grafo(g));
  printf("%d vértices\n", numero_vertices(g));
  printf("%d arestas\n", numero_arestas(g));
  printf("%s direcionado\n", direcionado(g) ? "" : "não ");
  printf("%sbipartido\n", bipartido(g) ? "" : "não ");
  printf("ordenacao_topologica:\n" );
  //v1 = ordenacao_topologica(g);
  printf("N comp %d\n",componentes_fortes(g,gf));

  if (d != infinito)
   printf("diâmetro = oo\n");
  else
   printf("diâmetro = %d\n", d);

  escreve_grafo(stdout, g);
  printf("concluido!\n");

  return ! destroi_grafo(g);
}
