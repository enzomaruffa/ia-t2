#include <stdio.h>
#include <stdlib.h>
#include "jogada.h"

int escreve_jogada_filosofo(char lado, JogadaFilosofo_t *jogada, char *vetor, int tamanho_vetor) {
    if (tamanho_vetor < 6) { // 2 para o lado, 2 para o f, 2 para o número
        return 1;
    }

    sprintf(vetor, "%c f %d", lado, jogada->posicao);
}

void destroi_jogada_filosofo(JogadaFilosofo_t *jogada) {
    free(jogada);
}

int escreve_jogada_bola(char lado, JogadaBola_t *jogada, char *vetor, int tamanho_vetor) {
    if (tamanho_vetor < 4 + (jogada->pulos * 2)) { // 2 para o lado, 2 para o o, 2 para cada chute
        return 
        1;
    }

    sprintf(vetor, "%c o", lado);

    // TODO: Sinceras nem ideia se isso funciona. Testei em um compilador online e deu boa
    for (int i = 0; i < jogada->pulos; i++) {
        sprintf(vetor, "%s %d", vetor, jogada->posicao_pulos[i]);
    }
}

void destroi_jogada_bola(JogadaBola_t *jogada) {
    free(jogada->posicao_pulos);
    free(jogada);
}