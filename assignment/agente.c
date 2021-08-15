#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "campo.h"
#include "minimax.h"
 
#define MAXSTR 512

int main(int argc, char **argv) {
  char buffer[MAXSTR];
  char *linha;

  // campo_conecta(argc, argv);
  while(1) {

    // campo_recebe(buf);
    linha = readline(NULL);
    if(linha[0] == '0')
      exit(0);
    sprintf(buffer, "%s\n", linha);
    
    char *pch;

    // Pega o lado
    pch = strtok(buffer, " ");
    if (pch == NULL) { return 1; }

    char lado_campo = 'e';
    lado_campo = *pch;

    // Pega o tamanho do mapa
    pch = strtok(NULL, " ");
    if (pch == NULL) { return 1; }

    // Pega a largura do campo
    int tamanho_campo = 0;
    tamanho_campo = atoi(pch);

    pch = strtok(NULL, " ");
    if (pch == NULL) { return 1; }

    // Define as informações da linha
    char *mapa = (char *) malloc(sizeof(char) * tamanho_campo);
    for (int i = 0; i < tamanho_campo; ++i) {
      mapa[i] = pch[i];
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
    int utilidade = minimax_inicial(jogada_final, campo, 4, 1);

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
