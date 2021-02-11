// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.1 -- Julho de 2016
//
// interface de acesso ao disco rígido

#ifndef __PPOS_DISK__
#define __PPOS_DISK__

#define LEITURA 1
#define ESCRITA 2

// structura de dados que representa o disco para o SO
typedef struct disk_t
{
  struct disk_t *prev,*next;
  int id; //id da task que esta usando o disco.
  int func; //leitura ou escrita
  int bloco; //bloco para leitura
  int disk_status; //status do disco.
  int block_tam; //tamanho dos blocos.
  int disk_tam; //tamanho do disco em blocos.
  int status;
  void *buffer; //buffer de leitura

} disk_t ;

// inicializacao do gerenciamento de disco
// retorna -1 em erro ou 0 em sucesso
// num_blocks: tamanho do disco, em blocos
// block_size: tamanho de cada bloco do disco, em bytes
int disk_mgr_init (int *num_blocks, int *block_size) ;

// leitura de um bloco, do disco para o buffer indicado
int disk_block_read (int block, void *buffer) ;

// escrita de um bloco, do buffer indicado para o disco
int disk_block_write (int block, void *buffer) ;

void diskDriverBody (void *args);

#endif
