// ------------------------------------------------
//  Trabalho de Inteligência Artifical - 2018/01
//  Arianne de Paula Bortolan - GRR20140220
// ------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "libfilas.h" //Lib propria

row_type *solucao; //Lista de cores que são a solução
int solucao_number; //Número de passos

typedef struct node_type{ // Estrutura do vertice que armazena as informações
	list_type *arestas; // Lista de vizinhos
	int x, y;
  int cor;
  int id; //Número de identificação do vertice/bloco, um bloco deve ter todos a mesma ID, porém Número de ID != Número cor
} node_type;

typedef struct graph_type{ // Estrutura do grafo
	list_type *nodes; // Lista de vertices de um grafo
} graph_type;

// ------------------------------------------------
// Lê o grafo em um formato matricial, sendo que seus vertices ainda não possuem vizinhos,
// Porém os campos x e y vão marcar sua posição referente a linha e coluna em que se encontram
// ------------------------------------------------
void le_tabuleiro(graph_type *tabuleiro, int max_linhas, int max_colunas){
	int linha_atual, coluna_atual;
	node_type *aux_node;

	tabuleiro->nodes = init_list();
  coluna_atual = 0;
	linha_atual = 0;

  while(linha_atual < max_linhas){
		aux_node = (node_type*)malloc(sizeof(node_type));
		aux_node->x = linha_atual;
		aux_node->y = coluna_atual;
		aux_node->id = 0;
		scanf("%d", &aux_node->cor);
		insert_list(tabuleiro->nodes, aux_node);
		coluna_atual = (coluna_atual + 1) % max_colunas;
		if(!coluna_atual) linha_atual++;
	}
}

// ------------------------------------------------
// Cria os vizinhos com base em sua posição matricial, ou seja, o vertice [l,c]
// será vizinho dos vertices [l+1, c], [l-1, c], [l, c+1], [l, c-1] */
// ------------------------------------------------
void cria_arestas(graph_type *tabuleiro){
	list_node_type *lonl, *ronl;
	node_type *lon, *ron;

  lonl = first_on_list(tabuleiro->nodes);

  while(!(end_list(tabuleiro->nodes, lonl))){
		lon = get_content(lonl);
		lon->arestas = init_list();
		ronl = first_on_list(tabuleiro->nodes);

  	while(!(end_list(tabuleiro->nodes, ronl))){
			ron = get_content(ronl);

      // Se ele é um vizinho matricial insiro na minha lista de vizinhos
  		if(((lon->x == ron->x) && (abs(lon->y - ron->y) == 1)) || ((lon->y == ron->y) && (abs(lon->x - ron->x) == 1))){
				insert_list(lon->arestas, ron);
      }

			ronl = next_on_list(ronl);
    }
		lonl = next_on_list(lonl);
	}

}

// ------------------------------------------------
// Atribui IDs específicos para ser possível agrupar os vertices, um bloco da mesma cor deve possuir o mesmo ID
// ------------------------------------------------
void recursive_set_ids(node_type *node, int *current_id, int *check, int cor){
	list_node_type *pintor;
	node_type *vizinho;

  // Verifico se vertice não possui ID e se é da mesma cor que seu vizinho
	if((!(node->id)) && (node->cor == cor)){
		node->id = *current_id;
		pintor = first_on_list(node->arestas);

  	while(!(end_list(node->arestas, pintor))){
			vizinho = get_content(pintor);
			recursive_set_ids(vizinho, current_id, check, cor);
			pintor = next_on_list(pintor);
		}

  	*check = 1; // marco se um ID foi atribuído
	}
}

// ------------------------------------------------
// Controla a função de atribuição de IDs
// ------------------------------------------------
int set_ids(graph_type *tabuleiro){
	list_node_type *pintor;
	node_type *content;
	int current_id = 1, check;
	pintor = first_on_list(tabuleiro->nodes);

  while(!(end_list(tabuleiro->nodes, pintor))){
		check = 0;
		content = get_content(pintor);
		content->x = 0;
		recursive_set_ids(content, &current_id, &check, content->cor);

    if(check){
      current_id++;
    }
		pintor = next_on_list(pintor);
	}

	return current_id; // Retorno o ID corrente para saber quantos vertices eu terei
}

