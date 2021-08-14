#include <stdlib.h>
#include <stdio.h>
  
#include "minimax.h"

int calcula_utilidade(EstadoCampo_t *estado) {
    return 1;
}

int calcula_utilidade_oponente_bola(EstadoCampo_t *campo,
    JogadaBola_t **jogadas_oponente_bola, int tamanho_buffer_jogadas_bola,
    int *tamanho_jogadas_oponente_bola) {
    int menor_utilidade = 10000;

    printf("tamanho_jogadas_oponente_bola: %d\n", *tamanho_jogadas_oponente_bola);
    for(int i = 0; i < *tamanho_jogadas_oponente_bola; ++i) {
        // copia campo
        EstadoCampo_t *campo_aux_oponente_bola = (EstadoCampo_t *) malloc(sizeof(EstadoCampo_t));
        clona_campo(campo_aux_oponente_bola, campo);

        // realiza jogada 
        aplica_jogada_bola(jogadas_oponente_bola[i], campo_aux_oponente_bola);

        // calcula utilidade das jogadas do oponente
        int utilidade_atual = calcula_utilidade(campo_aux_oponente_bola);

        // pega a menor utilidade deles e coloca na nossa jogada atual
        if(menor_utilidade > utilidade_atual) {
            menor_utilidade = utilidade_atual;
        }
    }
    return menor_utilidade;
}

int calcula_utilidade_oponente_filosofo(EstadoCampo_t *campo,
    JogadaFilosofo_t **jogadas_oponente_filosofo, int tamanho_buffer_jogadas_filosofo,
    int *tamanho_jogadas_oponente_filosofo) {
    int menor_utilidade = 10000;
    printf("tamanho_jogadas_oponente_filosofo: %d\n", *tamanho_jogadas_oponente_filosofo);
    for(int j = 0; j < *tamanho_jogadas_oponente_filosofo; ++j) {
        // copia campo
        EstadoCampo_t *campo_aux_oponente_filosofo = (EstadoCampo_t *) malloc(sizeof(EstadoCampo_t));
        clona_campo(campo_aux_oponente_filosofo, campo);

        // realiza jogada 
        aplica_jogada_filosofo(jogadas_oponente_filosofo[j], campo_aux_oponente_filosofo);
        
        // calcula utilidade das jogadas do oponente
        int utilidade_atual = calcula_utilidade(campo_aux_oponente_filosofo);
        // pega a menor utilidade deles e coloca na nossa jogada atual
        if(menor_utilidade > utilidade_atual) {
            menor_utilidade = utilidade_atual;
        }
    }
    return menor_utilidade;
}
