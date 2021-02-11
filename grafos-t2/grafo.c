#include <stdio.h>
#include <stdlib.h>
#define __USE_XOPEN_EXTENDED
#include <string.h>
#include <errno.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

/*
 * ---------------------------------------
 *              DEFINES
 * ---------------------------------------
 */
 #define XPTO(x)           (void)(x)

typedef unsigned int UINT;
typedef long int LINT;
typedef unsigned short USHORT;
typedef long unsigned int LUINT;
typedef int bool;

#ifndef TRUE
#define TRUE        1
#endif

#ifndef FALSE
#define FALSE       !TRUE
#endif

//-----------------------------------------------------------------------------
// (apontador para) fila duplamente encadeada
typedef struct fila *fila;

//-----------------------------------------------------------------------------
// (apontador para) uma HEAP
typedef struct HEAP *HEAP;
//-----------------------------------------------------------------------------
// (apontador para) nó da fila encadeada cujo conteúdo é um void *

typedef struct no *no;

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar uma aresta

typedef struct aresta *aresta;
//------------------------------------------------------------------------------
// devolve o nome do grafo g

char *nome_grafo(grafo g);

//------------------------------------------------------------------------------
// devolve 1, se g é direcionado
//             ou
//         0, caso contrário

int direcionado(grafo g);

//------------------------------------------------------------------------------
// devolve 1, se g tem pesos nas arestas/arcos,
//      ou
// devolve 0, caso contrário

int ponderado(grafo g);

//------------------------------------------------------------------------------
// devolve o número de arestas/arcos do grafo g

unsigned int n_arestas(grafo g);

//------------------------------------------------------------------------------
// devolve o nome do vertice v

char *nome_vertice(vertice v);


//------------------------------------------------------------------------------

/*
 * ---------------------------------------
 *              FILA
 * ---------------------------------------
 */

//-----------------------------------------------------------------------------
// devolve o tamanho da fila
//             ou
// devolve NULL se o a filea é vazia
unsigned int tamanho_fila(fila f);

//-----------------------------------------------------------------------------
// devolve um apontador para o primeiro no da fila
//             ou
// devolve NULL se o a fila é vazia
no primeiro_no(fila f);

//---------------------------------------------------------------------------
// devolve o apontador para o conteúdo do nó
//             ou
// devolve NULL se o no é NULL
void *conteudo(no n);

//---------------------------------------------------------------------------
// devolve o apontador para o sucessor do no
//             ou
// devolve NULL se o no é NULL
no proximo_no(no n);

//---------------------------------------------------------------------------
// cria uma fila vazia e a devolve
//             ou
// devolve NULL em caso de falha
fila constroi_fila(void);

//---------------------------------------------------------------------------
// desaloca a fila f e todos os seus nós

// se destroi != NULL invoca
//     destroi(conteudo(n))
// para cada nó n da fila.
// devolve 1 em caso de sucesso,
//             ou
// devolve 0 em caso de falha
int destroi_fila(fila *f, int destroi(void *));

//---------------------------------------------------------------------------
// insere um novo nó na fila l cujo conteúdo é p
// devolve o no recém-criado
//             ou
// devolve NULL em caso de falha
no insere_fila(void *conteudo, fila f);

//------------------------------------------------------------------------------
// remove o no de endereço rno de f
// devolve o no, em caso de sucesso
//             ou
// devolve NULL em caso de falha
no remove_no(no rno, fila f);

vertice busca_vertice(const char* tail, const char* head,fila vertices, vertice* vdst);
void verifica_cabeca_calda(const char* vname, vertice* head, vertice* tail);
static void BuildListOfEdges(grafo g, Agraph_t* Ag_g, Agnode_t* Ag_v, const char* head_name);
static void BuildListOfArrows(grafo g, Agraph_t* Ag_g, Agnode_t* Ag_v, const char* head_name);
typedef void (*BuildList)(grafo, Agraph_t*, Agnode_t*, const char*);
void desvisitar_arestas(grafo g);
void desvisitar_vertices(grafo g);
int destroi_arestas(void* c);
int destroi_vertices(void* c);
void heapify(HEAP h);
void heap_sort(HEAP h, int i);
vertice heap_pop(HEAP h);
void heap_push(HEAP h, vertice data);
void heap_free(HEAP h);
HEAP heap_alloc(int elem);
int rot_ge(int *x, int *y);
int rot_g(int *x, int *y);
/*
 * ---------------------------------------
 *              STRUCTS
 * ---------------------------------------
 */

