#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jogada.h"

int escreve_jogada_filosofo(char lado, JogadaFilosofo_t *jogada, char *vetor, int tamanho_vetor) {
    if (tamanho_vetor < 6) { // 2 para o lado, 2 para o f, 2 para o número
        return 1;
    }

    sprintf(vetor, "%c f %d", lado, jogada->posicao);
    return 0;
}

// Clona uma jogada filósofo
int clona_jogada_filosof(JogadaFilosofo_t *dest, JogadaFilosofo_t *jogada) {
    dest->posicao = jogada->posicao;
    return 0;
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
    return 0;
}

// Clona uma jogada bola
int clona_jogada_bola(JogadaBola_t *dest, JogadaBola_t *jogada) {
    dest->pulos = jogada->pulos;
    dest->resulta_em_gol = jogada->resulta_em_gol;
    dest->posicao_pulos = malloc(sizeof(int) * dest->pulos);
    memcpy(dest->posicao_pulos, jogada->posicao_pulos, sizeof(int) * dest->pulos);
    return 0;
}

void destroi_jogada_bola(JogadaBola_t *jogada) {
    free(jogada->posicao_pulos);
    free(jogada);
}