#ifndef __MINIMAX_H_
#define __MINIMAX_H_
#include "jogada.h"
#include "estado_campo.h"

// Calcula o valor de utilidade de um campo
int calcula_utilidade(EstadoCampo_t *estado);

int calcula_utilidade_oponente_bola(EstadoCampo_t *campo,
    JogadaBola_t **jogadas_oponente_bola, int tamanho_buffer_jogadas_bola,
    int *tamanho_jogadas_oponente_bola);

int calcula_utilidade_oponente_filosofo(EstadoCampo_t *campo,
    JogadaFilosofo_t **jogadas_oponente_filosofo, int tamanho_buffer_jogadas_filosofo,
    int *tamanho_jogadas_oponente_filosofo);

int calcula_utilidade_jogadas_filosofo(EstadoCampo_t *campo,
    JogadaFilosofo_t **jogadas_filosofo, int tamanho_buffer_jogadas_filosofo,
    int *tamanho_jogadas_filosofo);

// Escolhe a melhor jogada para um campo. Pode alterar o parâmetro recebido. 
int escolhe_jogada(EstadoCampo_t *estado, RespostaJogada_t *resposta_jogada);

// Minimax
int minimax_inicial(RespostaJogada_t * jogada, EstadoCampo_t *estado, int profundidade, int maximizando);

#endif