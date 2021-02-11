//------------------------------------------------------------------------------
// Trabalho-1 2018/1 Grafos
// Arianne P. Bortolan - GRR20140220
// Matheus C Horstmann - GRR20158600
// Este trabalho contém funções extras para talvez um uso futuro.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

typedef struct aresta *aresta;

typedef struct queue *queue;

typedef struct node *node;

queue grafos_lidos;

//------------------------------------------------------------------------------
// Estruturas do grafo

struct aresta {
    vertice head,tail; //{u,v} da aresta
    long int peso;
};

struct grafo {
    queue vertices_grafo;
    queue arestas_grafo;
    queue produtos;
    queue consumidor;
    grafo grafo_recomendado; //grafo de recomendação do grafo principal
    char *nome_graf;
    int direcio;
    int ponde;
    int tipado;
    unsigned int n_vertice; //numero TOTAL de vertices do grafo
    unsigned int n_aresta; //numero TOTAL de arestas do grafo
};

struct vertice { //vertice V
    queue vertice_entrada; //queue de vertices que arestas entra em V
    queue vertice_saida; //queue de vertices que arestas sai de V
    queue saida; //arestas de saida
    queue entrada; //arestas de entrada
    char* tipo;
    char *nome_vert;
    int n_arestas_ent; //numero total de arestas que entram em v
    int n_arestas_sai; //numero total de arestas que entram em v
};
//---------------------------------------------------------------------------
//Estruturas de lista/queue

struct node {
    void *content;
    node next;
};

struct queue {
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

    if ( !l )
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

