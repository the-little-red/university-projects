
/* Estruturas de dados Reaproveitado de outros trabalhos */

#include <stdio.h>
#include <stdlib.h>

typedef struct stack_node_type{
	struct stack_node_type *previous;
	void *content;
} stack_node_type;

typedef struct stack_type{
	struct stack_node_type *top;
} stack_type;

stack_type* init_stack(); /* Inicia a estutura, deve SEMPRE ser chamada */
int push_stack(stack_type*, void*); /* Empilha um ponteiro para um conteúdo qualquer */
void* pop_stack(stack_type*); /* Desempilha um conteúdo qualquer (NÃO DESALOCA O CONTEÚDO) */
int stack_is_empty(stack_type*); /* Retorna 1 se a pilha está vazia e 0 caso não esteja */
void deallocate_stack(stack_type*); /* Desaloca toda a pilha e destrói a estrutura (NÃO DESALOCA O CONTEÚDO) */

stack_type* init_stack(){
	stack_type *stack;
	stack = (stack_type*)malloc(sizeof(stack_type));
	if (!stack) return NULL;
	stack->top = (stack_node_type*)malloc(sizeof(stack_node_type));
	if (!stack->top) return NULL;
	stack->top->previous = NULL;
	stack->top->content = NULL;
	return stack;
}

int push_stack(stack_type *stack, void *thing){
	stack_node_type *new;
	new = (stack_node_type*)malloc(sizeof(stack_node_type));
	if (!new) return 0;
	new->previous = stack->top;
	stack->top = new;
	new->content = thing;
	return 1;
}

void* pop_stack(stack_type *stack){
	void *thing;
	stack_node_type *popped;
	if (!stack->top->previous) return NULL;
	thing = stack->top->content;
	popped = stack->top;
	stack->top = stack->top->previous;
	free(popped);
	return thing;
}

int stack_is_empty(stack_type *stack){
	if (!stack->top->previous)
		return 1;
	return 0;
}

void deallocate_stack(stack_type *stack){
	while(!stack_is_empty(stack))
		pop_stack(stack);
	free(stack->top);
	free(stack);
}

typedef struct row_node_type{
	struct row_node_type *previous, *next;
	void *content;
} row_node_type;

typedef struct row_type{
	struct row_node_type *base;
} row_type;

row_type* init_row(); /* Inicia a estutura, deve SEMPRE ser chamada */
int enter_row(row_type*, void*); /* Enfileira um ponteiro para um conteúdo qualquer */
void* turn_row(row_type*); /* Desenfileira um conteúdo qualquer (NÃO DESALOCA O CONTEÚDO) */
int row_is_empty(row_type*); /* Retorna 1 se a fila está vazia e 0 caso não esteja */
void deallocate_row(row_type *row); /* Desaloca toda a fila e destrói a estrutura (NÃO DESALOCA O CONTEÚDO) */

/* --------------- FUNCTIONS --------------- */
row_type* init_row(){
	row_type *row;
	row = (row_type*)malloc(sizeof(row_type));
	if (!row) return NULL;
	row->base = (row_node_type*)malloc(sizeof(row_node_type));
	if (!row->base) return NULL;
	row->base->next = row->base;
	row->base->previous = row->base;
	row->base->content = NULL;
	return row;
}

int enter_row(row_type *row, void *thing){
	row_node_type *new;
	new = (row_node_type*)malloc(sizeof(row_node_type));
	if(!new) return 0;
	new->next = row->base;
	new->previous = row->base->previous;
	row->base->previous->next = new;
	row->base->previous = new;
	new->content = thing;
	return 1;
}

void* turn_row(row_type *row){
	void *thing;
	row_node_type *removed;
	if(row->base->next == row->base) return NULL;
	removed = row->base->next;
	row->base->next = row->base->next->next;
	row->base->next->previous = row->base;
	thing = removed->content;
	free(removed);
	return thing;
}

int row_is_empty(row_type *row){
	if(row->base->next == row->base)
		return 1;
	return 0;
}

