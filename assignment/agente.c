#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "campo.h"
#include "minimax.h"
 
#define MAXSTR 512

int main(int argc, char **argv) {
  char buf[MAXSTR];
  char lado_campo;
  int tamanho_campo;
  char *linha;

  // campo_conecta(argc, argv);

  while(1) {
    // campo_recebe(buf);

    linha = readline(NULL);
    if(linha[0] == '0')
      exit(0);
    sprintf(buf, "%s\n", linha);

    // Define as informações da linha
    lado_campo = linha[0];
    tamanho_campo = linha[2] - '0';
    char *mapa = (char *) malloc(sizeof(char) * tamanho_campo);
    for (int i = 0; i < tamanho_campo; ++i) {
      mapa[i] = linha[i+4];
      printf("%c", mapa[i]);
    }
    printf("\n");


    EstadoCampo_t *campo = (EstadoCampo_t *) malloc(sizeof(EstadoCampo_t));
    cria_campo(campo, lado_campo, 1, mapa, tamanho_campo);
    
    printf("campo criado: \n");
    for (int k = 0; k < campo->tamanho_mapa; ++k) {
      printf("%c", campo->mapa[k]);
    }
    printf("\n");
    
    RespostaJogada_t *jogada_final = malloc(sizeof(RespostaJogada_t));
    int utilidade = minimax_inicial(jogada_final, campo, 2, 1);

    printf("utilidade: %d\n", utilidade);
    if (jogada_final->tipo == 1) {
      aplica_jogada_bola(jogada_final->jogada, campo);
      // escreve_jogada_bola();
    } else if(jogada_final->tipo == 0) {
      aplica_jogada_filosofo(jogada_final->jogada, campo);
      // escreve_jogada_filosofo();
    }

    printf("campo_final: ");
    for (int k = 0; k < campo->tamanho_mapa; ++k) {
      printf("%c", campo->mapa[k]);
    }

    printf("\n");
    free(linha);
    free(mapa);
    // destroi_campo(campo);

    // campo_envia(buf);
  }
}
