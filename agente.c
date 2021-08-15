#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "campo.h"
#include "minimax.h"
 
#define MAXSTR 512

int main(int argc, char **argv) {
  char buffer[MAXSTR];
  // char *linha;

  campo_conecta(argc, argv);
  while(1) {

    campo_recebe(buffer);
    printf("RECEBEU [%s]", buffer);
    printf("\n");
    // linha = readline(NULL);
    // if(linha[0] == '0')
    //   exit(0);
    // sprintf(buffer, "%s\n", linha);
    

    // LEITURA
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
      // printf("%c", mapa[i]);
    }
    // printf("\n");

    EstadoCampo_t *campo = (EstadoCampo_t *) malloc(sizeof(EstadoCampo_t));
    cria_campo(campo, lado_campo, 1, mapa, tamanho_campo);

    // Verifica se acabou: 

    int posicao_bola = -1;
    for (int i = 0; i < campo->tamanho_mapa; i++) {
        if (campo->mapa[i] == 'o') {
            posicao_bola = i;
        }
    }

    if (posicao_bola < 0) {
      return 0;
    }
    
    // printf("campo criado: \n");
    // for (int k = 0; k < campo->tamanho_mapa; ++k) {
    //   printf("%c", campo->mapa[k]);
    // }
    // printf("\n");

    int quantidade_vazio = 0;
    for (int k = 0; k < campo->tamanho_mapa; ++k) {
      if (campo->mapa[k] == '.') {
        quantidade_vazio += 1;
      }
    }

    int profundidade = 8;

    printf("quantidade_vazio %d", quantidade_vazio);

    if (quantidade_vazio > 14) {
      profundidade = 7;
    };
    
    RespostaJogada_t *jogada_final = malloc(sizeof(RespostaJogada_t));

    clock_t start;
    clock_t end;
    float seconds;

    // ==== 
    start = clock();
    alphabeta_inicial(jogada_final, campo, profundidade, -1000000, 1000000, 1);

    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("Total time: %f\n", seconds);

    // printf("utilidade: %d\n", utilidade);
    // if (jogada_final->tipo == 1) {
    //   aplica_jogada_bola(jogada_final->jogada, campo);
      // escreve_jogada_bola();
    // } else if(jogada_final->tipo == 0) {
    //   aplica_jogada_filosofo(jogada_final->jogada, campo);
      // escreve_jogada_filosofo();
    // }

    // printf("campo_final: ");
    // for (int k = 0; k < campo->tamanho_mapa; ++k) {
    //   printf("%c", campo->mapa[k]);
    // }

    memset(buffer, ' ', MAXSTR);

    if (jogada_final->tipo == 1) {
      escreve_jogada_bola(campo->meu_lado, (JogadaBola_t *)jogada_final->jogada, buffer, MAXSTR);
      destroi_jogada_bola(jogada_final->jogada);
    } else if(jogada_final->tipo == 0) {
      escreve_jogada_filosofo(campo->meu_lado, (JogadaFilosofo_t *)jogada_final->jogada, buffer, MAXSTR);
      destroi_jogada_filosofo(jogada_final->jogada);
    }; 

    printf("ENVIANDO [%s]", buffer);
    printf("\n");

    free(jogada_final);
    destroi_campo(campo);

    campo_envia(buffer);
  }
}