void deallocate_row(row_type *row){
	while(!row_is_empty(row))
		turn_row(row);
	free(row->base);
	free(row);
}

/* --------------- STRUCTURES --------------- */
typedef struct list_node_type{
	struct list_node_type *next, *previous;
	void *content;
} list_node_type;

typedef struct list_type{
	struct list_node_type *base;
} list_type;

/* --------------- HEADERS --------------- */
list_type* init_list(); /* Inicia a estutura, deve SEMPRE ser chamada */
int insert_list(list_type*, void*); /* Insere um ponteiro para um conteúdo qualquer */
int is_on_list(list_type*, void*); /* Retorn 1 se conteúdo está na lista e 0 caso não esteja */
int remove_list(list_type*, void*); /* Busca e remove um nó, retorna 1 caso o nó seja removido + */
int remove_node(list_type*, list_node_type*); /* Remove um nó da lista + */
list_node_type* first_on_list(list_type*); /* Retorna o primeiro elemento da lista */
list_node_type* last_on_list(list_type*); /* Retorna o último elemento da lista */
list_node_type* next_on_list(list_node_type*); /* Dado um nó da lista, retorna o próximo */
list_node_type* previous_on_list(list_node_type*); /* Dado um nó da lista, retorna o anterior */
void* get_content(list_node_type*); /* Retorna o conteúdo de um nó da lista */
void for_all_in_list(list_type*, void realize(void*)); /* Realiza um procedimento para toda a lista + */
void for_selected_in_list(list_type*, int feature(void*), void realize(void*)); /* + */
int list_is_empty(list_type*); /* Retorna 1 se a lista está vazia e 0 caso não esteja */
int end_list(list_type*, list_node_type*); /*Retorna 1 se o nodo se encontra no "final" da lista + */
void invert_list(list_type *list); /* Inverte a ordem da lista */
list_type* segregate_from_list(list_type *list, int feature(void*)); /* + */
list_type* select_from_list(list_type *list, int feature(void*)); /* + */
int include_on_list(list_type*, list_type*, int feature(void*)); /* + */
int switch_from_list(list_type*, list_type*, int feature(void*)); /* + */
void swap_nodes(list_node_type*, list_node_type*); /* Troca dois nós de lugar, podem ser de diferentes listas */
void remove_duplicate(list_type*); /* Checa repetições do mesmo PONTEIRO na lista e remove réplicas */
list_type* intersection_list(list_type*, list_type*); /* Retorna interseção entre duas listas */
list_type* union_list(list_type*, list_type*);/* Retorna união entre duas listas */
list_type* difference_list(list_type*, list_type*); /* Retorna diferença entre duas listas */
void deallocate_list(list_type*); /* Desaloca toda a lista e destrói a estrutura (NÃO DESALOCA O CONTEÚDO) */

/* --------------- FUNCTIONS --------------- */
list_type* init_list(){
	list_type *list;
	list = (list_type*)malloc(sizeof(list_type));
	if (!list) return NULL;
	list->base = (list_node_type*)malloc(sizeof(list_node_type));
	if (!list->base) return NULL;
	list->base->next = list->base;
	list->base->previous = list->base;
	list->base->content = NULL;
	return list;
}

int insert_list(list_type *list, void *thing){
	list_node_type *new;
	new = (list_node_type*)malloc(sizeof(list_node_type));
	if (!new) return 0;
	new->next = list->base;
	new->previous = list->base->previous;
	list->base->previous->next = new;
	list->base->previous = new;
	new->content = thing;
	return 1;
}

int is_on_list(list_type *list, void *thing){
	list_node_type *node = first_on_list(list);
	while(!end_list(list, node)){
		if (get_content(node) == thing) return 1;
		node = next_on_list(node);
	}
	return 0;
}

/* Passada uma lista e um ponteiro para o conteúdo do tipo que está sendo armazenado é realizada uma busca na lista e, caso o mesmo ponteiro esteja na lista, o nó no qual se encontra é removido e é retornado o valor 1, simbolizando que a remoção foi efetivada, caso o ponteiro não esteja na lista, a função retorna 0 */
int remove_list(list_type *list, void *thing){
	list_node_type *node = first_on_list(list);
	while(!end_list(list, node)){
		if (get_content(node) == thing){
			node->previous->next = node->next;
			node->next->previous = node->previous;
			free(node);
			return 1;
		}
		node = next_on_list(node);
	}
	return 0;
}

