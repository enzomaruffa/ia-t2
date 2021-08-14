#ifndef __ESTADOCAMPO_H_
#define __ESTADOCAMPO_H_
#include "jogada.h"

typedef struct {
    // Se for 'e', esquerdo. 'd', direito
    char meu_lado;
    int tamanho_mapa;

    // Vetor de caracteres com o mapa. 0 é um gol, tamanho_mapa - 1 é outro
    char *mapa;

    int utilidade_calculada;

    // Se for 0, não é a nossa vez. Se 1, sim.
    char minha_vez;
} EstadoCampo_t;

// Cria um campo a partir do mapa inserido
void cria_campo(EstadoCampo_t *dest, char meu_lado, char minha_vez, char *mapa, int tamanho_mapa);

// Clona um campo
void clona_campo(EstadoCampo_t *dest, EstadoCampo_t *campo);

// Destrói um campo da memória
void destroi_campo(EstadoCampo_t *campo);

// Adiciona as jogadas_bola possíveis
void adiciona_jogadas_bola(int lado_diff, EstadoCampo_t *campo, int posicao_bola, 
                        JogadaBola_t **jogadas_bola, int tamanho_buffer_jogadas_bola,
                        int *tamanho_jogadas_bola);

// Recebe 2 buffers e calcula todas as jogadas possiveis para um campo
int cria_jogadas_possiveis(EstadoCampo_t *campo,
    JogadaFilosofo_t **jogadas_filosofo, int tamanho_buffer_jogadas_filosofo,
    JogadaBola_t **jogadas_bola, int tamanho_buffer_jogadas_bola,
    int *tamanho_jogadas_filosofo, int *tamanho_jogadas_bola);

// Aplica uma jogada filosofo em um campo
int aplica_jogada_filosofo(JogadaFilosofo_t *jogada, EstadoCampo_t *campo);

// Aplica uma jogada bola em um campo
int aplica_jogada_bola(JogadaBola_t *jogada, EstadoCampo_t *campo);

#endif