struct HEAP {
    int elem;
    int pos;
    vertice*   v;
};

struct no {
    void *conteudo;
    no prox;
};

struct fila {
    unsigned int tamanho;
    int padding; // só pra evitar warning
    no primeiro;
};

struct grafo {
    UINT n_vertices;
    UINT n_aresta;
    int tipo;
    bool eh_ponderado;
    char*  nome;
    fila vertices;        // fila de vértices.
    fila arestas;        // fila de arestas
};

struct vertice {
    char *nome;
    bool foi_visitado;
    bool foi_inserido;
    int* rot;
    UINT cor;
    int padding; // só pra evitar warning
    fila vizinhos_entrada;
    fila vizinhos_saida;
};

struct aresta {
    bool eh_ponderado;
    bool foi_visitado;
    LINT peso;
    vertice calda;    // tail
    vertice cabeca;   // head
};

/*
 * ---------------------------------------
 *              FILA
 * ---------------------------------------
 */

unsigned int tamanho_fila(fila f) {
    return f->tamanho;
}

no primeiro_no(fila f) {
    return f->primeiro;
}

void *conteudo(no n) {
    return n->conteudo;
}

no proximo_no(no n) {
    return n->prox;
}

fila constroi_fila(void){
    fila f;
    if (!(f = (fila) malloc(sizeof(struct fila)))) {
        perror("Could not allocate memory!");
        exit(EXIT_FAILURE);
    }

    if ( !f )
        return NULL;

    f->primeiro = NULL;
    f->tamanho = 0;

    return f;
}

int destroi_fila(fila *f, int destroi(void *)) {
    int retorno = 1;
    no n;
    while ( (n = primeiro_no(*f)) ) {
        n = remove_no(n,*f);
        if ( destroi )
            retorno &= destroi(conteudo(n));

        free(n);
    }

    free(*f);
    *f = NULL;
    return retorno;
}

no insere_fila(void *conteudo, fila f) {

    no novo = malloc(sizeof(struct no));

    if ( !novo )
        return NULL;

    novo->conteudo = conteudo;
    novo->prox = primeiro_no(f);

    ++f->tamanho;

    return f->primeiro = novo;
}

no remove_no(no rno, fila f){
    if (rno == primeiro_no(f)) {
        f->primeiro = rno->prox;
        --f->tamanho;
        return rno;
    }
    for (no n = primeiro_no(f); n->prox; n = proximo_no(n)) {
        if (n->prox == rno) {
            n->prox = rno->prox;
            free(rno);
            --f->tamanho;
            return rno;
        }
    }
    return NULL;
}

/*
 * FIM DA FILA
 */

//------------------------------------------------------------------------------
// Funcao para retornar quem é head, quem é tail em relação ao vertice vname.
void verifica_cabeca_calda(const char* vname, vertice* head, vertice* tail) {
    vertice tmp;

    if( strcmp(vname, (*head)->nome) != 0 ) {
        if( strcmp(vname, (*tail)->nome) == 0 ) {
            /* swap */
            tmp = *head;
            *head = *tail;
            *tail = tmp;
        }
    }
}


