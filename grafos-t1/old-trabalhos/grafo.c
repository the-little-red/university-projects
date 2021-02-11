//------------------------------------------------------------------------------
// Trabalho-2 2017/2 Grafos
// Arianne P. Bortolan - GRR20140220
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphviz/cgraph.h>
// #include "queue.h"
#include "grafo.h"

typedef struct aresta *aresta;

typedef struct queue *queue;

typedef struct node *node;

const int infinito = 0;

//------------------------------------------------------------------------------
// queue que guarda os grafos lidos para consulta

queue grafos_lidos;

//------------------------------------------------------------------------------
// Estruturas do grafo

struct aresta{
  vertice head,tail; //{u,v} da aresta
  long int peso;
  int visitado;
};

struct grafo{
  queue vertices_grafo;
  queue arestas_grafo;
  char *nome_graf;
  int direcio;
  int ponde;
  unsigned int n_vertice; //numero TOTAL de vertices do grafo
  unsigned int n_aresta; //numero TOTAL de arestas do grafo
};

struct vertice{ //vertice V
  queue vertice_entrada; //queue de vertices que arestas entra em V (caso nao direcionado NULL)
  queue vertice_saida; //queue de vertices que arestas sai de V (caso nao direcionado NULL)
  queue saida; //arestas de saida
  queue entrada; //arestas de entrada
  char *nome_vert;
  int n_arestas_ent; //numero total de arestas que entram em v
  int n_arestas_sai; //numero total de arestas que entram em v
  int visitado;
  int marcado;
};

//---------------------------------------------------------------------------
//Estruturas queue

struct node {
  void *content;
  node next;
};

struct queue{
  unsigned int tam;
  node first;
};

//---------------------------------------------------------------------------
// devolve o tamanho da queue

static unsigned int tam_queue(queue l){
  return l->tam;
}

//---------------------------------------------------------------------------
// devolve o first nodo da queue l,

static node first_node(queue l){
  return l->first;
}

//---------------------------------------------------------------------------
// devolve o conteúdo do nodo n

static void *content(node n){
  return n->content;
}

//---------------------------------------------------------------------------
// devolve o proximo nodo n

static node next_node(node n){
  return n->next;
}

//---------------------------------------------------------------------------
// cria uma queue vazia e a devolve
//
// devolve NULL em caso de falha

static queue constroi_queue(void) {
  queue l = malloc(sizeof(struct queue));

  if ( ! l )
    return NULL;

  l->first = NULL;
  l->tam = 0;

  return l;
}

//---------------------------------------------------------------------------
// desaloca a queue e todo seu content
// devolve 1 em caso de sucesso,
// ou 0 em caso de falha

static int destruct_queue(queue l, int destruct(void *)) {
  node p;
  int result;

  if(!l)
    return 0;

  while ( (p = first_node(l)) ) {

    l->first = next_node(p);

    if ( destruct )
      result &= destruct(content(p));

    free(p);
  }

  free(l);

  return result;
}

//---------------------------------------------------------------------------
// insere um novo nodo na queue com seu content

static node append_queue(void *content, queue l) {

  node novo = malloc(sizeof(struct node));

  if ( ! novo )
    return NULL;

  novo->content = content;
  novo->next = first_node(l);
  ++l->tam;

  return l->first = novo;
}

static int remove_node(struct queue *l, struct node *rnode, int destruct(void *)){
  int r = 1;
  if (l->first == rnode) {
    l->first = rnode->next;
    if (destruct != NULL) {
      r = destruct(rnode->content);
    }
    free(rnode);
    l->tam--;
    return r;
  }
  for (node n = first_node(l); n->next; n = next_node(n)) {
    if (n->next == rnode) {
      n->next = rnode->next;
      if (destruct != NULL)
         r = destruct(rnode->content);

      free(rnode);
      l->tam--;
      return r;
    }
  }
  return 0;
}

static int queue_remove(struct node *rnode, struct queue *l){
  int r = 1;
  if (l->first == rnode) {
    l->first = rnode->next;
    l->tam--;
    return r;
  }
  for (node n = first_node(l); n->next; n = next_node(n)) {
    if (n->next == rnode) {
      n->next = rnode->next;
      l->tam--;
      return r;
    }
  }
  return 0;
}

// Com base em uma aresta checa se o grafo é ponderado
static int checa_ponderado(Agraph_t *gt){
    char *peso = agget(agfstedge(gt, agfstnode(gt)), (char*)"weight");

    if(!peso)
      return 0;
    return 1;
}

//----------------------------------------------------------------------
// retorna o peso da aresta
static long int peso_aresta(aresta a){
  return a->peso;
}
//------------------------------------------------------------------------------
// devolve o nome do grafo g, caso o grafo nao exista, retorna NULL
char *nome_grafo(grafo g) {
  if(g != NULL)
    return g->nome_graf;
  else
    return NULL;
}


