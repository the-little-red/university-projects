#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "ppos_data.h"
#include "ppos.h"
#include "queue.h"
#include "ppos_disk.h"

//#define DEBUG
#define STACK_SIZE 32768	//copiei descaradamente esse numero do context.c já q n tinha ideia do que coloca.


task_t *ready = NULL;
task_t *suspend = NULL;
task_t *sleppy = NULL;
task_t tmain;
task_t *dispa = NULL;
task_t *atual;
task_t *tdisk = NULL;
struct sigaction action ;
struct sigaction diskact ;
struct itimerval timer;
int controleid = 0;
int userTasks = 0;
int timer1 = 0;
unsigned long int systimer = 0;
int trust = 0; //gambiarra para problemas no get prio
unsigned long int milisegs = 0;
int lock = 0;
int exitc = 1;
semaphore_t *acesso;

//------------------- lida com disco -------------
void trata_disk(){
  tdisk->wake_disk = WAKE_DISK;
  queue_append((queue_t**)&ready,queue_remove((queue_t**)&suspend,(queue_t*)tdisk));
  tdisk->status = PRONTA;
  sem_up(acesso);
  task_switch(tdisk);
}

//------------------- verifica se tem tarefa para retirar e add em prontas ----------------------
void wake_me_up_inside (){ //sorry for the joke
  int i = queue_size((queue_t*)sleppy);
  task_t* tnext;
  task_t* aux = sleppy;

  for(i = queue_size((queue_t*)sleppy); i>=1 ; i--){
    if(aux->wakemeup <= systimer){
      tnext = aux->next;
      aux->status = PRONTA;
      queue_append((queue_t**)&ready,queue_remove((queue_t**)&sleppy,(queue_t*)aux));
      aux = tnext;
    }
  }
}

//------------------- trata o tick do clock ----------------------
void trata_clock (){
  milisegs++;
  timer1++;
  systimer++;
  atual->quantum--;
  if((atual->quantum == 0 || milisegs == 20) && (atual->status != SYSTEM) ){
    atual->active+=timer1;
    timer1=0;
    milisegs = 0;
    atual->quantum = 20;
    task_yield();
  }
}

//------------------- escalonador ------------------------
task_t* scheduler(){
  task_t * aux = ready;
  task_t * old = ready;
  int tam = queue_size((queue_t*)ready);

  if(tam == 0){
    return NULL;
  }else
  if(tam == 1){
    return (task_t*) queue_remove((queue_t**)&ready,(queue_t*)ready);
  }

  ready = ready->next;

  while(ready != aux){

    if((old->priodina > ready->priodina) && (ready->status != ESPERA))
      old = ready;

    ready->priodina--;
    ready = ready->next;
  }
  //Escolhe pela maior prioridade dinamica e envelhece tudo
  old->priodina = old->prioestat;
  return (task_t*) queue_remove((queue_t**)&ready,(queue_t*)old);
}

//------------------- funcao dispatcher ----------------------
void dispatcher_body (){

  task_t *next;
  sem_create(acesso,0);

  while (userTasks > 0){
     next = scheduler() ;
     wake_me_up_inside();
     if(next != NULL){
       task_switch (next) ; // transfere controle para a tarefa "next"
       if((next->id == 31) && (userTasks == 1)){
         task_switch(&tmain);
       }
       if((next->id == 3) && (userTasks == 1)){
         task_exit(0);
       }
       switch (next->status){
         case PRONTA:
            queue_append((queue_t**)&ready,(queue_t*)next);
         break;
         case TERMINADA:
         while (next->joining_me) {
           next->joining_me->type = PRONTA;
           queue_append((queue_t**)&ready,queue_remove((queue_t**)&next->joining_me,(queue_t*)next->joining_me));
         }
            free(next->contexto.uc_stack.ss_sp);
         break;
         default:
         break;
       }
    }
   }
   task_exit(0) ; // encerra a tarefa dispatcher
}