//------------------------------------------------------------------------------
// Le todas as arestas de um grafo não direcionado.
static void BuildListOfEdges(grafo g, Agraph_t* Ag_g, Agnode_t* Ag_v, const char* head_name) {
    XPTO(head_name);
    Agedge_t*   Ag_e;
    aresta a;
    char*       weight;
    char str_weight[5] = "peso";
    vertice head, tail;

    for( Ag_e=agfstedge(Ag_g, Ag_v); Ag_e; Ag_e=agnxtedge(Ag_g, Ag_e, Ag_v) ) {
        if( agtail(Ag_e) == Ag_v ) {
            if (!(a = (aresta) malloc(sizeof(struct aresta)))) {
                perror("Could not allocate memory!");
                exit(EXIT_FAILURE);
            }
            memset(a, 0, sizeof(struct aresta));
            weight = agget(Ag_e, str_weight);
            if( weight ) {
                a->peso = atol(weight);
                a->eh_ponderado = TRUE;
                g->eh_ponderado = TRUE;
            }
            tail = busca_vertice(agnameof(agtail(Ag_e)), \
                          agnameof(aghead(Ag_e)), g->vertices, &head);
            a->calda = tail;
            a->cabeca  = head;
            if( !insere_fila(a, head->vizinhos_saida ) ) exit(EXIT_FAILURE);
            if( !insere_fila(a, tail->vizinhos_entrada ) ) exit(EXIT_FAILURE);
        }
    }
}

//------------------------------------------------------------------------------
// Le todas os arcos de um grafo direcionato.
static void BuildListOfArrows(grafo g, Agraph_t* Ag_g, Agnode_t* Ag_v, const char* head_name) {
    Agedge_t*   Ag_e;
    aresta a;
    char*       weight;
    char str_weight[5] = "peso";
    vertice head, tail;

    for( Ag_e=agfstout(Ag_g, Ag_v); Ag_e; Ag_e=agnxtout(Ag_g, Ag_e) ) {
        if( agtail(Ag_e) == Ag_v ) {
            if (!(a = (aresta) malloc(sizeof(struct aresta)))) {
                perror("Could not allocate memory!");
                exit(EXIT_FAILURE);
            }
            memset(a, 0, sizeof(struct aresta));
            weight = agget(Ag_e, str_weight);
            if( weight ) {
                a->peso = atol(weight);
                a->eh_ponderado = TRUE;
                g->eh_ponderado = TRUE;
            }
            tail = busca_vertice(agnameof(agtail(Ag_e)), \
                          agnameof(aghead(Ag_e)), g->vertices, &head);
            verifica_cabeca_calda(head_name, &head, &tail);
            a->cabeca = head;
            a->calda  = tail;
            if( !insere_fila(a, tail->vizinhos_entrada ) ) exit(EXIT_FAILURE);
            if( !insere_fila(a, head->vizinhos_saida ) ) exit(EXIT_FAILURE);
        }
    }
}

//------------------------------------------------------------------------------
// Encontra um vertice baseado em seu nome.
// devolve  ponteiro para o elemento da fila head,
//          e tail em vdst.
vertice busca_vertice(const char* tail, const char* head,fila vertices, vertice* vdst) {

    int many = 0;
    vertice r_tail = NULL;
    vertice tmp = NULL;
    *vdst = NULL;
    for( no n=primeiro_no(vertices); n && many < 2; n=proximo_no(n) ) {
        tmp = (vertice)conteudo(n);
        if( strcmp(tail, tmp->nome) == 0 ) {
            r_tail = tmp;
            many++;
        }
        if( strcmp(head, tmp->nome) == 0 ) {
            *vdst = tmp;
            many++;
        }
    }
    return r_tail;
}
/*
 * ---------------------------------------
 *              GRAFO
 * ---------------------------------------
 */
char *nome_grafo(grafo g) {
    return g->nome;
}

char *nome_vertice(vertice v) {
    return v->nome;
}

int direcionado(grafo g) {
    return g->tipo;
}

int ponderado(grafo g) {
    return g->eh_ponderado;
}

int n_vertices(grafo g) {
    return (int)g->n_vertices;
}

UINT n_arestas(grafo g) {
    return g->n_aresta;
}