    if ( !novo )
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

//------------------------------------------------------------------------------
// Com base em uma aresta checa se o grafo é ponderado
static int checa_ponderado(Agraph_t *gt){
    char *peso = agget(agfstedge(gt, agfstnode(gt)), (char*)"weight");

    if ( !peso ) {
        return 0;
    }
    return 1;
}

//------------------------------------------------------------------------------
// Com base em um vertice checa se o grafo é tipado
static int checa_tipado(Agraph_t *gt){
    char *tipo = agget(agfstnode(gt), (char*)"tipo");

    if ( !tipo ) {
        return 0;
    }
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
    if ( g != NULL ) {
        return g->nome_graf;
    }else{
        return NULL;
    }
}

//------------------------------------------------------------------------------
// Compara se duas strings são iguais, usado para comparar nomes

static int compara_strings(char *a, char *b){
    int i=0;

    if (!a || !b) {
        return 0;
    }

    do {
        if(a[i] != b[i])
            return 0;
        i++;
    } while(a[i] != '\0');
    return 1;
}

//------------------------------------------------------------------------------
// Checa se dois vértices são iguais

static int checa_vertice(vertice v1, vertice v2){
    if (!compara_strings(nome_vertice(v1),nome_vertice(v2))) {
        return 0;
    }
    return 1;
}

//------------------------------------------------------------------------------
// Checa se duas arestas são iguais

static int checa_aresta(aresta a, aresta b){
    if(((a->tail == b->tail) && (a->head == b->head)) && (peso_aresta(a) == peso_aresta(b))) {
        return 1;
    }else{
        return 0;
    }
}

//---------------------------------------------------------------------------------
// zera visitados e marcados, garanto que todos os vertices estão como não visitados

// static void zera_visita_marca(grafo g){
//     node n;
//     vertice v;
//
//     for(n=first_node(g->vertices_grafo); n; n=next_node(n)) {
//         v=content(n);
//         v->marcado = 0;
//         v->visitado = 0;
//     }
// }

//------------------------------------------------------------------------------
// Retorna se uma aresta está no grafo ou não

static int busca_aresta(aresta a, grafo g){
    aresta arestaTemp;
    node n;

    for (n=first_node(g->arestas_grafo); n; n=next_node(n)) {
        arestaTemp = content(n);
        if (checa_aresta(a, arestaTemp)) {
            return 1;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
// devolve um grafo de nome s,
//         ou
//         NULL, caso não exista tal grafo
// primeiro verifica se ALGUM grafo existe, caso nao, retorna NULL

grafo grafo_nome(char *s) {
    int n;
    grafo g;
    node it;

    if(!grafos_lidos) //verifica se existe ALGUM grafo lido
        return NULL;

    g = content(grafos_lidos->first);
    it = grafos_lidos->first;

    for(n=1; n <= tam_queue(grafos_lidos); n++) {
        it = next_node(it);
        if(compara_strings(g->nome_graf,s)) {
            return g;
        }
        g = content(it);
    }
    return NULL;
}

//------------------------------------------------------------------------------
// devolve 1, se g é direcionado,
//         ou
//         0, caso contrário

int direcionado(grafo g){
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

int ponderado(grafo g){
    if(g != NULL)
        return g->ponde;
    else
        return 0;
}


//------------------------------------------------------------------------------
// devolve 1, se g é tipado,
//         ou
//         0, caso contrário
//
// o grafo g é tipado se todas as suas arestas/arcos tem um
// atributo de nome "weight" cujo valor é um número.

int tipado(grafo g){
    if(g != NULL)
        return g->tipado;
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

//------------------------------------------------------------------------------
// Funções de verificação de peso
// Se Agedge tem peso retorna seu peso, caso contrário retorna 0

static int acha_peso(Agedge_t *at){
    char *peso = agget(at, (char*)"weight");
    if(!peso)
        return(0);
    return(atoi(peso));
}

//------------------------------------------------------------------------------
// Funções de verificação de tipo
// Se Agedge tem peso retorna seu peso, caso contrário retorna 0

static char* acha_tipo(Agnode_t *an){
    char *tipo = agget(an, (char*)"tipo");
    if(!tipo) {
        return(0);
    }
    return(tipo);
}

//------------------------------------------------------------------------------
// Funções de busca para ordenação e componentes

// static void marca_vertice(vertice v){
//     v->marcado = 1;
// }

// static int marcado(vertice v){
//     return v->marcado;
// }

//------------------------------------------------------------------------------
// Guarda as arestas/arcos no grafo e os apontadores para os vértices

static void le_arestas(Agraph_t *graph, grafo g, Agnode_t *vert) {
    aresta a;
    Agedge_t *at;
    vertice v;
    node n;

    for(at=agfstedge(graph, vert); at; at=agnxtedge(graph, at, vert)) {
        if(!direcionado(g))
            if (aghead(at) == vert) continue; //verificação pra casos não direcionados
        a = (aresta)malloc(sizeof(struct aresta));
        a->peso=0;
        if(ponderado(g))
            a->peso = acha_peso(at);
        for(n = first_node(g->vertices_grafo); n; n=next_node(n)) {
            v = content(n);

            if(at &&compara_strings(nome_vertice(v), agnameof(agtail(at)))) {
                a->tail = v;
                append_queue(a,v->saida);
                v->n_arestas_sai++;
            }

            if(at &&compara_strings(nome_vertice(v), agnameof(aghead(at)))) {
                a->head = v;
                append_queue(a,v->entrada);
                v->n_arestas_ent++;
            }
        }
        if (!busca_aresta(a, g)) {
            append_queue(a,g->arestas_grafo);
        }
    }
}

//------------------------------------------------------------------------------
// Guarda os vértices no grafo

static void le_vertices(Agraph_t *graph, grafo g) {
    vertice v;
    Agnode_t *vert;

    for (vert = agfstnode(graph); vert; vert = agnxtnode(graph,vert)) {

        v = (vertice)malloc(sizeof(struct vertice));
        // #error Aqui da seg fault se descomentar a linha
        // Ele entra na função printa o tipo mas da seg fault
        if (tipado(g))
            v->tipo = acha_tipo(vert);
        v->nome_vert = agnameof(vert);
        v->n_arestas_ent=0;
        v->n_arestas_sai=0;
        v->entrada=constroi_queue();
        v->saida=constroi_queue();
        v->vertice_entrada=constroi_queue();
        v->vertice_saida=constroi_queue();
        append_queue(v,g->vertices_grafo);
    }
}

//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot.
//         NULL, em caso de erro
//
// devolve o grafo escrito,
//         ou

static void imprime_arestas(grafo g){
    node n;
    aresta arest;

    for(n=first_node(g->arestas_grafo); n; n=next_node(n)) {
        arest=content(n);
        if ((arest->head != NULL) && (arest->tail != NULL)) {
            printf(" %s", nome_vertice(arest->tail));
            printf(" -%s", direcionado(g) ? ">" : "-");
            printf(" %s;\n", nome_vertice(arest->head));
            if(ponderado(g))
                printf(" [peso = %ld]", peso_aresta(arest));
            printf("\n");
        }
    }
    printf("}\n");
}

static void imprime_vertices(grafo g){
    node n;
    vertice v;

    for(n=first_node(g->vertices_grafo); n; n=next_node(n)) {
        v = content(n);
        if(direcionado(g))
            printf(" %s -- Grau: Saída[%i] - Entrada[%i]\n", nome_vertice(v), v->n_arestas_sai, v->n_arestas_ent);
        else
            printf(" %s -- Grau: [%i]\n", nome_vertice(v), (v->n_arestas_sai + v->n_arestas_ent));
    }
}

grafo escreve_grafo(FILE *output, grafo g) {
    fprintf(output, "------ GRAFO LIDO ------  \n");
    fprintf(output, "%sgraph %s{\n", direcionado(g) ? "di" : "", nome_grafo(g));
    imprime_arestas(g);
    imprime_vertices(g);

    if(g->grafo_recomendado){
      fprintf(output, "------ GRAFO DE RECOMENDAÇÕES ------  \n");
      fprintf(output, "%sgraph %s{\n", direcionado(g) ? "di" : "", nome_grafo(g->grafo_recomendado));
      imprime_arestas(g->grafo_recomendado);
      imprime_vertices(g->grafo_recomendado);
    }

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
    for(n=1; n <= tam_queue(g->vertices_grafo); n++) {
        nod = next_node(nod);
        if (compara_strings(nome_vertice(vert), s))
            return(vert);
        if (nod !=NULL)
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

    if(!g) {
        return 0;
    }
    if(!v) {
        return 0;
    }

    vert = vertice_nome(nome_vertice(v),g);

    if(direcao == 1) {
        return vert->n_arestas_sai;
    }
    if(direcao == -1) {
        return vert->n_arestas_ent;
    }

    //default ignora direcionado
    return (vert->n_arestas_ent+vert->n_arestas_sai);
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

    if(direcionado(g) && direcao == -1) { //retorna o de entrada caso direcao -1 e direcionado
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
// Função que verifica vizinhança entre c1 e c2, se vizinhança de c2 > c1
// mesmo após a intercessao da vizinhança entre c1 e c2 ser retirada
// os produtos de c2 são recomendados pra c1
// Se vizinhança maior retorna 1
// Caso contrario retorna 0

int verifica_vizinhanca(vertice c1, vertice c2, grafo g){
  int i;
  int pertence = 0;
  vertice vert; //vertice pra interação na lista de vertices de c1
  aresta comp1 = (aresta)malloc(sizeof(struct aresta)); //aresta para verificar se o produto P de c2 tem aresta com c1
  aresta comp2 = (aresta)malloc(sizeof(struct aresta));
  node n;

  comp1->tail = c2;
  comp2->head = c2;

  if(tam_queue(c1->vertice_entrada) > 0){
    for(n = first_node(c1->vertice_entrada); n; n=next_node(n)){
      vert= content(n);
      comp1->head = vert;
      comp2->tail = vert;
      if(busca_aresta(comp1,g) || busca_aresta(comp2,g)){
        pertence++;
      }
    }
  }
  if(tam_queue(c1->vertice_saida) > 0){
    comp1->head = c2;
    comp2->tail = c2;

    for(n = first_node(c1->vertice_saida); n; n=next_node(n)){
      vert= content(n);
      comp1->tail = vert;
      comp2->head = vert;
      if(busca_aresta(comp1,g) || busca_aresta(comp2,g)){
        pertence++;
      }
    }
  }

  i = tam_queue(c2->vertice_saida) +  tam_queue(c2->vertice_entrada);
  if(i - pertence > 0 ){
    return 1;
  }
  return 0;
}

void separa_tipos(grafo g){
  node n;
  vertice vert;

  for(n = first_node(g->vertices_grafo); n; n=next_node(n)){
    vert = content(n);
     if(strcmp(vert->tipo,"c") == 0){
      append_queue(vert,g->consumidor);
    }
     if(strcmp(vert->tipo,"p") == 0){
      append_queue(vert,g->produtos);
    }
  }
}


//------------------------------------------------------------------------------
// c(tail) --> p(head)
//cria e adiciona subgrafo dos produtos de c2 recomendados para c1
void cria_recomendado(vertice c1, vertice c2, grafo g){
  node n,n2;
  vertice vert;
  vertice v,v2;
  aresta comp1 = (aresta)malloc(sizeof(struct aresta));
  aresta comp2 = (aresta)malloc(sizeof(struct aresta));
  aresta a;

  //se c1 ainda não está em recomendado é adicionado
  //caso esteja então somente preciso somente pegar esse vertice para alterações

  if(!vertice_nome(c1->nome_vert,g->grafo_recomendado)){
    v2 = (vertice)malloc(sizeof(struct vertice));
    v2->vertice_entrada = constroi_queue();
    v2->vertice_saida = constroi_queue();
    v2->saida = constroi_queue();
    v2->entrada = constroi_queue();
    v2->tipo = 'c';
    v2->nome_vert = c1->nome_vert;
    v2->n_arestas_ent = 0;
    v2->n_arestas_sai = 0;
    append_queue(v2,g->grafo_recomendado->consumidor);
    append_queue(v2,g->grafo_recomendado->vertices_grafo);
    g->grafo_recomendado->n_vertice++;
    g->grafo_recomendado->n_aresta++;
  }else{
    v2=vertice_nome(c1->nome_vert,g->grafo_recomendado);
  }

  if(tam_queue(c2->vertice_saida) > 0){
    comp1->tail=v2;
    comp2->tail=c1;
    for(n=first_node(c2->vertice_saida); n ; n=next_node(n)){
      vert = content(n);
      comp1->head = vert;
      comp2->head = vert;
      //a aresta {c,p} não existe, então é adicionada ao grafo recomendado
      if(!busca_aresta(comp2,g)){
        if(!vertice_nome(vert->nome_vert,g->grafo_recomendado)){
          a = (aresta)malloc(sizeof(struct aresta));
          v = (vertice)malloc(sizeof(struct vertice));
          v->vertice_entrada = constroi_queue();
          append_queue(v2,v->vertice_entrada);
          v->vertice_saida = constroi_queue();
          v->saida = constroi_queue();
          v->entrada = constroi_queue();
          v->tipo = 'p';
          v->nome_vert = vert->nome_vert;
          v->n_arestas_ent = 1;
          v->n_arestas_sai = 0;
          a->head = v;
          a->tail = v2;
          a->peso = 1;
          append_queue(a, v->entrada);
          append_queue(v,g->grafo_recomendado->produtos);
          append_queue(v,g->grafo_recomendado->vertices_grafo);
          append_queue(a,g->grafo_recomendado->arestas_grafo);
          g->grafo_recomendado->n_vertice++;
          g->grafo_recomendado->n_aresta++;
          v2->n_arestas_sai++;
          append_queue(v, v2->vertice_saida);
          append_queue(a, v2->saida);
        }else{
          vert= vertice_nome(vert->nome_vert,g->grafo_recomendado);
          printf("\n");
          for(n2=first_node(vert->entrada);n2;n2=next_node(n2)){
            a=content(n2);
            if(strcmp(vert->nome_vert,a->head->nome_vert)==0){
              a->peso++;
            }
          if(busca_aresta(comp1,g->grafo_recomendado)==0){
            vert= vertice_nome(vert->nome_vert,g->grafo_recomendado);
            a = (aresta)malloc(sizeof(struct aresta));
            a->tail=v2;
            a->head=vert;
            a->peso=0;
            append_queue(a,vert->entrada);
            vert->n_arestas_ent++;
            append_queue(a,v2->saida);
            v2->n_arestas_sai++;
          }
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
// devolve o grafo de recomendações de g
//
// cada aresta {c,p} de H representa uma recomendação do produto p
// para o consumidor c, e tem um atributo "weight" que é um inteiro
// representando a intensidade da recomendação do produto p para o
// consumidor c.
//
// cada vértice de g tem um atributo "tipo" cujo valor é 'c' ou 'p',
// conforme o vértice seja consumidor ou produto, respectivamente

grafo recomendacoes(grafo g){
 node n1, n2;
 vertice c1,c2;

  for(n1=first_node(g->consumidor); n1 ; n1=next_node(n1)){
    for(n2=first_node(g->consumidor); n2 ; n2=next_node(n2)){
      c1 = content(n1);
      c2 = content(n2);
      if(n1!=n2){
        if(verifica_vizinhanca(c1,c2,g) !=0){
          cria_recomendado(c1,c2,g);
        }
      }
    }
  }
  return g->grafo_recomendado;
}


//------------------------------------------------------------------------------
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
    grafo g2 = (grafo)malloc(sizeof(struct grafo));
    if (!g)
        return NULL;

    g->nome_graf = agnameof(gf);
    g->direcio = agisdirected(gf);
    g->n_vertice = agnnodes(gf);
    g->n_aresta = agnedges(gf);
    g->vertices_grafo = constroi_queue();
    g->arestas_grafo = constroi_queue();
    g->ponde = checa_ponderado(gf);
    g->tipado = checa_tipado(gf);
    g->produtos = constroi_queue();
    g->consumidor = constroi_queue();

    g->grafo_recomendado = (grafo)malloc(sizeof(struct grafo));
    g->grafo_recomendado->vertices_grafo = constroi_queue();
    g->grafo_recomendado->arestas_grafo = constroi_queue();
    g->grafo_recomendado->produtos = constroi_queue();
    g->grafo_recomendado->consumidor = constroi_queue();
    g->grafo_recomendado->ponde = 1;
    g->grafo_recomendado->tipado = 1;
    g->grafo_recomendado->n_vertice = 0;
    g->grafo_recomendado->n_aresta = 0;
    g->grafo_recomendado->direcio = 0;
    g->grafo_recomendado->nome_graf = "recomendacoes";

    le_vertices(gf, g);

    for (vet=agfstnode(gf); vet; vet=agnxtnode(gf,vet))
        le_arestas(gf,g,vet);

    for(n=first_node(g->vertices_grafo); n; n=next_node(n)) {
        v=content(n);

        m =first_node(v->saida);
        for(j=1; j<= (tam_queue(v->saida)); j++) {
            a=content(m);
            if(!compara_strings(nome_vertice(a->tail), nome_vertice(v)))
                append_queue(a->tail,v->vertice_saida);
            else
                append_queue(a->head,v->vertice_saida);
            m=next_node(m);
        }
        m =first_node(v->entrada);
        for(j=1; j<= (tam_queue(v->entrada)); j++) {
            a=content(m);
            if(!compara_strings(nome_vertice(a->head), nome_vertice(v)))
                append_queue(a->head,v->vertice_saida);
            else
                append_queue(a->tail,v->vertice_saida);
            m=next_node(m);
        }
    }

    separa_tipos(g); //realiza a separação dos tipos de vertices
    g2=recomendacoes(g); //cria o grafo de recomendados
    grafos_lidos = constroi_queue();
    append_queue(g,grafos_lidos);
    append_queue(g2,grafos_lidos);

    agclose(gf);
    return g;
}
