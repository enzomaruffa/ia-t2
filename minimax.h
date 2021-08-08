#ifndef __MINIMAX_H_
#define __MINIMAX_H_
#include "jogada.h"
#include "campo.h"

// Calcula o valor de utilidade de um campo
void calcula_utilidade(EstadoCampo_t *estado);

// Atualiza o valor de utilidade de um campo
void atualiza_utilidade(EstadoCampo_t *estado);

// Escolhe a melhor jogada para um campo. Pode alterar o parâmetro recebido. 
int escolhe_jogada(EstadoCampo_t *estado, RespostaJogada_t *resposta_jogada);

#endif