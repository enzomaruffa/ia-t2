#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>

#define MAXSTR 512
#define MAXINT 16
#define OUTRO(l) (l=='d'?'e':'d')

redisContext* inicia(int argc, char **argv, char *lado, int *tam,
		     int *jogadas, char *tempo) {
  redisContext *rediscontext;
  char *ip;
  int porta;
  
  if(argc < 5) {
    printf("formato:\n");
    printf("         %s lado tam jogadas tempo [ip porta]\n\n", argv[0]);
    printf("   lado: lado do campo que inicia, os valores possívies são e ou d\n");
    printf("   tam: tamanho do campo, deve ser impar\n");
    printf("   jogadas: número máximo do jogadas na partida\n");
    printf("   tempo: limite em segundos para cada jogada, 0 indica sem limite\n");
    printf("   ip: parâmetro opcional que indica o ip ou o hostname do servidor redis\n");
    printf("       o valor default é 127.0.0.1\n");
    printf("   porta: parâmetro opcional que indica a porta do servidor redis\n");
    printf("          o valor default é 10001\n");
    exit(1);
  }
  
  *lado = argv[1][0];
  *tam = atoi(argv[2]);
  *jogadas = atoi(argv[3]);
  strcpy(tempo, argv[4]);
  ip  = (argc > 5) ? argv[5] : "127.0.0.1";
  porta = (argc > 6) ? atoi(argv[6]) : 10001;

  rediscontext = redisConnect(ip, porta);
  if (rediscontext == NULL || rediscontext->err) {
    if(rediscontext) {
      printf("Erro ao conectar com o servidor redis: %s\n", rediscontext->errstr);
      exit(1);
    } else {
      printf("Não foi possível conectar com o servidor redis\n");
    }
  }
  return rediscontext;
}  

int parse_mov(char *buf, char *rl, char *rm, int *rf, int *rs, int *rb) {
  int i;
  char *s;

  if(!(s = strtok(buf, " \n")) || sscanf(s , "%c", rl) != 1)
    return 0;
  if(*rl != 'e' && *rl != 'd')
    return 0;
  if(!(s = strtok(NULL, " \n")) || sscanf(s, "%c", rm) != 1)
    return 0;
  if(*rm != 'n' && *rm != 'f' && *rm != 'o')
    return 0;
  if(*rm != 'n') {
    if(*rm == 'f') {
      if(!(s = strtok(NULL, " \n")) || sscanf(s, "%d", rf) != 1)
	return 0;
    }
    else {
      if(!(s = strtok(NULL, " \n")) || sscanf(s, "%d", rs) != 1)
	return 0;
      for(i = 0; i < *rs; i++)
	if(!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(rb[i])) != 1)
	  return 0;
    }
  }
  return 1;
}

int aplica_mov(char *buf, char *campo, int tam,
	       char rl, char rm, int rf, int rs, int *rb) {
  int i, j, inc, bola;

  strcpy(buf, campo);
  if(rm == 'n')
    return 1;
  if(rm == 'f') {
    if(--rf < 0 || rf >= tam)
      return 0;
    if(buf[rf] == '.') {
      buf[rf] = 'f';
      return 1;
    }
    return 0;
  }
  if(rm == 'o') {
    if(rs < 1)
      return 0;
    for(bola = 0; bola < tam; bola++)
      if(buf[bola] == 'o')
	break;
    for(i = 0; i < rs; i++) {
      if(bola < 0 || bola >= tam)
	break;
      inc = (--rb[i] < bola) ? -1 : 1;
      for(j = bola + inc; j != rb[i]; j += inc)
	if(buf[j] != 'f')
	  return 0;
        else
	  buf[j] = '.';
      buf[bola] = '.';
      bola = rb[i];
      if(bola >= 0 && bola < tam) {
	if(buf[bola] == '.')
	  buf[bola] = 'o';
	else
	  return 0;
      }
      if(bola < 0 && rl == 'e')
	return 0;
      if(bola >= tam && rl == 'd')
	return 0;
    }
    if(i == rs)
      return 1;
  }
  return 0;
}

int gol(char *campo, int tam) {
  int i;

  for(i = 0; i < tam; i++)
    if(campo[i] == 'o')
      return 0;
  return 1;
}

void limpa_campo(char *campo, int tam) {
  int i;

  for(i = 0; i < tam; i++)
    campo[i] = '.';
  campo[tam] = 0;
}

int main(int argc, char **argv) {
  redisContext *c;
  redisReply *r;
  char buf[MAXSTR];
  char campo[MAXSTR];
  char mov[MAXSTR];
  char key[16];
  char timeout[16];
  char lado;
  char vencedor;
  char rl;
  char rm;
  int rf;
  int rs;
  int rb[MAXINT];
  int jogadas;
  int tam;
  int ok;
  
  c = inicia(argc, argv, &lado, &tam, &jogadas, timeout);

  vencedor = ' ';
  limpa_campo(campo, tam);
  campo[tam/2] = 'o';

  printf("%d: %s\n", jogadas, campo);

  sprintf(buf, "%c %d %s\n%c n\n", lado, tam, campo, OUTRO(lado));

  while(jogadas) {
    sprintf(key, "campo_%c", lado);
    r = redisCommand(c, "LTRIM %s 1 0", key);
    freeReplyObject(r);
    r = redisCommand(c, "RPUSH %s %s", key, buf);
    freeReplyObject(r);
    
    ok = 0;
    sprintf(key, "mov_%c", lado);
    r = redisCommand(c, "BLPOP %s %s", key, timeout);
    if(r->type != REDIS_REPLY_NIL) {
      strcpy(mov, r->element[1]->str);
      if(parse_mov(mov, &rl, &rm, &rf, &rs, rb) &&
	 aplica_mov(buf, campo, tam, rl, rm, rf, rs, rb)) {
	  strcpy(campo, buf);
	  strcpy(mov, r->element[1]->str);
	  ok = 1;
	}
    }
    freeReplyObject(r);
    if(!ok)
      sprintf(mov, "%c n\n", lado);

    printf("%d: %s", jogadas, mov);
    printf("%d: %s\n", jogadas, campo);

    if(gol(campo, tam)) {
      printf("%d: gol de %c\n", jogadas, lado);
      vencedor = lado;
      break;
    }
    
    lado = OUTRO(lado);
    sprintf(buf, "%c %d %s\n%s", lado, tam, campo, mov);
    jogadas--;
  }

  limpa_campo(campo, tam);
  
  sprintf(buf, "e %d %s\nd n\n", tam, campo);
  r = redisCommand(c, "RPUSH campo_e %s", buf);
  freeReplyObject(r);

  sprintf(buf, "d %d %s\ne n\n", tam, campo);
  r = redisCommand(c, "RPUSH campo_d %s", buf);
  freeReplyObject(r);

  if(jogadas == 0)
    printf("empate\n");
  else    
    printf("vencedor: %c\n", vencedor);
}