// ------------------------------------------------
// Função que agrupa os vertices conforme os IDs que eles possuem,
// um conjunto de vertices da mesma cor vira apenas um vertice com a uniáo dos vizinhos
// ------------------------------------------------
void recursive_set_vizinhos(node_type *node, int id, graph_type *tabuleiro, int cor){
	list_node_type *pintor;
	node_type *vizinho, *source = NULL, *destiny = NULL, *new_node;

  if((node->id == id) && !(node->x)){
		node->x = 1;
		pintor = first_on_list(node->arestas);

    while(!(end_list(node->arestas, pintor))){
			vizinho = get_content(pintor);
			recursive_set_vizinhos(vizinho, id, tabuleiro, cor);
			pintor = next_on_list(pintor);
		}
	}
	else
  if(!(node->cor == cor)){
		pintor = first_on_list(tabuleiro->nodes);

    while(!(end_list(tabuleiro->nodes, pintor))){
			new_node = get_content(pintor);

    	if(new_node->id == id){
				new_node->cor = cor;
				source = new_node;
			}

    	if(new_node->id == node->id){
				destiny = new_node;
      }
      // Se encontrei meus vertices de origem e destino eu saio
    	if(source && destiny)
        break;

      pintor = next_on_list(pintor);
		}
		insert_list(source->arestas, destiny); // meu vertice destino entra na lista de vizinhos do vertice origem
	}
}

// ------------------------------------------------
// Controla a função recursiva de monta a vizinhança, à partir de um grafo sem vizinhança,
// checa pelos IDs quem deve ser os vizinhos dos vertices e insere eles em suas vizinhanças
// ------------------------------------------------
void define_vizinhanca(graph_type *tabuleiro, graph_type *new_tabuleiro){
	list_node_type *pintor;
	node_type *content;
	pintor = first_on_list(tabuleiro->nodes);

  while(!(end_list(tabuleiro->nodes, pintor))){
		content = get_content(pintor);
		recursive_set_vizinhos(content, content->id, new_tabuleiro, content->cor);
		pintor = next_on_list(pintor);
	}

}

// ------------------------------------------------
// Com o grafo montado no início, monto o grafo agrupado por cores usando os IDs atribuídos
// ------------------------------------------------
graph_type* organiza_tabuleiro(graph_type *tabuleiro, int nodes_number){
	graph_type *new_tabuleiro = (graph_type*)malloc(sizeof(graph_type));
	node_type *new, *content;
	list_node_type *pintor;
	int i;

  new_tabuleiro->nodes = init_list();

  for(i=1; i<nodes_number; i++){
		new = (node_type*)malloc(sizeof(node_type));
		new->arestas = init_list();
		new->id = i;
		new->x = 0;
		new->y = 0;
		insert_list(new_tabuleiro->nodes, new);
	}

  define_vizinhanca(tabuleiro, new_tabuleiro);
	pintor = first_on_list(new_tabuleiro->nodes);

  while(!(end_list(new_tabuleiro->nodes, pintor))){
		content = get_content(pintor);
		remove_duplicate(content->arestas);
		pintor = next_on_list(pintor);
	}
	return new_tabuleiro;
}

// ------------------------------------------------
// Desaloca vertices de um grafo
// ------------------------------------------------
void deallocate_nodes(void *p){
	node_type *aux = p;
	deallocate_list(aux->arestas);
	free(aux);
}

// ------------------------------------------------
// Desaloca o grafo
// ------------------------------------------------
void deallocate_tabuleiro(graph_type *tabuleiro){
	for_all_in_list(tabuleiro->nodes, deallocate_nodes);
	deallocate_list(tabuleiro->nodes);
	free(tabuleiro);
}

// ------------------------------------------------
// Marca a variável de controle como zero
// ------------------------------------------------
void uncheck_visited(void *p){
	node_type *content = p;
	content->x = 0;
}

// ------------------------------------------------
// Função que simula uma pintura, dado um grafo e um vertice, a pintura é "feita" com a cor do vertice dado,
// Então é verificado qual a soma de distâncias que o tabuleiro retorna para uma pintura dessa cor
// ------------------------------------------------
int estimate_cost(graph_type *tabuleiro, node_type *vizinho){
	int distancia = 0;
	list_node_type *pintor, *first = first_on_list(tabuleiro->nodes);
	node_type *aux, *content_aux, *content_first = get_content(first);
	row_type *to_process = init_row(); // fila de processamento

  for_all_in_list(tabuleiro->nodes, uncheck_visited);
	content_first->y = 0;
	content_first->x = 1;

  enter_row(to_process, content_first);
	vizinho->y = 0;
	vizinho->x = 1;
	enter_row(to_process, vizinho);

  while(!(row_is_empty(to_process))){
		aux = turn_row(to_process);
		pintor = first_on_list(aux->arestas);

    while(!(end_list(aux->arestas, pintor))){
  	   content_aux = get_content(pintor);

    	if(!content_aux->x){
				content_aux->x = 1;
				content_aux->y = aux->y + 1;
				distancia += content_aux->y;
				enter_row(to_process, content_aux);
			}

			pintor = next_on_list(pintor);
		}
	}
	return distancia;
}

