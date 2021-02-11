#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila

void queue_append (queue_t **queue, queue_t *elem){
	queue_t *aux = *queue;

	//verifica se elemento existe
	if(elem == NULL){
		printf("Erro! Elemento não existe!\n");
		return;
	}
	//verifica se elemento já não foi inserido numa fila
	if(elem->next != NULL || elem->prev != NULL){
		//printf("Erro! Elemento já esta em uma fila!\n");
		return;
	}
	//verifica se fila existe
	if(queue == NULL){
	 	printf("Erro! Fila não existe!\n");
	 	return;
	}
	//verifica se fila vazia caso sim um tipo de inserção caso não, outro tipo.
	if(*queue == NULL){
		*queue = elem;
		(*queue)->next = elem;
		(*queue)->prev = elem;
	}else{
		elem->next=aux;
		(*queue)->prev = elem;
		while((*queue)->next != aux){
			*queue=(*queue)->next;
		}
		elem->prev= *queue;
		(*queue)->next = elem;
		(*queue) = aux;
	}
}

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: apontador para o elemento removido, ou NULL se erro

 queue_t *queue_remove (queue_t **queue, queue_t *elem){
	 queue_t *aux = *queue;
	 //verifica se fila existe
	 if(queue == NULL){
		 printf("Erro! Fila não existe!\n");
		 return NULL;
	 }
	 //verifica se fila esta vazia
	 if(*queue == NULL){
		 //printf("Erro! Fila esta vazia!\n");
		 return NULL;
	 }

	 //verifica se elemento existe
	 if(elem == NULL){
		 printf("Erro! Elemento não existe!\n");
		 return NULL;
	 }

	 //verifica se só existe um elemento
	 if(*queue == (*queue)->next && *queue == elem){
		 *queue = NULL;
		 elem->next = NULL;
		 elem->prev = NULL;
		 return elem;
	 }

	 if(*queue == elem){
		 (*queue)->next->prev = (*queue)->prev;
		 (*queue)->prev->next = (*queue)->next;
		 *queue = (*queue)->next;
		 elem->next = NULL;
		 elem->prev = NULL;
		 return elem;
		}
	 *queue = (*queue)->next;
	 while(*queue != aux){
		 if(*queue == elem){
			 (*queue)->next->prev = (*queue)->prev;
			 (*queue)->prev->next = (*queue)->next;
			 *queue = aux;
			 elem->next = NULL;
			 elem->prev = NULL;
			 return elem;
			}
		*queue = (*queue)->next;
	 }
	// printf("Erro! Elemento não pertence a fila! \n");
	 return NULL;
}

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila

int queue_size (queue_t *queue){
	queue_t *aux = queue;
	int i = 0;
	if(!queue){
		return i;
	}else{
		i++; }
	while(queue->next != aux){
		i++;
		queue = queue->next;
	}
	return i;
}

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print (char *name, queue_t *queue, void print_elem (void*) ){
 	int num = queue_size (queue);

 	if (queue == NULL) {
 		printf("%s: []\n", name);
 	}else{
		printf("%s: [", name);
 		while (num > 0){
			print_elem(queue);
			printf(" ");
			queue = queue->next;
			num--;
 		}
		printf("\b]\n");
	}
}