/* Difere da função revome_list pois não realiza busca, apenas recebe o ponteiro e o remove da lista, caso o ponteiro seja NULL a função retorna 0, caso o ponteiro não aponte para algo presente na lista os resultados são inesperados */
int remove_node(list_type *list, list_node_type *node){
	if(node){
		node->previous->next = node->next;
		node->next->previous = node->previous;
		free(node);
		return 1;
	}
	return 0;
}

list_node_type* first_on_list(list_type *list){
	return list->base->next;
}

list_node_type* last_on_list(list_type *list){
	return list->base->previous;
}

list_node_type* next_on_list(list_node_type *node){
	return node->next;
}

list_node_type* previous_on_list(list_node_type *node){
	return node->previous;
}

void* get_content(list_node_type *node){
	return node->content;
}

/* Essa função recebe uma lista e uma função no formato: void funcao(void*) - ela operara o procedimento descrito na função em todo o conteudo da lista, útil para desalocação, impressão, alteração de conteúdo, entre outros. A função passada como argumento receberá um ponteiro void que deve ser atribuído à outro ponteiro no formato do conteúdo armazenado pela lista antes de realizar a operação. */
void for_all_in_list(list_type *list, void realize(void*)){
	list_node_type *node = first_on_list(list);
	void *thing;
	while(!end_list(list, node)){
		thing = get_content(node);
		realize(thing);
		node=next_on_list(node);
	}
}

/* Essa função atua como a função for_all_in_list, porém, opera apenas nos nós que retornam verdadeiro para função attribute */
void for_selected_in_list(list_type *list, int feature(void*), void realize(void*)){
	list_node_type *node = first_on_list(list);
	void *thing;
	while(!end_list(list, node)){
		thing = get_content(node);
		if(feature(thing))
			realize(thing);
		node=next_on_list(node);
	}
}

int list_is_empty(list_type *list){
	if (end_list(list, first_on_list(list)))
		return 1;
	return 0;
}

/* Retorna o 'fim' da lista, porém, note que a lista é circular, para percorrer toda a lista sem problemas comece sempre do nó oferecido pela função first_on_list */
int end_list(list_type *list, list_node_type *node){
	if(list->base == node)
		return 1;
	return 0;
}

void invert_list(list_type *list){
	list_node_type *temp_node, *node = first_on_list(list);
	while(!end_list(list, node)){
		temp_node = node->next;
		node->next = node->previous;
		node->previous = temp_node;
		node = previous_on_list(node);
	}
	temp_node = node->next;
	node->next = node->previous;
	node->previous = temp_node;
}

/* Essa função recebe como parâmetros uma lista e uma função no formato: int funcao(void*) - e retorna uma lista. A função passada como parâmetro receberá o conteúdo de cada nó da lista da mesma forma como ocorre na função passada em for_all_in_list, porém, diferentemente daquela, essa função retornará um inteiro e separará da lista todos os nós que retornarem 1 nessa função, dessa forma é possível colocar em outra lista nós que possuem um conteúdo específico. A lista de retorno é inicializada dentro da função.*/
list_type* segregate_from_list(list_type *list, int feature(void*)){
	list_type *return_list = init_list();
	list_node_type *node, *aux_node;
	void *thing;
	node = first_on_list(list);
	while(!end_list(list, node)){
		thing = get_content(node);
		if(feature(thing)){
			aux_node = previous_on_list(node);
			insert_list(return_list, thing);
			remove_node(list, node);
			node = aux_node;
		}
		node = next_on_list(node);
	}
	return return_list;
}