//------------------------------------------------------------------------------
// Compara se duas strings são iguais, usado para comparar nomes
static int compara_strings(char *a, char *b){
  int i=0;

  if (!a || !b){
    return 0;
  }

  do{
    if(a[i] != b[i])
      return 0;
    i++;
  }while(a[i] != '\0');
  return 1;
}

// Checa se dois vértices são iguais
static int checa_vertice(vertice v1, vertice v2){//se sao iguais
    if (!compara_strings(nome_vertice(v1),nome_vertice(v2)))
        return 0;
    return 1; //se sao iguais
}
//------------------------------------------------------------------------------
// Checa se duas arestas são iguais

static int checa_aresta(aresta a, aresta b){
  if(((a->tail == b->tail) && (a->head == b->head)) && (peso_aresta(a) == peso_aresta(b)))
    return 1;
  else
    return 0;
}

//---------------------------------------------------------------------------------zera visitados e marcados, garanto que todos os vertices estão como não visitados

static void zera_visita_marca(grafo g){
  node n;
  vertice v;

  for(n=first_node(g->vertices_grafo); n; n=next_node(n)){
    v=content(n);
    v->marcado = 0;
    v->visitado = 0;
  }
}

//------------------------------------------------------------------------------
// Retorna se uma aresta está no grafo ou não

static int busca_aresta(aresta a, grafo g){
  aresta arestaTemp;
  node n;

  for (n=first_node(g->arestas_grafo); n; n=next_node(n)){
    arestaTemp = content(n);
    if (checa_aresta(a, arestaTemp))
      return 1;
  }
    return 0;
}

//------------------------------------------------------------------------------
// devolve um grafo de nome s,
//         ou
//         NULL, caso não exista tal grafo
//primeiro verifica se ALGUM grafo existe, caso nao, retorna NULL

grafo grafo_nome(char *s) {
  int n;
  grafo g;
  node it;

  if(!grafos_lidos) //verifica se existe ALGUM grafo lido
    return NULL;

  g = content(grafos_lidos->first);
  it = grafos_lidos->first;
  for(n=1; n <= tam_queue(grafos_lidos); n++){
    it = next_node(it);
    if(compara_strings(g->nome_graf,s))
      return g;
    g = content(it);
  }
  return NULL;
}

//------------------------------------------------------------------------------
// devolve 1, se g é direcionado,
//         ou
//         0, caso contrário

int direcionado(grafo g) {
  if(g != NULL)
    return g->direcio;
  else
    return 0;
}


//------------------------------------------------------------------------------
// devolve 1, se g é ponderado,
//         ou
//         0, caso contrário
//
// o grafo g é ponderado se todas as suas arestas/arcos tem um
// atributo de nome "weight" cujo valor é um número.

int ponderado(grafo g) {
  if(g != NULL)
    return g->ponde;
  else
    return 0;
}

//------------------------------------------------------------------------------
// devolve o número de vértices do grafo g

unsigned int numero_vertices(grafo g) {
  if(g != NULL)
    return g->n_vertice;
  else
    return 0;
}

//------------------------------------------------------------------------------
// devolve o número de arestas/arcos do grafo g

unsigned int numero_arestas(grafo g) {
  if(g != NULL)
    return g->n_aresta;
  else
    return 0;
}

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
//
// devolve 1 em caso de sucesso,
//         ou
//         0, caso contrário

int destroi_grafo(grafo g) {
  if(!g)
    return 0;
  if(!grafos_lidos)
    return 0;
  free(g);
  return 1;
}

/////// Funções de verificação de peso
//Se Agedge tem peso retorna seu peso, caso contrário retorna 0
static int acha_peso(Agedge_t *at){
    char *peso = agget(at, (char*)"weight");
    if(!peso)
        return(0);
    return(atoi(peso));
}

//// Funções de busca para ordenação e componentes

static void marca_vertice(vertice v){
    v->marcado = 1;
}

static int marcado(vertice v){
    return v->marcado;
}

//------------------------------------------------------------------------------
// Guarda as arestas/arcos no grafo e os apontadores para os vértices

static void le_arestas(Agraph_t *graph, grafo g, Agnode_t *vert) {
  aresta a;
  Agedge_t *at;
  vertice v;
  node n;

  for(at=agfstedge(graph, vert); at; at=agnxtedge(graph, at, vert)){
    if(!direcionado(g))
      if (aghead(at) == vert) continue; //verificação pra casos não direcionados
    a = (aresta)malloc(sizeof(struct aresta));
    a->peso=0;
    if(ponderado(g))
      a->peso = acha_peso(at);
    for(n = first_node(g->vertices_grafo); n; n=next_node(n)){
      v = content(n);

      if(at &&compara_strings(nome_vertice(v), agnameof(agtail(at)))){
        a->tail = v;
        append_queue(a,v->saida);
        v->n_arestas_sai++;
      }

      if(at &&compara_strings(nome_vertice(v), agnameof(aghead(at)))){
        a->head = v;
        append_queue(a,v->entrada);
        v->n_arestas_ent++;
      }
    }
    if (!busca_aresta(a, g)){
      append_queue(a,g->arestas_grafo);
    }
  }
}

