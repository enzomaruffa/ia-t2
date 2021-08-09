#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "campo.h"
#include "estado_campo.h"
#include "minimax.h"

#define MAXSTR 512

int main(int argc, char **argv) {
  char buf[MAXSTR];
  char *linha;

  campo_conecta(argc, argv);

  while(1) {
    campo_recebe(buf);

    linha = readline(NULL);
    if(linha[0] == '0')
      break;
    sprintf(buf, "%s\n", linha);

    free(linha);

    campo_envia(buf);
  }
}
