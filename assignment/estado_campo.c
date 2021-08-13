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

int verifica_gol(char meu_lado, int posicao_aterrisagem, int tamanho_mapa) {
    // Se a posicao for o gol, faz uma jogada de gol
    if (meu_lado == 'd' && posicao_aterrisagem == tamanho_mapa - 1) {
        return -1; // Gol contra
    } else if (meu_lado == 'd' && posicao_aterrisagem == 0) {
        return 1; // Gol a favor
    } else if (meu_lado == 'e' && posicao_aterrisagem == tamanho_mapa - 1) {
        return 1; // Gol a favor
    } 
    return -1; // Gol contra
}

void adiciona_jogadas_bola(int lado_diff, EstadoCampo_t *campo, int posicao_bola, 
                        JogadaBola_t **jogadas_bola, int tamanho_buffer_jogadas_bola,
                        int *tamanho_jogadas_bola) {
    // Guarda as posições que a bola está aterrisando
    int posicao_atual_aterrisagem = 0;
    int *posicoes_aterrisagem = malloc(sizeof(campo->tamanho_mapa));

    // Guarda as posições que devemos checar
    int posicao_atual_checagem = 0;
    int iterador_checagem = 0;
    int *posicoes_checagem = malloc(sizeof(campo->tamanho_mapa));

    posicoes_checagem[0] = posicao_bola + lado_diff;
    posicao_atual_checagem += 1;

    while (iterador_checagem < posicao_atual_checagem) {
        int posicao_checagem = posicoes_checagem[posicao_atual_checagem];

        if (campo->mapa[posicao_checagem] == 'f') {
            // Encontra a posicao de soltar a bola
            int posicao_aterrisagem = posicao_checagem + lado_diff;
            while (campo->mapa[posicao_aterrisagem] == 'f') {
                posicao_aterrisagem += lado_diff;
            }

            // Adiciona na lista de posições de aterrisagem
            posicoes_aterrisagem[posicao_atual_aterrisagem] = posicao_aterrisagem;
            posicao_atual_aterrisagem += 1;

            JogadaBola_t *jogada = malloc(sizeof(JogadaBola_t));
            jogada->pulos = posicao_atual_aterrisagem;
            jogada->posicao_pulos = malloc(sizeof(int) * posicao_atual_aterrisagem);
            jogada->resulta_em_gol = 0;

            // Copia as posições pulos envolvidas
            memcpy(jogada->posicao_pulos, posicoes_aterrisagem, sizeof(int) * posicao_atual_aterrisagem);

            // Adiciona nas jogadas bola
            jogadas_bola[*tamanho_jogadas_bola] = jogada;
            *tamanho_jogadas_bola += 1;

            if (campo->mapa[posicao_aterrisagem] == 'g') {
                jogada->resulta_em_gol = verifica_gol(campo->meu_lado, posicao_aterrisagem, campo->tamanho_mapa);
            } else if (campo->mapa[posicao_aterrisagem] == '.') {
                // Enfileira a jogada para ser verificada
                posicoes_checagem[posicao_atual_checagem] = posicao_aterrisagem + 1;
                posicao_atual_checagem += 1;
            }
        }
    }

    free(posicoes_aterrisagem);
    free(posicoes_checagem);
}

// 1 se nenhuma jogada é possível porque algum jogador ganhou
int cria_jogadas_possiveis(EstadoCampo_t *campo,
    JogadaFilosofo_t **jogadas_filosofo, int tamanho_buffer_jogadas_filosofo,
    JogadaBola_t **jogadas_bola, int tamanho_buffer_jogadas_bola,
    int *tamanho_jogadas_filosofo, int *tamanho_jogadas_bola) {

    if (campo->mapa[0] == 'o' || campo->mapa[campo->tamanho_mapa - 1] == 'o') { 
        return 1;
    }

    *tamanho_jogadas_filosofo = 0;
    *tamanho_jogadas_bola = 0;

    // Gera jogadas de filosofos
    for (int i = 0; i < campo->tamanho_mapa; i++) {
        if (campo->mapa[i] == '.') {
            JogadaFilosofo_t *jogada = malloc(sizeof(JogadaFilosofo_t));
            jogada->posicao = i;

            jogadas_filosofo[*tamanho_jogadas_filosofo] = jogada;
            *tamanho_jogadas_filosofo += 1;
        }
    }

    // Acha a bola
    int posicao_bola = -1;

    for (int i = 0; i < campo->tamanho_mapa; i++) {
        if (campo->mapa[i] == 'o') {
            posicao_bola = i;
        }
    }

    // Gera jogadas da bola
    if (posicao_bola != -1) {
        // Olha para a direita
        adiciona_jogadas_bola(1, campo, posicao_bola, jogadas_bola, tamanho_buffer_jogadas_bola, tamanho_jogadas_bola);
        // Olha para a esquerda
        adiciona_jogadas_bola(-1, campo, posicao_bola, jogadas_bola, tamanho_buffer_jogadas_bola, tamanho_jogadas_bola);
    }

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
 * Não valida se a jogada é válida!
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

    return 0;
}