//------------------------------------------------------------------------------
// Guarda os vértices no grafo
static void le_vertices(Agraph_t *graph, grafo g) {
  vertice v;
  Agnode_t *vert;

  for (vert = agfstnode(graph); vert; vert = agnxtnode(graph,vert)){

    v = (vertice)malloc(sizeof(struct vertice));
    v->nome_vert = agnameof(vert);
    v->n_arestas_ent=0;
    v->n_arestas_sai=0;
    v->visitado=0;
    v->marcado=0;
    v->entrada=constroi_queue();
    v->saida=constroi_queue();
    v->vertice_entrada=constroi_queue();
    v->vertice_saida=constroi_queue();
    append_queue(v,g->vertices_grafo);
  }
}

//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot.
//
// devolve o grafo escrito,
//         ou
//         NULL, em caso de erro

static void imprime_arestas(grafo g){
  node n;
  aresta arest;

  for(n=first_node(g->arestas_grafo); n; n=next_node(n)){
    arest=content(n);
    if ((arest->head != NULL) && (arest->tail != NULL)){
      printf(" %s", nome_vertice(arest->tail));
      printf(" -%s", direcionado(g)?">":"-");
      printf(" %s;\n", nome_vertice(arest->head));
      if(ponderado(g))
          printf(" [peso = %ld]", peso_aresta(arest));
      printf(";\n");
    }
  }
  printf("}\n");
}

static void imprime_vertices(grafo g){
  node n;
  vertice v;

  for(n=first_node(g->vertices_grafo); n; n=next_node(n)){
    v = content(n);
    if(direcionado(g))
      printf(" %s -- Grau: Saída[%i] - Entrada[%i]\n", nome_vertice(v), v->n_arestas_sai, v->n_arestas_ent);
    else
      printf(" %s -- Grau: [%i]\n", nome_vertice(v), (v->n_arestas_sai + v->n_arestas_ent));
  }
}

grafo escreve_grafo(FILE *output, grafo g) {
  fprintf(output, "%sgraph %s{\n", direcionado(g)?"di":"", nome_grafo(g));
  imprime_arestas(g);
  imprime_vertices(g);
  return(g);
}

//------------------------------------------------------------------------------
// devolve o nome do vertice v

char *nome_vertice(vertice v){
  if(v != NULL)
    return v->nome_vert;
  else
    return NULL;
}

//------------------------------------------------------------------------------
// devolve um vertice de nome s no grafo g,
//         ou
//         NULL caso não exista em g um vertice de nome s

vertice vertice_nome(char *s, grafo g) {
  int n;
  vertice vert;
  node nod;

  if(!g) //verifica se existe o grafo
    return NULL;

  if(g->n_vertice <= 0) //verifica se grafo tem vertices
    return NULL;

  vert = content(g->vertices_grafo->first);
  nod = g->vertices_grafo->first;
  for(n=1; n <= tam_queue(g->vertices_grafo); n++){
    nod = next_node(nod);
    if (compara_strings(nome_vertice(vert), s))
        return(vert);
    vert=content(nod);
  }
  return NULL;
}

//------------------------------------------------------------------------------
// devolve o grau do vértice v no grafo g
//
// se g é direcionado, e
//                       direcao =  1, o grau devolvido é o de saída
//                       direcao = -1, o grai devolvido é o de entrada
//
// caso contrário o valor de direcao é ignorado.

unsigned int grau(vertice v, int direcao, grafo g) {
  vertice vert;

  if(!direcionado(g)){
    return 0;
  }
  if(!g){
    return 0;
  }
  if(!v){
    return 0;
  }

  vert = vertice_nome(nome_vertice(v),g);

  if(direcao == 1){
    return vert->n_arestas_sai;
  }
  if(direcao == -1){
    return vert->n_arestas_ent;
  }

  return 0;
}

// //------------------------------------------------------------------------------
// devolve o "primeiro" vizinho de v em g,
//         ou
//         NULL se v é vértice isolado em g
//
// se g é direcionado, e
//                       direcao =  1, o vizinho devolvido é de saída
//                       direcao = -1, o vizinho devolvido é de entrada
//
// caso contrário o valor de direcao é ignorado.
//