// ------------------------------------------------
// Junta somente dois vertices, junta o vertice principal com o vertice escolhido
// ------------------------------------------------
void merge_nodes(graph_type *tabuleiro, node_type *raiz, node_type *chosen){
	list_node_type *pintor, *pintor_list;
	node_type *vizinho;
	raiz->cor = chosen->cor;
	pintor = first_on_list(chosen->arestas);

  while(!(end_list(chosen->arestas, pintor))){
		vizinho = get_content(pintor);

    if((!(is_on_list(vizinho->arestas, raiz))) && (vizinho != raiz)){
			insert_list(vizinho->arestas, raiz);
    }
    remove_list(vizinho->arestas, chosen);
		pintor = next_on_list(pintor);
	}

  pintor_list = first_on_list(chosen->arestas);

  while(!end_list(chosen->arestas, pintor_list)){

    // Se não é vizinho da raiz, então insere como vizinho da raiz
    if(!is_on_list(raiz->arestas, get_content(pintor_list))){
			insert_list(raiz->arestas, get_content(pintor_list)); //
    }
    pintor_list = next_on_list(pintor_list);
	}

  remove_list(raiz->arestas, raiz);
	deallocate_list(chosen->arestas);
	free(chosen);
	remove_list(tabuleiro->nodes, chosen);

}

// ------------------------------------------------
// Recebe uma cor e busca por vizinhanças dessa cor, ao encontrar, pinta o tabuleiro
// ------------------------------------------------
void paint(graph_type *tabuleiro, node_type *raiz, int cor){
	list_node_type *pintor, *aux;
	node_type *vizinho;
	raiz->cor = cor;
	pintor = first_on_list(raiz->arestas);

	while(!(end_list(raiz->arestas, pintor))){
		vizinho = get_content(pintor);
		aux = next_on_list(pintor);

  	if(vizinho->cor == cor){
			merge_nodes(tabuleiro, raiz, vizinho); // função que faz a pintura
    }

  	pintor = aux;
	}

}

// ------------------------------------------------
// Função "principal", recebe o tabuleiro fornecido e o soluciona
// ------------------------------------------------
void resolve_tabuleiro(graph_type *tabuleiro){
	int current_cost, lower = 2000000, *new_cor;
	list_node_type *pintor_tabuleiro, *pintor_node;
	node_type *content, *vizinho, *melhor;

	pintor_tabuleiro = first_on_list(tabuleiro->nodes);
	content = get_content(pintor_tabuleiro);
	pintor_node = first_on_list(content->arestas);

	while(!(list_is_empty(content->arestas))){

		while(!(end_list(content->arestas, pintor_node))){
			vizinho = get_content(pintor_node);
			current_cost = estimate_cost(tabuleiro, vizinho);

			if(current_cost <= lower){
				lower = current_cost;
				melhor = vizinho;
			}

			pintor_node = next_on_list(pintor_node);

		}
		new_cor = (int*)malloc(sizeof(int));
		*new_cor = melhor->cor;
		enter_row(solucao, new_cor);
		solucao_number++;
		paint(tabuleiro, content, melhor->cor);
		pintor_tabuleiro = first_on_list(tabuleiro->nodes);
		content = get_content(pintor_tabuleiro);
		pintor_node = first_on_list(content->arestas);
	}
}


int main(){
	int max_linhas, max_colunas, max_cores, nodes_number, *cor;
	graph_type *new_tabuleiro, *tabuleiro = (graph_type*)malloc(sizeof(graph_type));
	solucao = init_row();
	solucao_number = 0;

  scanf("%d %d %d", &max_linhas, &max_colunas, &max_cores);

  le_tabuleiro(tabuleiro, max_linhas, max_colunas);

	cria_arestas(tabuleiro);

	nodes_number = set_ids(tabuleiro);

  new_tabuleiro = organiza_tabuleiro(tabuleiro, nodes_number);

  deallocate_tabuleiro(tabuleiro);

  resolve_tabuleiro(new_tabuleiro);

  deallocate_tabuleiro(new_tabuleiro);

  printf("%d\n", solucao_number);

	while(!(row_is_empty(solucao))){
		cor = (int*)turn_row(solucao);
		printf("%d ", *cor);
		free(cor);
	}

  printf("\n");
	deallocate_row(solucao);
}
