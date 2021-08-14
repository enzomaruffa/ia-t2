#include <stdlib.h>
#include <stdio.h>
  
#include "minimax.h"

// Ideia no momento: o valor de utilidade do estado é o quão distante a bola pode ficar do nosso gol - o quão distante do gol deles
int calcula_utilidade(EstadoCampo_t *estado) {
    int posicao_bola = -1;
    for (int i = 0; i < estado->tamanho_mapa; i++) {
        if (estado->mapa[i] == 'o') {
            posicao_bola = i;
        }
    }
    
    int tamanho_buffer_jogadas_bola = estado->tamanho_mapa;
    JogadaBola_t **jogadas_bola = malloc(sizeof(JogadaBola_t) * estado->tamanho_mapa);
    int tamanho_jogadas_bola = 0;

    // Gera jogadas da bola
    if (posicao_bola != -1) {
        // Olha para a direita
        adiciona_jogadas_bola(1, estado, posicao_bola, jogadas_bola, tamanho_buffer_jogadas_bola, &tamanho_jogadas_bola);
        // Olha para a esquerda
        adiciona_jogadas_bola(-1, estado, posicao_bola, jogadas_bola, tamanho_buffer_jogadas_bola, &tamanho_jogadas_bola);
    }

    int meu_gol = estado->meu_lado == 'e' ? 0 : estado->tamanho_mapa;

    int maior_distancia_meu_gol = abs(meu_gol - posicao_bola);
    int menor_distancia_meu_gol = abs(meu_gol - posicao_bola);

    for (int i = 0; i < tamanho_jogadas_bola; i++) {
        JogadaBola_t *jogada = jogadas_bola[i];

        int posicao_aterrisagem = jogada->posicao_pulos[jogada->pulos - 1];
        int distancia_meu_gol = abs(meu_gol - posicao_aterrisagem);

        if (distancia_meu_gol > maior_distancia_meu_gol) {
            maior_distancia_meu_gol = distancia_meu_gol;
        } else if (distancia_meu_gol < menor_distancia_meu_gol) {
            menor_distancia_meu_gol = distancia_meu_gol;
        }
    }

    // Caso vá perder, aumenta em 1000
    if (menor_distancia_meu_gol == 0) {
        menor_distancia_meu_gol += 1000;
    }

    // Caso vá ganhar, aumenta em 1000
    if (maior_distancia_meu_gol == estado->tamanho_mapa) {
        menor_distancia_meu_gol += 1000;
    }

    free(jogadas_bola);
    return maior_distancia_meu_gol - (menor_distancia_meu_gol * 2);
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