vertice primeiro_vizinho(vertice v, int direcao, grafo g){
  vertice vert = vertice_nome(nome_vertice(v),g);

  if(!v)
    return NULL;
  if(!g)
    return NULL;
  if(g->n_vertice <= 1)
    return NULL;
  if(!vert)
      return NULL;

  if(direcionado(g) && direcao == -1){ //retorna o de entrada caso direcao -1 e direcionado
    return content(next_node(vert->vertice_entrada->first));
  }

  return content(next_node(vert->vertice_saida->first)); //por default retorna primeiro vertice que de saida como primeiro vizinho
}

//------------------------------------------------------------------------------
// devolve o "próximo" vizinho de v em g após u,
//         ou
//         NULL se u é o "último" vizinho de v em g
//
// se g é direcionado, e
//                       direcao =  1, o vizinho devolvido é de saída
//                       direcao = -1, o vizinho devolvido é de entrada
//
// caso contrário o valor de direcao é ignorado.

vertice proximo_vizinho(vertice u, vertice v, int direcao, grafo g) {
  u = primeiro_vizinho(v,direcao,g);
  if(!u)
    return NULL;
  else
    return primeiro_vizinho(u,direcao,g);
}

//------------------------------------------------------------------------------
// devolve 1, se v é um vértice simplicial em g,
//         ou
//         0, caso contrário
//
// Um vértice é simplicial no grafo se sua vizinhança é uma clique
// Nessa função é verificado o tamanho da menor intercessão entre a vizinhaça de um vertice v e seus vizinhos, esse tamanho deve ser v -1 para que v seja simplicial (optado por fazer dessa maneira e não via clique devido a simplicidade dessa verificação)

// Retorna a intercessao entre duas queues de vertices
static queue intercessao(queue l1, queue l2){
  queue intercessao;
  node n1, n2;
  vertice v1, v2;
  intercessao = constroi_queue();

  for(n1=first_node(l1); n1; n1=next_node(n1)){
    v1 = content(n1);
    for(n2=first_node(l2); n2; n2=next_node(n2)){
      v2 = content(n2);
      if(compara_strings(v1->nome_vert, v2->nome_vert)){
        append_queue(v2, intercessao);
        break; // se já encontrou, sai do laço
      }
    }
  }
  return intercessao;
}

int simplicial(vertice v, grafo g) {
  vertice vert = vertice_nome(nome_vertice(v),g); //verifico se v pertence a g
  node n;
  queue iguais;
  unsigned int tam1, tam2;

  if(!vert)
    return -1;

  tam1 = tam_queue(v->vertice_entrada);
  tam2 = tam_queue(v->vertice_saida);

  for(n=first_node(v->vertice_entrada);n; next_node(n)){
    vert=content(n);
    iguais = intercessao(v->vertice_entrada,vert->vertice_entrada);
    if(tam1 !=tam_queue(iguais))
      return 0;
  }
  for(n=first_node(v->vertice_saida);n; next_node(n)){
    vert=content(n);
    iguais = intercessao(v->vertice_saida,vert->vertice_saida);
    if(tam2 != tam_queue(iguais))
      return 0;
  }
  return 1;
}

