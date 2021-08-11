#include <stdlib.h>
#include <string.h>
#include "estado_campo.h"

void cria_campo(EstadoCampo_t *dest, char meu_lado, char minha_vez, char *mapa, int tamanho_mapa) {
    dest->meu_lado = meu_lado;
    dest->minha_vez = minha_vez;
    dest->tamanho_mapa = tamanho_mapa + 2; // Adiciona posição 0 e K+1
    dest->utilidade_calculada = 0;

    dest->mapa = malloc(sizeof(char) * dest->tamanho_mapa);
    memcpy(dest->mapa + 1, mapa, sizeof(char) * tamanho_mapa);

    // Seta a posição 0 e K + 1 para serem o gol 'g'
    dest->mapa[0] = 'g';
    dest->mapa[tamanho_mapa + 1] = 'g';
}

void clona_campo(EstadoCampo_t *dest, EstadoCampo_t *campo) {
    dest->meu_lado = campo->meu_lado;
    dest->minha_vez = campo->minha_vez;
    dest->tamanho_mapa = campo->tamanho_mapa;
    dest->utilidade_calculada = campo->utilidade_calculada;

    dest->mapa = malloc(sizeof(char) * dest->tamanho_mapa);
    memcpy(dest->mapa, campo->mapa, sizeof(char) * dest->tamanho_mapa);
}

int cria_jogadas_possiveis(EstadoCampo_t *campo,
    JogadaFilosofo_t *jogadas_filosofo, int tamanho_buffer_jogadas_filosofo,
    JogadaBola_t *jogadas_bola, int tamanho_buffer_jogadas_bola,
    int tamanho_jogadas_filosofo, int tamanho_jogadas_bola) {
    // TODO
    return 0;
}

// 0 = ok
// 1 = erro
int aplica_jogada_filosofo(JogadaFilosofo_t *jogada, EstadoCampo_t *campo) {
    if (jogada->posicao < 0 || jogada->posicao > (campo->tamanho_mapa - 1) || campo->mapa[jogada->posicao] != '.') {
        return 1;
    }

    campo->mapa[jogada->posicao] = 'f';

    return 0;
}

/** 
 * 0 = ok
 * 1 = erro
 * 2 = ganha jogo
 * Não valida que a jogada é válida!
 */
int aplica_jogada_bola(JogadaBola_t *jogada, EstadoCampo_t *campo) {
    for (int i = 0; i < jogada->pulos; i++) {
        int posicao = jogada->posicao_pulos[i];
        if (posicao < 0 || posicao > (campo->tamanho_mapa - 1) || campo->mapa[posicao] == 'f' || campo->mapa[posicao] == 'o') {
            return 1;
        }
    }

    int posicao_atual_bola = -1;
    for (int i = 0; i < campo->tamanho_mapa; i++) {
        if (campo->mapa[i] == 'o') {
            posicao_atual_bola = i;
            break;
        }
    }

    int ultima_posicao_bola = jogada->posicao_pulos[jogada->pulos - 1];

    // Se a posicao final tiver um g, seta ganhou pra 1
    int ganhou = campo->mapa[ultima_posicao_bola] == 'g';

    int start = posicao_atual_bola > ultima_posicao_bola ? ultima_posicao_bola : ultima_posicao_bola;
    int end = posicao_atual_bola > ultima_posicao_bola ? posicao_atual_bola : ultima_posicao_bola;

    for (int i = start; i < end + 1; i++) {
        if (campo->mapa[i] == ultima_posicao_bola) {
            // Coloca a bola na posicao final
            campo->mapa[i] = 'o';
        } else {
            // Passa transformando todos os f e o em .
            campo->mapa[i] = '.';
        }
    } 
    
    return ganhou;
}