grafo le_grafo(FILE *input) {
    Agraph_t*   Ag_g;
    Agnode_t*   Ag_v;
    grafo g;
    vertice v;

    if (!(g = (grafo) malloc(sizeof(struct grafo)))) {
        perror("Could not allocate memory!");
        exit(EXIT_FAILURE);
    }
    memset(g, 0, sizeof(struct grafo));

    Ag_g = agread(input, NULL);
    if ( !Ag_g ) {
        free(g);
        perror("Could not allocate memory!");
        return NULL;
    }

    g->nome = strdup(agnameof(Ag_g));
    g->tipo = agisdirected(Ag_g);
    g->n_vertices= (UINT)agnnodes(Ag_g);
    g->n_aresta = (UINT)agnedges(Ag_g);
    g->vertices = constroi_fila();
    for( Ag_v=agfstnode(Ag_g); Ag_v; Ag_v=agnxtnode(Ag_g, Ag_v) ) {
        /* construct data for the actual vertex */
        if (!(v = (vertice) malloc(sizeof(struct vertice)))) {
            perror("Could not allocate memory!");
            exit(EXIT_FAILURE);
        }

        memset(v, 0, sizeof(struct vertice));
        v->nome =strdup(agnameof(Ag_v));
        if (!(v->rot = (int*) malloc( sizeof(int) * (LUINT)n_vertices(g)))) {
            perror("Could not allocate memory!");
            exit(EXIT_FAILURE);
        }
        memset(v->rot, 0, (int)  sizeof(int) *  (LUINT)n_vertices(g));
        v->vizinhos_entrada = constroi_fila();
        v->vizinhos_saida = constroi_fila();
        v->cor = 0;
        // Insert vertex to the list of vertexes in the graph list.
        if( !insere_fila(v, g->vertices) ) exit(EXIT_FAILURE);
    }

    /* get all edges; neighborhood of all vertexes */
    BuildList build_list[2];
    build_list[0] = BuildListOfEdges;
    build_list[1] = BuildListOfArrows;
    for( Ag_v=agfstnode(Ag_g); Ag_v; Ag_v=agnxtnode(Ag_g, Ag_v) )
        build_list[g->tipo](g, Ag_g, Ag_v, agnameof(Ag_v));

    agclose(Ag_g);
    return g;
}

// Seta para não visitados as arestas do grafo G.
void desvisitar_arestas(grafo g) {

    for (no nv = primeiro_no(g->vertices); nv; nv = proximo_no(nv)) {
        vertice v = conteudo(nv);
        for (no na = primeiro_no(v->vizinhos_saida); na; na = proximo_no(na))
            ((aresta)conteudo(na))->foi_visitado = FALSE;
    }
}

void desvisitar_vertices(grafo g) {
    for (no nv = primeiro_no(g->vertices); nv; nv = proximo_no(nv)) {
        ((vertice)conteudo(nv))->foi_visitado = FALSE;
        ((vertice)conteudo(nv))->foi_inserido = FALSE;
    }
}
//------------------------------------------------------------------------------
grafo escreve_grafo(FILE *output, grafo g) {
    vertice v;
    aresta e;
    char ch;
    no n, ne;

    if( !g ) return NULL;
    fprintf( output, "strict %sgraph \"%s\" {\n\n",
                  direcionado(g) ? "di" : "", g->nome
    );

    for( n=primeiro_no(g->vertices); n; n=proximo_no(n) ) {
        fprintf(output, "    \"%s\"", ((vertice)conteudo(n))->nome);
        //Não é posivel ter cor de numero 0, se exisit não é grafo color
        if(((vertice)conteudo(n))->cor) {
            fprintf(output, " [color=\"#%06X\"]",((vertice)conteudo(n))->cor);
        }
        fprintf(output, "\n");
    }
    fprintf( output, "\n" );

    ch = direcionado(g) ? '>' : '-';
    for( n=primeiro_no(g->vertices); n; n=proximo_no(n) ) {
        v = (vertice)conteudo(n);
        for( ne=primeiro_no(v->vizinhos_saida); ne; ne=proximo_no(ne) ) {
            e = (aresta)conteudo(ne);
            if( e->foi_visitado == TRUE ) continue;
            e->foi_visitado = TRUE;
            fprintf(output, "    \"%s\" -%c \"%s\"",
                          e->calda->nome, ch, e->cabeca->nome
            );

            if ( g->eh_ponderado )
                fprintf( output, " [peso=%ld]", e->peso );
            fprintf( output, "\n" );
        }
    }
    fprintf( output, "}\n" );

    desvisitar_arestas(g);
    return g;
}

int destroi_arestas(void* c) {
    aresta a = (aresta)c;
    if( a->cabeca ) {
        a->cabeca = a->calda = NULL;
        free(c);
    }
    c = NULL;

    return 1;
}