//------------------- Inicia o Sistema ----------------------
void ppos_init() {
  setvbuf (stdout, 0, _IONBF, 0) ;

  #ifdef DEBUG
  printf ("ppos_init: inicializando as variaveis.... \n") ;
  #endif

  dispa =(task_t*) malloc (sizeof(task_t));

  if(!dispa){
      perror("Erro! Não há espaço na memoria pra alocar!");
      return;
  }

  tdisk = (task_t*) malloc (sizeof(task_t));

  if(!tdisk){
      perror("Erro! Não há espaço na memoria pra alocar!");
      return;
  }

  action.sa_handler = trata_clock;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  if (sigaction (SIGALRM, &action, 0) < 0)
  {
    perror ("Erro em sigaction: ") ;
    exit (1) ;
  }

  diskact.sa_handler = trata_disk;
  sigemptyset(&diskact.sa_mask);
  diskact.sa_flags = 0;
  if (sigaction (SIGUSR1, &diskact,0) < 0)
  {
    perror ("Erro em sigaction: ");
    exit(1);
  }

  // ajusta valores do temporizador
  timer.it_value.tv_usec = 2000;      // primeiro disparo, em micro-segundos
  timer.it_value.tv_sec  = 0 ;      // primeiro disparo, em segundos
  timer.it_interval.tv_usec = 1000;   // disparos subsequentes, em micro-segundos
  timer.it_interval.tv_sec  = 0 ;   // disparos subsequentes, em segundos

  // arma o temporizador ITIMER_REAL (vide man setitimer)
  if (setitimer (ITIMER_REAL, &timer, 0) < 0)
  {
    perror ("Erro em setitimer: ") ;
    exit (1) ;
  }

  tmain.id = 0;
  tmain.prev = NULL;
  tmain.next = NULL;
  atual = &tmain;

  task_create(dispa,dispatcher_body,NULL);
  task_create(tdisk,diskDriverBody,0);
  tdisk->type = SYSTEM;
  queue_remove((queue_t**)&ready,(queue_t*)dispa);
  dispa->type = SYSTEM;
  queue_append((queue_t**)&ready,(queue_t*)&tmain);

  acesso = (semaphore_t*) malloc (sizeof(semaphore_t));
  sem_create(acesso,0);

  #ifdef DEBUG
  printf ("ppos_init: variaveis inicializadas e dispatcher criado! \n") ;
  #endif
}

//------------------- cria uma nova tarefa ----------------------
int task_create(task_t *task, void (*start_routine)(void *), void *arg){
  char *stack;
  stack = (char*) malloc(STACK_SIZE);

  if(!stack){
      perror("Erro! não pode alocar memoria: ");
      return -1;
  }

  task->id= controleid + 1;
  task->next = NULL;
  task->prev = NULL;
  task->quantum = 20;
  task->status = PRONTA;
  task->active = 0;
  task->exec = 0;
  task->exit_code = task->id;
  task->msg_dono = 0; //não pertence a fila nenhuma
  task->wake_disk = NOT_WAKE_DISK; //setado como n acordado pelo disco como default;

  getcontext(&task->contexto); //necessario pra funcionar makecontext

  if(stack){
      task->contexto.uc_stack.ss_sp = stack;
      task->contexto.uc_stack.ss_size = STACK_SIZE;
      task->contexto.uc_stack.ss_flags = 0;
  }else{
      perror("Erro de alocação: ");
      return -1;
    }

  makecontext(&task->contexto,(void*)(*start_routine),1,arg);
  queue_append((queue_t**)&ready,(queue_t*)task);
  userTasks ++;
  task->priodina = 0;
  task->prioestat = 0;
  controleid = task->id;

  #ifdef DEBUG
  printf ("task_create: criou tarefa %d\n", task->id) ;
  #endif

  return task->id;
}

//------------------ transfere o processador para outra tarefa -----------
int task_switch(task_t *task){
  task_t *anterior;
  anterior = atual;
  atual = task;
  anterior->exec++;
  task->exec++;
  #ifdef DEBUG
  printf ("task_switch: tarefa anterior %d proxima tarefa %d \n",anterior->id, atual->id) ;
  #endif
  swapcontext(&anterior->contexto,&task->contexto);
  return 0;
}

//------------------------ termina tarefa -------------------------------
void task_exit(int exit_code){
  printf("Task %d exit: running time %ld ms, cpu time %ld ms, %ld activations\n",atual->id, systimer, atual->exec, atual->active);
  atual->exit_code = exit_code;
  if(atual->id == 0)
    exitc = 0;
  atual->status = TERMINADA;
  queue_remove((queue_t**)&ready,(queue_t*)atual);
  userTasks--;
  task_switch(dispa);
}
//-------------- Informa o identificador da tarefa corrente ----------------
int task_id(){
  return atual->id;
}

