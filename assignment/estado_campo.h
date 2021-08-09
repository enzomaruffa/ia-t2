#ifndef __ESTADOCAMPO_H_
#define __ESTADOCAMPO_H_
#include "jogada.h"

typedef struct {
    char lado;
    int tamanho_mapa;
    char *mapa;

    int utilidade_calculada;

    // Se for 0, não é a nossa vez. Se 1, sim.
    char minha_vez;
} EstadoCampo_t;

// Cria um campo a partir do mapa inserido
void cria_campo(EstadoCampo_t *dest, char *mapa, int tamanho_mapa);

// Recebe 2 buffers e calcula todas as jogadas possiveis para um campo
int cria_jogadas_possiveis(EstadoCampo_t *campo,
    JogadaFilosofo_t *jogadas_filosofo, int tamanho_buffer_jogadas_filosofo,
    JogadaBola_t *jogadas_bola, int tamanho_buffer_jogadas_bola,
    int tamanho_jogadas_filosofo, int tamanho_jogadas_bola);

// Aplica uma jogada filosofo em um campo
void aplica_jogada_filosofo(JogadaFilosofo_t *jogada, EstadoCampo_t *campo);

// Aplica uma jogada bola em um campo
void aplica_jogada_bola(JogadaBola_t *jogada, EstadoCampo_t *campo);

#endif