int destroi_vertices(void* c) {
    int ret;
    vertice v = (vertice)c;

    free(v->nome);
    v->nome = NULL;
    free(v->rot);
    v->rot = NULL;

    ret = destroi_fila(&(v->vizinhos_entrada), destroi_arestas);
    destroi_fila(&(v->vizinhos_saida), NULL);
    v->vizinhos_entrada = v->vizinhos_saida = NULL;
    free(c);
    c = NULL;

    return ret;
}

int destroi_grafo(grafo g) {
    int ret;

    free(g->nome);
    g->nome = NULL;
    ret = destroi_fila(&(g->vertices), destroi_vertices);

    g->vertices = NULL;
    free(g);
    g = NULL;

    return ret;
}

vertice vertice_de_nome(char *nome, grafo g){
    for (no n = primeiro_no(g->vertices); n; n = proximo_no(n)) {
        vertice aux = (vertice) conteudo(n);
        if( strcmp(nome, aux->nome) == 0 ) {
            return aux;
        }
    }
    return NULL;
}

vertice *busca_lexicografica(vertice r, grafo g, vertice *v){
    no na;
    vertice vert,aux;
    aresta a;
    int rotulo_atual, index,i;

    HEAP h;


    rotulo_atual = n_vertices(g);
    index = rotulo_atual -1;
    *(r->rot) = rotulo_atual;
    --rotulo_atual;
    v[index] = r;
    h = heap_alloc(n_vertices(g));
    heap_push(h,r);
    while( (vert = heap_pop(h)) != NULL ) {
        if(vert->foi_inserido) continue;
        vert->foi_inserido = TRUE;
        v[index] = vert;
        --index;
        for( na = primeiro_no(vert->vizinhos_entrada); na; na = proximo_no(na) ) {
            a = conteudo(na);
            if( !a->foi_visitado) {
                if(a->calda == vert) {
                    aux = a->cabeca;
                }else{
                    aux = a->calda;
                }
                if(!aux->foi_inserido) {
                    i = 0;
                    while( *(aux->rot+i++));
                    *(aux->rot+(i-1)) = rotulo_atual;
                }
                if( !aux->foi_visitado && !aux->foi_inserido) {
                    heap_push(h, aux);
                    aux->foi_visitado = TRUE;
                }
                a->foi_visitado = TRUE;
            }
        }

        for( na = primeiro_no(vert->vizinhos_saida); na; na = proximo_no(na) ) {
            a = conteudo(na);
            if( !a->foi_visitado) {
                if(a->calda == vert) {
                    aux = a->cabeca;
                }else{
                    aux = a->calda;
                }
                if(!aux->foi_inserido) {
                    i = 0;
                    while( *(aux->rot+i++));
                    *(aux->rot+(i-1)) = rotulo_atual;
                }
                if( !aux->foi_visitado && !aux->foi_inserido) {
                    heap_push(h, aux);
                    aux->foi_visitado = TRUE;
                }
                a->foi_visitado = TRUE;
            }
        }

        heapify(h);
        --rotulo_atual;
    }
    desvisitar_arestas(g);
    desvisitar_vertices(g);
    heap_free(h);
    return v;
}

unsigned int cor(vertice v, grafo g){
    XPTO(g);
    if (v == NULL) return 0;
    return v->cor;
}

unsigned int colore(grafo g, vertice *v){
    int i;
    no na;
    aresta a;
    UINT cor,max_cor;
    vertice aux;
    bool usado = FALSE;

    max_cor=0;
    for(i= 0; i < n_vertices(g); i++) {
        for(cor= 1; (cor <= (UINT)n_vertices(g)) && (v[i]->cor == 0); cor++) {
            usado = FALSE;
            for( na = primeiro_no(v[i]->vizinhos_saida); na; na = proximo_no(na) ) {
                a = conteudo(na);
                if(a->calda == v[i]) {
                    aux = a->cabeca;
                }else{
                    aux = a->calda;
                }
                if(aux->cor == cor) {
                    usado = TRUE;
                }
            }
            for( na = primeiro_no(v[i]->vizinhos_entrada); na; na = proximo_no(na) ) {
                a = conteudo(na);
                if(a->calda == v[i]) {
                    aux = a->cabeca;
                }else{
                    aux = a->calda;
                }
                if(aux->cor == cor) {
                    usado = TRUE;
                }
            }

            if(!usado) {
                v[i]->cor = cor;

                if(cor>max_cor) max_cor=cor;
            }

        }
    }
    return max_cor;
}

