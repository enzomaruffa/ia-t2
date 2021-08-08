#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "campo.h"

#define MAXSTR 512
#define MAXINT 16

int main(int argc, char **argv) {
  char buf[MAXSTR];
  char campo[MAXSTR]; 
  char lado_meu;
  char lado_adv;
  char mov_adv;
  int tam_campo;
  int pos_filo;
  int pos_bola[MAXINT];
  int num_saltos;
  int i;

  // conecta com o controlador do campo
  campo_conecta(argc, argv);

  // recebe o campo inicial e o movimento do adversario
  campo_recebe(buf);

  // separa os elementos do string recebido
  sscanf(strtok(buf, " \n"), "%c", &lado_meu);
  sscanf(strtok(NULL, " \n"), "%d", &tam_campo);
  sscanf(strtok(NULL, " \n"), "%s", campo);
  sscanf(strtok(NULL, " \n"), "%c", &lado_adv);
  sscanf(strtok(NULL, " \n"), "%c", &mov_adv);
  if(mov_adv != 'n') {
    if(mov_adv == 'f')
      sscanf(strtok(NULL, " \n"), "%d", &pos_filo);
    else {
      sscanf(strtok(NULL, " \n"), "%d", &num_saltos);
      for(i = 0; i < num_saltos; i++)
	sscanf(strtok(NULL, " \n"), "%d", &(pos_bola[i]));
    }
  }

  // mostra o que recebeu
  printf("%c %d %s\n", lado_meu, tam_campo, campo);
  printf("%c %c", lado_adv, mov_adv);
  if(mov_adv != 'n') {
    if(mov_adv == 'f')
      printf(" %d", pos_filo);
    else {
      printf(" %d", num_saltos);
      for(i = 0; i < num_saltos; i++)
	printf(" %d", pos_bola[i]);
    }
  }
  printf("\n");
  
  // prepara um string com o movimento
  // de inserir um filosofo na posicao 1 do campo
  sprintf(buf, "%c f %d\n", lado_meu, 1);

  // envia o movimento para o controlador do campo
  campo_envia(buf);  
}