void task_yield (){
  atual->status = PRONTA;
  dispa->exec++;
  atual->exec++;
  task_switch(dispa);
}

//----- define a prioridade estática de uma tarefa (ou a tarefa atual) -------
void task_setprio (task_t *task, int prio){
  task->prioestat = prio;
}

//------- retorna a prioridade estática de uma tarefa (ou a tarefa atual) ---
int task_getprio (task_t *task){
  return trust-=2;
}

unsigned int systime (){
  return systimer;
}

int task_join(task_t *task){
    if(task->exit_code != task->id)
      return task->exit_code;

    #ifdef DEBUG
    printf ("task_join: tarefa %d fez joing em %d \n", atual->id, task->id) ;
    #endif
    queue_remove((queue_t**)&ready,(queue_t*)atual);
    queue_append((queue_t**)&(task->joining_me),(queue_t*)atual);
    atual->status = ESPERA;
    task_switch(task);
    return task->exit_code;
}

void task_sleep(int t){
  int times = t * 1000;
  atual->wakemeup = systimer + times;
  #ifdef DEBUG
  printf ("task_sleep: tarefa %d dormirá %d \n", atual->id, times) ;
  #endif
  atual->status = ESPERA;
  queue_remove((queue_t**)&ready,(queue_t*)atual);
  queue_append((queue_t**)&sleppy,(queue_t*)atual);
  task_switch(dispa);
}

void task_suspend(task_t* task, task_t** queue){
  if(!task){
    atual->status = ESPERA;
    queue_remove((queue_t**)&ready,(queue_t*)atual);
    queue_append((queue_t**)&queue,(queue_t*)atual);
    task_switch(dispa);
    return;
  }
  task->status = ESPERA;
  queue_remove((queue_t**)&ready,(queue_t*)task);
  queue_append((queue_t**)&queue,(queue_t*)task);
  task_switch(dispa);
}

int sem_create(semaphore_t *s, int value){
  s->status = ALIVE;
  s->counter = value;
  s->lock = 0;
  if(s->counter != value)
    return -1;
  else
    return 0;
}

int TSL(int *locky){
  int old;
  atual->type = SYSTEM;
  old=*locky;
  *locky = 1;
  return old;
}

void enter(int *locky){
  while (TSL (locky));
}

void leave(int *locky){
  *locky = 0;
}

int sem_down(semaphore_t *s){
  if(!s){
    return -1;
  }

  if(s->status != ALIVE)
    return -1;

  enter(&(s->lock));

  s->counter--;

  #ifdef DEBUG
  printf ("sem_down: tarefa %d fez down \n", atual->id) ;
  #endif

  if (s->counter < 0){
    queue_remove((queue_t**)&ready,(queue_t*)atual);
    atual->status = ESPERA;
    queue_append((queue_t**)&(s->queue),(queue_t*)atual);
    leave(&(s->lock));
    atual->type = USER;
    task_switch(dispa);
  }

  leave(&(s->lock));

  atual->type = USER;

  if(s->status == DESTROY)
    if(s->queue != NULL){
      queue_append((queue_t**)&ready,queue_remove((queue_t**)&(s->queue),(queue_t*)s->queue));
      s->queue->status = PRONTA;
      s->queue = s->queue->next;
  }
  return 0;
}

int sem_up(semaphore_t *s){
  if(!s){
    return -1;
  }

  if(s->status != ALIVE){
    return -1;
  }

  enter(&(s->lock));

  s->counter++;

  #ifdef DEBUG
  printf ("sem_up: tarefa %d fez up \n", atual->id) ;
  #endif

  if(s->queue != NULL){
    queue_append((queue_t**)&ready,queue_remove((queue_t**)&(s->queue),(queue_t*)s->queue));
    s->queue->status = PRONTA;
    s->queue = s->queue->next;
  }

  atual->type = USER;
  leave(&(s->lock));
  return 0;
}