/*
 *##################################################################
 * Block that represents module for heap operations. Esta imple -
 * mentação usa o min heap, ou seja, o menor valor será a raiz.
 *
 * Ref.: https://pt.wikipedia.org/wiki/Heap
 *##################################################################
 */
#define DAD(k)      ( ((k) - 1) >> 1 )
#define L_CHILD(k)  ( (((k) + 1) << 1) - 1 )
#define R_CHILD(k)  ( ((k) + 1) << 1 );

HEAP heap_alloc(int elem) {
    HEAP h = (HEAP)malloc(sizeof(struct HEAP));
    if( !h ) exit(EXIT_FAILURE);
    h->v = (vertice*)malloc(sizeof(struct vertice) * (LUINT)elem);
    if( !h->v ) exit(EXIT_FAILURE);
    h->elem = elem;
    h->pos = 0;

    return h;

}

//------------------------------------------------------------------------------
void heap_free(HEAP h) {
    free(h->v);
    free(h);
}

//------------------------------------------------------------------------------
// menor rótulo
int rot_g(int *x, int *y) {
    int i = 0;

    while( *(x+i) == *(y+i) ) {
        if( *(x+i) == 0 )
            return 0;
        i++;
    }

    return( *(x+i) > *(y+i) );
}

//------------------------------------------------------------------------------
// rótulo maior ou igual.
int rot_ge(int *x, int *y) {
    int i = 0;

    while( *(x+i) == *(y+i) ) {
        if( *(x+i) == 0 )
            return 1;
        i++;
    }

    return *(x+i) >= *(y+i);
}

//------------------------------------------------------------------------------
void heap_push(HEAP h, vertice data) {
    int u, z;
    vertice tmp;

    if( h->pos == h->elem ) return;

    z = h->pos;
    *(h->v+z) = data;
    h->pos++;

    while( z ) {
        u = DAD(z);
        if( rot_ge((*(h->v+u))->rot, (*(h->v+z))->rot) ) break;

        tmp = *(h->v + u);
        *(h->v+u) = *(h->v + z);
        *(h->v+z) = tmp;
        z = u;
    }
}

//------------------------------------------------------------------------------
vertice heap_pop(HEAP h) {
    int k, l, r, child;
    vertice tmp, ret;

    if( h->pos == 0 ) return NULL;

    ret = *h->v;
    h->pos--;
    *h->v = *(h->v + h->pos);

    k = 0;
    r=0;
    while( (l = L_CHILD(r)) < h->pos ) {
        r = R_CHILD(k);
        if( r < h->pos && rot_g((*(h->v+l))->rot, (*(h->v+r))->rot) )
            child = r;
        else child = l;

        if( rot_g((*(h->v+k))->rot, (*(h->v+child))->rot) ) {
            tmp = *(h->v + child);
            *(h->v+child) = *(h->v + k);
            *(h->v+k) = tmp;
            k = child;
        } else break;
    }

    return ret;
}

//------------------------------------------------------------------------------
void heap_sort(HEAP h, int i) {
    int l, r, maior;
    l = L_CHILD(i);
    r = R_CHILD(i);
    if ((l < h->pos) && rot_g(h->v[l]->rot, h->v[i]->rot)) {
        maior = l;
    } else {
        maior = i;
    }
    if ((r < h->pos) && rot_g(h->v[r]->rot, h->v[i]->rot)) {
        maior = r;
    }
    if (maior != i) {
        vertice tmp = h->v[maior];
        h->v[maior] = h->v[i];
        h->v[i] = tmp;
        heap_sort(h, maior);
    }
}

//------------------------------------------------------------------------------
// Corrige descendo, tambem chamado de heapify.
void heapify(HEAP h) {
    for( int i = h->pos >> 1; i >= 0; --i )
        heap_sort(h, i);
}
