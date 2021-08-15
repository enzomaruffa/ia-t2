#ifndef __MINIMAX_H_
#define __MINIMAX_H_
#include "jogada.h"
#include "estado_campo.h"

// Calcula o valor de utilidade de um campo
int calcula_utilidade(EstadoCampo_t *estado);

// Escolhe a melhor jogada para um campo. Pode alterar o parâmetro recebido. 
int escolhe_jogada(EstadoCampo_t *estado, RespostaJogada_t *resposta_jogada);

// Minimax
int minimax_inicial(RespostaJogada_t * jogada, EstadoCampo_t *estado, int profundidade, int maximizando);

#endif