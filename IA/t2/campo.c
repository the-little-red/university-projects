#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>
#include "campo.h"

redisContext *rediscontext;
redisReply *redisreply;
char campo_lado;
char key[16];

void campo_conecta(int argc, char **argv) {
  char *ip;
  int porta;

  if(argc < 2) {
    printf("formato: %s lado ip porta\n", argv[0]);
    printf("   lado: indica o lado do campo, os valores possívies são e ou d\n");
    printf("   ip: parâmetro opcional que indica o ip ou o hostname do servidor redis\n");
    printf("       o valor default é 127.0.0.1\n");
    printf("   porta: parâmetro opcional que indica a porta do servidor redis\n");
    printf("          o valor default é 10001\n");
    exit(1);
  }

  campo_lado = argv[1][0];
  ip  = (argc > 2) ? argv[2] : "127.0.0.1";
  porta = (argc > 3) ? atoi(argv[3]) : 10001;

  rediscontext = redisConnect(ip, porta);

  if (rediscontext == NULL || rediscontext->err) {
    if(rediscontext) {
      printf("Erro ao conectar com o servidor redis: %s\n", rediscontext->errstr);
      exit(1);
    } else {
      printf("Não foi possível conectar com o servidor redis\n");
    }
  }
}

void campo_envia(char *buffer) {
  sprintf(key, "mov_%c", campo_lado);
  redisreply = redisCommand(rediscontext, "RPUSH %s %s", key, buffer);
  freeReplyObject(redisreply);
}

void campo_recebe(char *buffer) {
  sprintf(key, "campo_%c", campo_lado);
  redisreply = redisCommand(rediscontext, "BLPOP %s 0", key);
  strcpy(buffer, redisreply->element[1]->str);
  freeReplyObject(redisreply);
}