/* Opera do mesmo modo que a função segregate_from_list, porém, não remove o conteúdo da lista de origem, apenas cria outra lista com os ponteiros para os conteúdos selecionados. Cuidado ao desalocar o conteúdo das listas pois podem haver mais de um ponteiro para o mesmo item. */
list_type* select_from_list(list_type *list, int feature(void*)){
	list_type *return_list = init_list();
	list_node_type *node;
	void *thing;
	node = first_on_list(list);
	while(!end_list(list, node)){
		thing = get_content(node);
		if(feature(thing))
			insert_list(return_list, thing);
		node = next_on_list(node);
	}
	return return_list;
}

/* Semelhante _a função select_from_list, porém, recebe um apêndice ao invés de criar uma lista nova. Retorna 0 caso a lista de retorno não tenha sido alocada */
int include_on_list(list_type *list, list_type *return_list, int feature(void*)){
	list_node_type *node;
	void *thing;
	if (!return_list) return 0;
	node = first_on_list(list);
	while(!end_list(list, node)){
		thing = get_content(node);
		if(feature(thing))
			insert_list(return_list, thing);
		node = next_on_list(node);
	}
	return 1;
}

/* Semelhante _a função segregate_from_list, porém, recebe um apêndice ao invés de criar uma lista nova. Retorna 0 caso a lista de retorno não tenha sido alocada */
int switch_from_list(list_type *list, list_type *return_list, int feature(void*)){
	list_node_type *node, *aux_node;
	void *thing;
	if (!return_list) return 0;
	node = first_on_list(list);
	while(!end_list(list, node)){
		thing = get_content(node);
		if(feature(thing)){
			aux_node = previous_on_list(node);
			insert_list(return_list, thing);
			remove_node(list, node);
			node = aux_node;
		}
		node = next_on_list(node);
	}
	return 1;
}

void swap_nodes(list_node_type *node1, list_node_type *node2){
	list_node_type *aux_node;
	node1->next->previous = node2;
	node2->next->previous = node1;
	aux_node = node1->next;
	node1->next = node2->next;
	node2->next = aux_node;
	node1->previous->next = node2;
	node2->previous->next = node1;
	aux_node = node1->previous;
	node1->previous = node2->previous;
	node2->previous = aux_node;
}

void remove_duplicate(list_type *list){
	list_node_type *aux_node, *node = first_on_list(list);
	while(!end_list(list, node)){
		aux_node = next_on_list(node);
		while(!end_list(list, aux_node)){
			if (get_content(node) == get_content(aux_node)){
				aux_node = previous_on_list(aux_node);
				remove_node(list, next_on_list(aux_node));
			}
			aux_node = next_on_list(aux_node);
		}
		node = next_on_list(node);
	}
}

list_type* intersection_list(list_type *list1, list_type *list2){
	list_type *return_list = init_list();
	list_node_type *node = first_on_list(list1);
	if(!return_list) return NULL;
	while(!end_list(list1, node)){
		if(is_on_list(list2, get_content(node)))
			insert_list(return_list, get_content(node));
		node = next_on_list(node);
	}
	return return_list;
}

list_type* union_list(list_type *list1, list_type *list2){
	list_type *return_list = init_list();
	list_node_type *node = first_on_list(list1);
	if(!return_list) return NULL;
	while(!end_list(list1, node)){
		insert_list(return_list, get_content(node));
		node = next_on_list(node);
	}
	node = first_on_list(list2);
	while(!end_list(list2, node)){
		if(!is_on_list(return_list, get_content(node)))
			insert_list(return_list, get_content(node));
		node = next_on_list(node);
	}
	return return_list;
}

list_type* difference_list(list_type *list1, list_type *list2){
	list_type *return_list = init_list();
	list_node_type *node = first_on_list(list1);
	if(!return_list) return NULL;
	while(!end_list(list1, node)){
		if(!is_on_list(list2, get_content(node)))
			insert_list(return_list, get_content(node));
		node = next_on_list(node);
	}
	return return_list;
}

void deallocate_list(list_type *list){
	list_node_type *node;
	while(!list_is_empty(list)){
		node = first_on_list(list);
		node->previous->next = node->next;
		node->next->previous = node->previous;
		free(node);
	}
	free(list->base);
	free(list);
}
