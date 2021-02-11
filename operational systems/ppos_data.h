// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.1 -- Julho de 2016
//
// Estruturas de dados internas do sistema operacional
#include <ucontext.h>

#ifndef __PPOS_DATA__
#define __PPOS_DATA__

#define PRONTA 0
#define TERMINADA 1
#define ESPERA 2

#define SYSTEM 0
#define USER 1

#define DESTROY 0
#define ALIVE 2

#define WAKE_DISK 1
#define NOT_WAKE_DISK 0


// Estrutura que define uma tarefa
typedef struct task_t
{
  struct task_t *prev, *next;  // Ṕara usar com a biblioteca de filas(cast)
  int id;                      // ID da tarefa
  ucontext_t contexto;         // Contexto da Tarefa
  int prioestat;              // Prioridade estatica da Tarefa
  int priodina;              // Prioridade dinamica da Tarefa
  int quantum;  //guarda quantum
  short type;
  short status;
  long int active;
  long int exec;
  int exit_code;
  int wake_disk;
  struct task_t *joining_me;
  unsigned long int wakemeup;
  int msg_dono; //caso esteja em suspend,qual fila ele esta preso
} task_t ;

// estrutura que define um semáforo
typedef struct semaphore_t
{
  int counter;
  int status; //saber se o semaforo foi dest ruido ou deu up
  int lock;
  struct task_t *queue;

} semaphore_t;

// estrutura que define uma fila de mensagens
typedef struct mqueue_t
{
  void *buffer; //buffer de msgs
  void *buffer_end; //fim do buffer
  int max_msgs; //numero max de msgs
  int msg_size; //tam das msgs
  int msg_itens; //n de itens no buffer
  void *inicio; //por ser void* o pointer n rola o esquema do site de refs
  void *fim;
  semaphore_t mutex;
  semaphore_t item;
  semaphore_t vaga;

} mqueue_t ;

// estrutura que define um mutex
typedef struct
{
} mutex_t ;

// estrutura que define uma barreira
typedef struct
{
} barrier_t ;

#endif