// Retorna se vertice esta na queue vértices ou não
static int busca_vertice(vertice v, queue l){
    node n;
    vertice vn;
    for(n=first_node(l); n; n=next_node(n)){
        vn = content(n);
        if (compara_strings(nome_vertice(vn), nome_vertice(v)))
            return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
// devolve 1, se g é um grafo bipartido,
//         ou
//         0, caso contrário
//verifica bipartição por clique

int bipartido(grafo g) {
  node n1,n2;
  vertice v1,v2;
  for (n1=first_node(g->vertices_grafo); n1; n1=next_node(n1)){
      v1=content(n1);
      for (n2=first_node(g->vertices_grafo); n2; n2=next_node(n2)){
          v2=content(n2);
          if (!compara_strings(v1->nome_vert, v2->nome_vert)){
              if (busca_vertice(v2, v2->vertice_entrada))
                  return 0;
          }
      }
  }
  return 1;
}

//------------------------------------------------------------------------------
// FUNCÕES QUE PROCURAM O CAMINHO MINIMO EM UM GRAFO

static long int distanciaVizinhos (vertice u, vertice v, grafo g) {
	node n;
	aresta aAux;

	if (u == v)
		return 0;
	if (!g->direcio) {
		for (n=first_node(u->saida); n; n=next_node(n)) {
			aAux = content(n);
			if (compara_strings(nome_vertice(u), nome_vertice(aAux->head))) {
				if (compara_strings(nome_vertice(v), nome_vertice(aAux->tail))) {
					if (g->ponde)
						return aAux->peso;
					else
						return 1;
				}
			}
			if (compara_strings(nome_vertice(u), nome_vertice(aAux->tail))) {
				if (compara_strings(nome_vertice(v), nome_vertice(aAux->head))) {
					if (g->ponde)
						return aAux->peso;
					else
						return 1;
				}
			}
		}

  	for (n=first_node(u->entrada); n; n=next_node(n)) {
  		aAux = content(n);
  		if (compara_strings(nome_vertice(u), nome_vertice(aAux->head))) {
  			if (compara_strings(nome_vertice(v), nome_vertice(aAux->tail))) {
  				if (g->ponde)
  					return aAux->peso;
  				else
  					return 1;
  			}
  		}
  		if (compara_strings(nome_vertice(u), nome_vertice(aAux->tail))) {
  			if (compara_strings(nome_vertice(v), nome_vertice(aAux->head))) {
  				if (g->ponde)
  					return aAux->peso;
  				else
  					return 1;
  			}
  		}
  	}
		return infinito;
	}
	else {
		for (n=first_node(u->saida); n; n=next_node(n)) {
			aAux = content(n);
			if (compara_strings(nome_vertice(v), nome_vertice(aAux->head))) {
				if (g->ponde)
					return aAux->peso;
				else
					return 1;
			}
		}
		return infinito;
	}
}

static unsigned int indice(vertice v, grafo g) {
	unsigned int i;
	node n;

	for (n=first_node(g->vertices_grafo), i=0; n; n=next_node(n), i=i+1) {
		vertice vAux = content(n);
		if (strcmp(nome_vertice(v), nome_vertice(vAux)) == 0) {
			break;
		}
	}
	return i;
}

// Caminho minimo de u a v em g utilizando o algoritmo de Floyd-Warshall
static queue cam_min(vertice u, vertice v, grafo g) {
	node n1, n2, n3;
	vertice vAux1, vAux2, vAux3;
	long int **dist;
	long int alt;
	vertice **next;
	int i;
	queue caminho = constroi_queue();

	// Alocacao das matrizes
	dist = malloc(sizeof(long int) * g->n_vertice);
	next = (vertice**)malloc(sizeof(vertice*) * g->n_vertice);
	for (i = 0; i < g->n_vertice; ++i) {
		dist[i] = malloc(sizeof(long int) * g->n_vertice);
		next[i] = (vertice*)malloc(sizeof(vertice) * g->n_vertice);
	}

	// Seta as distancias entre u e v em infinito, caso eles nao sejam vizinhos, ou com o peso do vertice (u, v), caso eles sejam vizinhos
	for (n1=first_node(g->vertices_grafo); n1; n1=next_node(n1)) {
		for (n2=first_node(g->vertices_grafo); n2; n2=next_node(n2)) {
			vAux1 = content(n1);
			vAux2 = content(n2);
			dist[indice(vAux1, g)][indice(vAux2, g)] = distanciaVizinhos(vAux1, vAux2, g);
			next[indice(vAux1, g)][indice(vAux2, g)] = vAux2;
		}
	}

	// Loop principal do algoritmo
	for (n1=first_node(g->vertices_grafo); n1; n1=next_node(n1)) {
		for (n2=first_node(g->vertices_grafo); n2; n2=next_node(n2)) {
			for (n3=first_node(g->vertices_grafo); n3; n3=next_node(n3)) {
				vAux1 = content(n1);
				vAux2 = content(n2);
				vAux3 = content(n3);

				if (dist[indice(vAux2, g)][indice(vAux1, g)] == infinito || dist[indice(vAux1, g)][indice(vAux3, g)] == infinito)
					alt = infinito;
				else
					alt = dist[indice(vAux2, g)][indice(vAux1, g)] + dist[indice(vAux1, g)][indice(vAux3, g)];
				if (alt < dist[indice(vAux2, g)][indice(vAux3, g)]) {
					dist[indice(vAux2, g)][indice(vAux3, g)] = dist[indice(vAux2, g)][indice(vAux1, g)] + dist[indice(vAux1, g)][indice(vAux3, g)];
					next[indice(vAux2, g)][indice(vAux3, g)] = next[indice(vAux2, g)][indice(vAux1, g)];
				}
			}
		}
	}

	// Se v nao e alcancavel a partie de u, retorna uma queue vazia
	if (dist[indice(u, g)][indice(v, g)] == infinito)
		return caminho;

	// Reconstrucao do caminho
	append_queue(u, caminho);
	while (u != v) {
		append_queue(u, caminho);
	}
  u = next[indice(u, g)][indice(v, g)];

	// A queue do caminho esta invertida, entao tenho que fazer uma copia dela ao contrario
	queue caminhoCopia = constroi_queue();
	for (n1=first_node(caminho), i = 0; n1; n1=next_node(n1), ++i) {
		vAux1 = content(n1);
		append_queue(vAux1, caminhoCopia);
	}

	// Se o caminho tem apenas um elemento significa que v=u
	// Neste caso a funcao retorna uma queue vazia
	if (i == 1) {
		queue vazia = constroi_queue();
		return vazia;
	}

	return caminhoCopia;
}

// Executa o algoritmo de Floyd-Warshall e retorna a matriz de distancias
static long int **distancias(long int **d, grafo g) {
	node n1, n2, n3;
	vertice vAux1, vAux2, vAux3;
	long int alt;

	// Seta as distancias entre u e v em infinito, caso eles nao sejam vizinhos, ou com o peso do vertice (u, v), caso eles sejam vizinhos
	for (n1=first_node(g->vertices_grafo); n1; n1=next_node(n1)) {
		for (n2=first_node(g->vertices_grafo); n2; n2=next_node(n2)) {
			vAux1 = content(n1);
			vAux2 = content(n2);
			d[indice(vAux1, g)][indice(vAux2, g)] = distanciaVizinhos(vAux1, vAux2, g);
		}
	}

	// Loop principal do algoritmo
	for (n1=first_node(g->vertices_grafo); n1; n1=next_node(n1)) {
		for (n2=first_node(g->vertices_grafo); n2; n2=next_node(n2)) {
			for (n3=first_node(g->vertices_grafo); n3; n3=next_node(n3)) {
				vAux1 = content(n1);
				vAux2 = content(n2);
				vAux3 = content(n3);

				if (d[indice(vAux2, g)][indice(vAux1, g)] == infinito || d[indice(vAux1, g)][indice(vAux3, g)] == infinito)
					alt = infinito;
				else
					alt = d[indice(vAux2, g)][indice(vAux1, g)] + d[indice(vAux1, g)][indice(vAux3, g)];
				if (alt < d[indice(vAux2, g)][indice(vAux3, g)])
					d[indice(vAux2, g)][indice(vAux3, g)] = d[indice(vAux2, g)][indice(vAux1, g)] + d[indice(vAux1, g)][indice(vAux3, g)];
			}
		}
	}
	return d;
}

int diametro(grafo g) {
  long int **mDistancias;
	long int d = 0;
	int i, j;

	mDistancias = malloc(sizeof(long int) * g->n_vertice);
	for (i = 0; i < g->n_vertice; ++i)
		mDistancias[i] = malloc(sizeof(long int) * g->n_vertice);

	mDistancias = distancias(mDistancias, g);

	for (i = 0; i < g->n_vertice; ++i) {
		for (j = 0; j < g->n_vertice; ++j) {
			if (mDistancias[i][j] > d)
				d = mDistancias[i][j];
		}
	}
	return (int) d;
}
//------------------------------------------------------------------------------
// devolve em c um caminho (direcionado) mínimo de u a v no grafo
//              (direcionado e/ou ponderado) g, de forma que
//
//                  c[0]=u, ..., c[n]=v, onde n é o tamanho do caminho
//
//
//              ou
//
//              NULL se não existe tal caminho,
//
// em qualquer caso, devolve a distância de u a v em distancia e
// devolve o tamanho do caminho como valor da função
int caminho_minimo(vertice *c, float *distancia, vertice u, vertice v, grafo g){
  node n;
  vertice cont;
  int j=0;
  queue caminho = constroi_queue();

  *distancia = (float) distanciaVizinhos(u,v,g);
  caminho = cam_min(u,v,g);
  c = (vertice*) malloc ((sizeof(vertice*)) * (tam_queue(caminho)));

  for(n=first_node(caminho);n;n=next_node(n)){
    cont= content(n);
    c[j]=cont;
    j++;
  }
  return tam_queue(caminho);
}


//------------------------------------------------------------------------------
// lê um grafo no formato dot de input
//
// devolve o grafo lido,
//         ou
//         NULL, em caso de erro

grafo le_grafo(FILE *input) {
  node n,m,l;
  vertice v;
  int j;
  aresta a;
  Agraph_t *gf = agread(input, NULL);
  Agnode_t *vet;
  grafo g = (grafo)malloc(sizeof(struct grafo));

  if (!g)
    return NULL;

  g->nome_graf = agnameof(gf);
  g->direcio = agisdirected(gf);
  g->n_vertice = agnnodes(gf);
  g->n_aresta = agnedges(gf);
  g->vertices_grafo = constroi_queue();
  g->arestas_grafo = constroi_queue();
  g->ponde = checa_ponderado(gf);
  le_vertices(gf, g);

  for (vet=agfstnode(gf); vet; vet=agnxtnode(gf,vet))
    le_arestas(gf,g,vet);

  for(n=first_node(g->vertices_grafo);n;n=next_node(n)){
    v=content(n);

    m =first_node(v->saida);
    for(j=1; j<= (tam_queue(v->saida)); j++){
      a=content(m);
      if(!compara_strings(nome_vertice(a->tail), nome_vertice(v)))
        append_queue(a->tail,v->vertice_saida);
      else
        append_queue(a->head,v->vertice_saida);
      m=next_node(m);
    }
    m =first_node(v->entrada);
    for(j=1; j<= (tam_queue(v->entrada)); j++){
      a=content(m);
      if(!compara_strings(nome_vertice(a->head), nome_vertice(v)))
        append_queue(a->head,v->vertice_saida);
      else
        append_qu eue(a->tail,v->vertice_saida);
      m=next_node(m);
    }
  }
  grafos_lidos = constroi_queue();
  append_queue(g,grafos_lidos);

  agclose(gf);
  return g;
}

//------------------------------------------------------------------------------ Funções de Cópia do grafo

static void copia_atributos(grafo g, grafo gn){
  strcpy(gn->nome_graf, nome_grafo(g));
  gn->n_vertice = g->n_vertice;
  gn->n_aresta = g->n_aresta;
  gn->ponde = ponderado(g);
  gn->direcio = direcionado(g);
}

// copia vertices de maneira invertida
static queue copia_vertices(queue vertices_g){
  queue l = constroi_queue();
  node n;
  vertice v, vn;

  for(n=first_node(vertices_g); n; n=next_node(n)){
    v = content(n);
    vn->nome_vert = nome_vertice(v);
    vn->n_arestas_ent = v->n_arestas_sai ;
    vn->n_arestas_sai = v->n_arestas_ent;
    vn->entrada = constroi_queue();
    vn->saida = constroi_queue();
    append_queue(vn, l);
  }
  return l;
}

//copia_vertices na ordem normal
static queue copia_verticesok(queue vertices_g){
  queue l = constroi_queue();
  node n;
  vertice v, vn;

  for(n=first_node(vertices_g); n; n=next_node(n)){
    v = content(n);
    vn->nome_vert = nome_vertice(v);
    vn->n_arestas_ent = v->n_arestas_ent;
    vn->n_arestas_sai = v->n_arestas_sai;
    vn->entrada = constroi_queue();
    vn->saida = constroi_queue();
    append_queue(vn, l);
  }
  return l;
}

// Copia as arestas de um grafo para outro de maneira invertida
static void copia_arestas(grafo g, grafo gn){
  node n,m;
  aresta a, an;
  gn->arestas_grafo = constroi_queue();
  m = first_node(gn->vertices_grafo);

  for(n=first_node(g->arestas_grafo); n; n=next_node(n)){
    a = content(n);
    an->peso = peso_aresta(a);
    an->tail = vertice_nome(a->head->nome_vert,content(m));
    an->head = vertice_nome(a->tail->nome_vert,content(m));
    append_queue(an, gn->arestas_grafo);
    m=next_node(m);
  }
}

// Configura os apontadores do grafo que está sendo criado de maneira invertida
static void configura_apontadores(grafo gn){
  node n1, n2;
  vertice v;
  aresta a;

  for(n1=first_node(gn->arestas_grafo); n1; n1=next_node(n1)){
    a = content(n1);
    for(n2=first_node(gn->vertices_grafo); n2; n2=next_node(n2)){
      v = content(n2);
      if(compara_strings(nome_vertice(a->head), nome_vertice(v)))
        append_queue(a, v->saida);
      if(compara_strings(nome_vertice(a->tail), nome_vertice(v))){
        append_queue(a, v->entrada);
      }
    }
  }
}

// Copia o grafo e devolve uma copia transposta dele
static grafo transpor_grafo(grafo g){
  grafo gn;
  gn = (grafo)malloc(sizeof(struct grafo));
  copia_atributos(g, gn);
  gn->vertices_grafo = copia_vertices(g->vertices_grafo);
  copia_arestas(g, gn);
  configura_apontadores(gn);
  return gn;
}

//realiza a busca pelos componentes retornando uma lista de vertices que formam um grafo
static queue busca_comp(vertice v, queue componente){
  node n;
  vertice v2;

  if(v->visitado == 0){

    v->visitado = 1;
    append_queue(v,componente);

    if ((v->n_arestas_sai != 0)){
      for(n=first_node(v->vertice_saida);n; n=next_node(n)){
        v2=content(n);
        componente = busca_comp(v2,componente);
      }
    }

    if (v->n_arestas_ent != 0){
      for(n=first_node(v->vertice_saida);n; n=next_node(n)){
        v2=content(n);
        componente = busca_comp(v2,componente);
      }
    }
  }
    return componente;
}
//------------------------------------------------------------------------------
// devolve o número de componentes fortes de g,
//         e um vetor de grafos com os componentes fortes de g em componentes

//pega os componentes que estão em vertices e os coloca em um grafo
static grafo cria_grafo(queue componente,grafo gt){
  node n,m;
  aresta a,an;
  vertice v;
  grafo g = (grafo)malloc(sizeof(struct grafo));

  if (!g)
    return NULL;

  copia_atributos(g,gt);
  for(n=first_node(componente);n;next_node(n)){
    g->vertices_grafo= copia_verticesok(componente);
  }
  g->n_vertice=tam_queue(g->vertices_grafo);

  return g;
}

int componentes_fortes(grafo g, grafo *componentes){
  grafo gtrans;
  node n;
  vertice v,vt;
  int j, tam;
  queue componente = constroi_queue();
  queue compt = constroi_queue();
  queue listacomponentes = constroi_queue();

  zera_visita_marca(g);

  n=first_node(g->vertices_grafo); //recebe raiz v de g
  v=content(n);
  if(v->visitado == 0){
    componente = busca_comp(v,componente);
  }

  gtrans=transpor_grafo(g);

  zera_visita_marca(gtrans);

  for(n=first_node(componente);n;n=next_node(n)){
    v=content(n);
    vt=vertice_nome(nome_vertice(v),gtrans);

    if(vt->visitado == 0){
      compt = busca_comp(vt,compt);
      append_queue(compt,listacomponentes);
      compt = constroi_queue();
    }
  }

  tam = tam_queue(listacomponentes);
  // componentes[] = (grafo*) malloc ((sizeof(grafo))*tam);
  n=first_node(listacomponentes);
  printf("morroaqui\n" );
  for(j=0; j< (tam); j++){
    componente = content(n);
    componentes[j] = (grafo*) malloc (sizeof(grafo));
    componentes[j]=cria_grafo(componente,gtrans);
    n=next_node(n);
  }
  return tam;

}

//------------------------------------------------------------------------------
// devolve um vetor de numero_vertices(g) vertices com uma ordenação
//         topológica de g
//         ou
//         NULL, caso g seja cíclico

vertice *ordenacao_topologica(grafo g){
  int i,j,k;
  node n,m,lm, guarda;
  vertice v,a,b;
  vertice* c;
  int tem_ciclo = 0;
  c = NULL;
  queue cl1 = constroi_queue();
  queue cl2 = constroi_queue(); //casos de ciclo;
  queue nvisitados = constroi_queue();
  queue percorrer = constroi_queue();
  queue ordem = constroi_queue();

  zera_visita_marca(g);

  for(n=first_node(g->vertices_grafo); n; n=next_node(n)){
    v = content(n);
    append_queue(v,nvisitados);
  }

  v = content(first_node(nvisitados));
  guarda = first_node(nvisitados);

  while((tam_queue(ordem) <= tam_queue(g->vertices_grafo)) || (tem_ciclo == 0)){

    if(v->visitado == 0)
      v->visitado = 1;
    else
    if(v->visitado == 1){
      marca_vertice(v);
      append_queue(v,ordem);
      queue_remove(guarda,nvisitados);
    }
    //checar ciclos
    if((v->n_arestas_sai > 0) && (v->n_arestas_ent > 0)){
      lm=first_node(v->vertice_saida);
      for(i=1; i<= (tam_queue(v->vertice_saida)); i++){
        a=content(lm);
        append_queue(a,cl1);
        lm=next_node(lm);
      }

      lm=first_node(v->vertice_entrada);
      for(i=1; i<= (tam_queue(v->vertice_entrada)); i++){
        a=content(lm);
        append_queue(a,cl2);
        lm=next_node(lm);
      }

      lm = first_node(cl1);
      m = first_node(cl2);
      for(j=1; j >= v->n_arestas_ent;j++){
        for(k=1; k >= v->n_arestas_sai; k++){
          a = content(lm);
          b = content(m);
          if(checa_vertice(a,b)){
            tem_ciclo = 1;
            return NULL;
          }
          lm= next_node(lm);
        }
        m=next_node(m);
      }
    }
    if(v->n_arestas_sai == 0){
      marca_vertice(v);
      append_queue(v,ordem);
      queue_remove(guarda,nvisitados);
    }

    for(m=first_node(v->vertice_saida);m;m=next_node(m)){
      a=content(m);
      append_queue(a,percorrer);
    }
    if(tam_queue(percorrer) > 0){
      b = content(first_node(percorrer));
      guarda = first_node(percorrer);
      queue_remove(first_node(percorrer),percorrer);
    }else{
      b = content(first_node(nvisitados));
      guarda = first_node(nvisitados);
    }
  }
  j=0;
  for(n=first_node(ordem); n; n=next_node(n)){
    c[j]=content(n);
    j++;
  }
  return c;
}
