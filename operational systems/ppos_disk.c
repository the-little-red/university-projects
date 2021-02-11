#include "hw_disk.h"
#include "ppos_disk.h"
#include "ppos_data.h"
#include "ppos.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define DEBUG //ifdef debug não esta funcionando ???

extern task_t *ready;
extern task_t *suspend;
extern task_t *sleppy;
extern task_t *tdisk;
extern task_t *atual;

disk_t *disco = NULL;
disk_t *fila_disco = NULL;
extern semaphore_t *acesso;


void diskDriverBody (void *args){
  task_t* aux = suspend;
  disk_t* aux2;

  while(1){

    sem_down(acesso);

    if(tdisk->wake_disk == WAKE_DISK){

      while(disco->id != aux->id){
        aux = aux->next;
      }
      queue_append((queue_t**)&ready,queue_remove((queue_t**)&suspend,(queue_t*)aux));
      aux->status = PRONTA;
      sem_up(acesso);

      #ifdef DEBUG
      printf("diskDriver: dentro tdisk if \n");
      #endif
      tdisk->status = ESPERA;
      queue_append((queue_t**)&suspend,queue_remove((queue_t**)&ready,(queue_t*)tdisk));
      task_switch(aux);
    }

    if((disk_cmd (DISK_CMD_STATUS,0,0) == 0) && (fila_disco != NULL)){
      aux2 = (disk_t *) queue_remove((queue_t**)&fila_disco,(queue_t*)fila_disco);

      if(aux2->func == LEITURA){
        disco->id = fila_disco->id;
        disk_cmd (DISK_CMD_READ, aux2->bloco, aux2->buffer);
        disco->status = disk_cmd(DISK_CMD_STATUS,0,0);
      }else{
        disco->id = fila_disco->id;
        aux2 = (disk_t *) queue_remove((queue_t**)&fila_disco,(queue_t*)fila_disco);
        disk_cmd (DISK_CMD_WRITE, aux2->bloco, aux2->buffer);
        disco->status = disk_cmd(DISK_CMD_STATUS,0,0);
      }

      #ifdef DEBUG
      printf("diskDriver: passou ifs \n");
      #endif
    }
    sem_up(acesso);
    tdisk->status = ESPERA;
    #ifdef DEBUG
    printf("diskDriver: sem_up após ifs \n");
    #endif
    queue_append((queue_t**)&suspend,queue_remove((queue_t**)&ready,(queue_t*)tdisk));
    task_yield();
  }
}

int disk_mgr_init (int *num_blocks, int *block_size){
  disco = (disk_t*) malloc (sizeof(disk_t));

  if(disk_cmd(DISK_CMD_INIT,0,0) < 0){
    return -1;
  }

  *block_size = disk_cmd(DISK_CMD_BLOCKSIZE,0,0);

  if(*block_size < 0){
    return -1;
  }

  *num_blocks = disk_cmd (DISK_CMD_DISKSIZE,0,0);

  if(*num_blocks < 0){
    return -1;
  }

  disco->id = atual->id;
  disco->block_tam = *block_size;
  disco->disk_tam =  *num_blocks;
  disco->prev = NULL;
  disco->next = NULL;
  disco->status = disk_cmd(DISK_CMD_STATUS,0,0);

  #ifdef DEBUG
  printf("disk_mgr_init: inicializado disco\n" ); //não aparece mesmo com debug ativo??
  #endif

  return 0;
}

int disk_block_read (int block, void* buffer){
  disk_t *nodo = (disk_t *) malloc (sizeof(disk_t));

  if (!nodo){
    return -1;
  }

  sem_down(acesso);

  nodo->id = atual->id;
  nodo->func = LEITURA;
  nodo->bloco = block;

  #ifdef DEBUG
  printf("disk_block_read: alocado nodo e sem down acesso\n" );
  #endif

  memcpy(nodo->buffer,buffer,nodo->block_tam);
  queue_append((queue_t**)&fila_disco,(queue_t*)nodo);

  if(tdisk->status == ESPERA){
    tdisk->wake_disk = WAKE_DISK;
    queue_append((queue_t**)&ready,queue_remove((queue_t**)&suspend,(queue_t*)tdisk));
    tdisk->status = PRONTA;
    #ifdef DEBUG
    printf("disk_block_read: acordado tdisk\n" );
    #endif
  }

  queue_append((queue_t**)&suspend,queue_remove((queue_t**)&ready,(queue_t*)atual));
  atual->status=ESPERA;
  sem_up(acesso);

  #ifdef DEBUG
  printf("disk_block_read:\n" );
  #endif

  task_yield();
  return 0;
}

int disk_block_write (int block, void* buffer){
  disk_t *nodo = (disk_t *) malloc (sizeof(disk_t));

  if (!nodo){
    return -1;
  }

  sem_down(acesso);

  nodo->id = atual->id;
  nodo->func = ESCRITA;
  nodo->bloco = block;

  memcpy(nodo->buffer,buffer,nodo->block_tam);
  queue_append((queue_t**)&fila_disco,(queue_t*)nodo);

  #ifdef DEBUG
  printf("disk_block_write: alocado nodo e sem down acesso\n" );
  #endif

  if(tdisk->status == ESPERA){
    tdisk->wake_disk = WAKE_DISK;
    queue_append((queue_t**)&ready,queue_remove((queue_t**)&suspend,(queue_t*)tdisk));
    tdisk->status = PRONTA;

    #ifdef DEBUG
    printf("disk_block_write: acordado tdisk\n" );
    #endif
  }

  queue_append((queue_t**)&suspend,queue_remove((queue_t**)&ready,(queue_t*)atual));
  atual->status = ESPERA;
  sem_up(acesso);

  #ifdef DEBUG
  printf("disk_block_write: sem up e suspen\n" );
  #endif

  task_yield();
  return 0;
}