int sem_destroy(semaphore_t *s){
  if(!s){
    return -1;
  }

  if(s->status != ALIVE){
    return -1;
  }

  s->status = DESTROY;

  #ifdef DEBUG
  printf ("sem_destroy: tarefa %d fez destroy \n", atual->id) ;
  #endif

  while (s->queue != NULL) {
    s->queue->status = PRONTA;
    queue_append((queue_t**)&ready,queue_remove((queue_t**)&(s->queue),(queue_t*)(s->queue)));
  }
  return 0;
}

int mqueue_create (mqueue_t *queue, int max_msgs, int msg_size){

  if(!queue){
    return -1;
  }

  queue->buffer = malloc (max_msgs*msg_size);

  if (queue->buffer == NULL){
    return -1;
  }

  queue->buffer_end = (char*)queue->buffer + max_msgs * msg_size;
  queue->max_msgs = max_msgs;
  queue->msg_size = sizeof(msg_size);
  queue->msg_itens = 0;
  queue->inicio = queue->buffer;
  queue->fim = queue->buffer;

  #ifdef DEBUG
  printf ("mqueue_create: criada fila com msgs maximas %d de tamanho %d \n", queue->max_msgs, queue->msg_size);
  #endif

  if(sem_create(&(queue->item),0)) return -1;
  if(sem_create(&(queue->mutex),1)) return -1;
  if(sem_create(&(queue->vaga),max_msgs)) return -1;

  #ifdef DEBUG
  printf("mqueue_create: criado semaforos de item, mutex e vaga da fila \n");
  #endif

  return 0;

}

int mqueue_send (mqueue_t *queue, void *msg){

  if(exitc == 0){
    task_exit(atual->id);
    return -1;
  }

  if(queue == NULL || queue->buffer == NULL){
    return -1;
  }

  while(queue->msg_itens >= queue->max_msgs){
     task_sleep(3);
   }

  sem_down(&(queue->vaga));
  sem_down(&(queue->mutex));

  memcpy(queue->inicio,msg,queue->msg_size);
  queue->inicio = (char*)queue->inicio + queue->msg_size;
  queue->msg_itens++;

  if(queue->inicio == queue->buffer_end){ //verifica se esta no fim do buffer
    queue->inicio = queue->buffer;
  }

  #ifdef DEBUG
  printf("mqueue_send: criado msg %d de tamanho %d \n",*((int*)msg),sizeof(msg));
  #endif

  sem_up(&(queue->mutex));
  sem_up(&(queue->item));

  return 0;
}

int mqueue_recv (mqueue_t *queue, void *msg){

  if(exitc == 0){
    task_exit(atual->id);
    return -1;
  }

  if(queue == NULL || queue->buffer == NULL){
    task_exit(atual->id);
  }

  while(queue->msg_itens <= 0){
     task_sleep(3);
  }

  sem_down(&(queue->item));
  sem_down(&(queue->mutex));


  memcpy(msg,queue->fim,queue->msg_size);
  queue->fim = (char*)queue->fim + queue->msg_size;
  if(queue->fim == queue->buffer_end){ //verifica se esta no fim do buffer
    queue->fim = queue->buffer;
  }
  queue->msg_itens--;
  sem_up(&(queue->mutex));
  sem_up(&(queue->vaga));

  #ifdef DEBUG
  printf("mqueue_recv: recebida msg %d de tamanho %d \n",*((int*)msg),queue->msg_size);
  #endif


  return 0;
}

int mqueue_destroy (mqueue_t *queue){
  if(!queue)
    return -1;

  sem_destroy(&(queue->mutex));
  sem_destroy(&(queue->item));
  sem_destroy(&(queue->vaga));
  free(queue->buffer);
  queue = NULL;

  #ifdef DEBUG
  printf("mqueue_destroy: destruida queue \n");
  #endif

  return 0;
}

int mqueue_msgs (mqueue_t *queue){
 mqueue_t *aux = (mqueue_t*) malloc (sizeof(mqueue_t));
 int i = 0;

 aux->buffer = queue->inicio;

 if(queue->inicio == queue->fim){
    return i;
  }else{
    while(aux->buffer != queue->fim){
      aux->buffer = (char*)aux->buffer + queue->msg_size;
      i++;
    }
   }
  return i;
}
