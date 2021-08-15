#include <stdlib.h>
#include <stdio.h>
  
#include "minimax.h"

// Ideia no momento: o valor de utilidade do estado é o quão distante a bola pode ficar do nosso gol - o quão distante do gol deles
int calcula_utilidade(EstadoCampo_t *estado) {
    int meu_gol = estado->meu_lado == 'e' ? 0 : estado->tamanho_mapa - 1;
    int gol_oponente = estado->meu_lado == 'd' ? 0 : estado->tamanho_mapa - 1;

    if (estado->mapa[meu_gol] == 'o') {
        return -10000;
    } else if (estado->mapa[gol_oponente] == 'o') {
        return 10000;
    }

    int posicao_bola = -1;
    for (int i = 0; i < estado->tamanho_mapa; i++) {
        if (estado->mapa[i] == 'o') {
            posicao_bola = i;
        }
    }
    
    int tamanho_buffer_jogadas_bola = estado->tamanho_mapa;
    JogadaBola_t **jogadas_bola = malloc(sizeof(JogadaBola_t *) * estado->tamanho_mapa);
    int tamanho_jogadas_bola = 0;

    // Gera jogadas da bola
    if (posicao_bola != -1) {
        // Olha para a direita
        adiciona_jogadas_bola(1, estado, posicao_bola, jogadas_bola, tamanho_buffer_jogadas_bola, &tamanho_jogadas_bola);
        // Olha para a esquerda
        adiciona_jogadas_bola(-1, estado, posicao_bola, jogadas_bola, tamanho_buffer_jogadas_bola, &tamanho_jogadas_bola);
    }

    int maior_distancia_meu_gol = abs(meu_gol - posicao_bola);
    int maior_distancia_gol_oponente = abs(gol_oponente - posicao_bola);

    for (int i = 0; i < tamanho_jogadas_bola; i++) {
        JogadaBola_t *jogada = jogadas_bola[i];

        int posicao_aterrisagem = jogada->posicao_pulos[jogada->pulos - 1];
        // printf("verificando posicao_aterrisagem: %d\n", posicao_aterrisagem);

        int distancia_meu_gol = abs(meu_gol - posicao_aterrisagem);
        int distancia_gol_oponente = abs(gol_oponente - posicao_aterrisagem);

        // printf("    distancia_meu_gol: %d\n", distancia_meu_gol);
        // printf("    distancia_gol_oponente: %d\n", distancia_gol_oponente);

        if (distancia_meu_gol > maior_distancia_meu_gol) {
            maior_distancia_meu_gol = distancia_meu_gol;
        } else if (distancia_gol_oponente > maior_distancia_gol_oponente) {
            maior_distancia_gol_oponente = distancia_gol_oponente;
        }
    }

    // printf("estado->tamanho_mapa: %d\n", estado->tamanho_mapa);

    // Caso vá perder, aumenta em 1000
    if (maior_distancia_gol_oponente == estado->tamanho_mapa - 1) {
        maior_distancia_gol_oponente += 1000;
    }

    // Caso vá ganhar, aumenta em 1000
    if (maior_distancia_meu_gol == estado->tamanho_mapa - 1) {
        maior_distancia_meu_gol += 1000;
    }

    // printf("maior_distancia_gol_oponente: %d\n", maior_distancia_gol_oponente);
    // printf("maior_distancia_meu_gol: %d\n", maior_distancia_meu_gol);

    for (int i = 0; i < tamanho_jogadas_bola; i++) {
        destroi_jogada_bola(jogadas_bola[i]);
    }
    free(jogadas_bola);

    return maior_distancia_meu_gol - (maior_distancia_gol_oponente * 2);
}

// Retorna o valor heurístico desse caminho
int minimax(EstadoCampo_t *estado, int profundidade, int maximizando) {
    int algum_vitorioso = verifica_vitorioso(estado);

    if (profundidade == 0) {
        return calcula_utilidade(estado);
    }

    if (algum_vitorioso) {
        return calcula_utilidade(estado) * profundidade;
    }

    // Pega todas as jogadas que podem ser feitas
    JogadaFilosofo_t **jogadas_filosofo = (JogadaFilosofo_t **) malloc(sizeof(JogadaFilosofo_t *) * estado->tamanho_mapa);
    JogadaBola_t **jogadas_bola = (JogadaBola_t **) malloc(sizeof(JogadaBola_t *) * estado->tamanho_mapa);
    int tamanho_buffer_jogadas_filosofo = estado->tamanho_mapa;
    int tamanho_buffer_jogadas_bola = estado->tamanho_mapa;
    int tamanho_jogadas_filosofo = 0; 
    int tamanho_jogadas_bola = 0;

    // Cria todas as jogadas possíveis
    cria_jogadas_possiveis(estado, jogadas_filosofo, tamanho_buffer_jogadas_filosofo,
    jogadas_bola, tamanho_buffer_jogadas_bola, &tamanho_jogadas_filosofo, &tamanho_jogadas_bola);


    // Cria os estados possíveis
    int tamanho_estados = tamanho_jogadas_bola + tamanho_jogadas_filosofo;
    EstadoCampo_t **estados = malloc(sizeof(EstadoCampo_t *) * tamanho_estados);

    for (int i = 0; i < tamanho_jogadas_bola; i++) {
        EstadoCampo_t *novo_campo = malloc(sizeof(EstadoCampo_t));
        clona_campo(novo_campo, estado);
        aplica_jogada_bola(jogadas_bola[i], novo_campo);
        estados[i] = novo_campo;
    }

    for (int i = tamanho_jogadas_bola; i < tamanho_estados; i++) {
        int indice_filosofo = i - tamanho_jogadas_bola;
        EstadoCampo_t *novo_campo = malloc(sizeof(EstadoCampo_t));
        clona_campo(novo_campo, estado);
        aplica_jogada_filosofo(jogadas_filosofo[indice_filosofo], novo_campo);
        estados[i] = novo_campo;
    }

    int utilidade = 0;

    if (maximizando) {
        utilidade = -100000;

        // Para cada jogada, roda minimax normal passando maximinzando como o oposto
        for (int i = 0; i < tamanho_estados; i++) {
            int nova_utilidade = minimax(estados[i], profundidade - 1, !maximizando);

            if (nova_utilidade > utilidade) {
                utilidade = nova_utilidade;
            } 
        }
    } else {
        utilidade = 100000;

        // Para cada jogada, roda minimax normal passando maximinzando como o oposto
        for (int i = 0; i < tamanho_estados; i++) {
            int nova_utilidade = minimax(estados[i], profundidade - 1, !maximizando);

            if (nova_utilidade < utilidade) {
                utilidade = nova_utilidade;
            } 
        }
    }

    // Libera memória
    for (int i = 0; i < tamanho_jogadas_bola; i++) {
        destroi_jogada_bola(jogadas_bola[i]);
    }
    free(jogadas_bola);


    for (int i = 0; i < tamanho_jogadas_filosofo; i++) {
        destroi_jogada_filosofo(jogadas_filosofo[i]);
    }
    free(jogadas_filosofo);

    for (int i = 0; i < tamanho_estados; i++) {
        destroi_campo(estados[i]);
    }
    free(estados);

    // Retorna o valor heurístico de acordo com o critério
    return utilidade;
}

int minimax_inicial(RespostaJogada_t * jogada, EstadoCampo_t *estado, int profundidade, int maximizando) {
    int algum_vitorioso = verifica_vitorioso(estado);

    if (profundidade == 0 || algum_vitorioso) {
        jogada->tipo = 2;
        jogada->jogada = NULL;
        return 0;
    }

    // Pega todas as jogadas que podem ser feitas
    JogadaFilosofo_t **jogadas_filosofo = (JogadaFilosofo_t **) malloc(sizeof(JogadaFilosofo_t *) * estado->tamanho_mapa);
    JogadaBola_t **jogadas_bola = (JogadaBola_t **) malloc(sizeof(JogadaBola_t *) * estado->tamanho_mapa);
    int tamanho_buffer_jogadas_filosofo = estado->tamanho_mapa;
    int tamanho_buffer_jogadas_bola = estado->tamanho_mapa;
    int tamanho_jogadas_filosofo = 0; 
    int tamanho_jogadas_bola = 0;


    // Cria todas as jogadas possíveis
    cria_jogadas_possiveis(estado, jogadas_filosofo, tamanho_buffer_jogadas_filosofo,
    jogadas_bola, tamanho_buffer_jogadas_bola, &tamanho_jogadas_filosofo, &tamanho_jogadas_bola);

    // Cria os estados possíveis
    int tamanho_estados = tamanho_jogadas_bola + tamanho_jogadas_filosofo;
    EstadoCampo_t **estados = malloc(sizeof(EstadoCampo_t *) * tamanho_estados);

    for (int i = 0; i < tamanho_jogadas_bola; i++) {
        EstadoCampo_t *novo_campo = malloc(sizeof(EstadoCampo_t));
        clona_campo(novo_campo, estado);
        aplica_jogada_bola(jogadas_bola[i], novo_campo);
        estados[i] = novo_campo;
    }

    for (int i = tamanho_jogadas_bola; i < tamanho_estados; i++) {
        int indice_filosofo = i - tamanho_jogadas_bola;
        EstadoCampo_t *novo_campo = malloc(sizeof(EstadoCampo_t));
        clona_campo(novo_campo, estado);
        aplica_jogada_filosofo(jogadas_filosofo[indice_filosofo], novo_campo);
        estados[i] = novo_campo;
    }

    int utilidade = 0;
    int indice_estado = -1;

    if (maximizando) {
        utilidade = -100000;

        // Para cada jogada, roda minimax normal passando maximinzando como o oposto
        for (int i = 0; i < tamanho_estados; i++) {
            int nova_utilidade = minimax(estados[i], profundidade - 1, !maximizando);

            for (int k = 0; k < estados[i]->tamanho_mapa; ++k) {
                printf("%c", estados[i]->mapa[k]);
            }
            printf("    Jogada resultou em %d\n", nova_utilidade);
            if (nova_utilidade >= utilidade) {
                utilidade = nova_utilidade;
                indice_estado = i;
            } 
        }
    } else {
        utilidade = 100000;

        // Para cada jogada, roda minimax normal passando maximinzando como o oposto
        for (int i = 0; i < tamanho_estados; i++) {
            int nova_utilidade = minimax(estados[i], profundidade - 1, !maximizando);

            for (int k = 0; k < estados[i]->tamanho_mapa; ++k) {
                printf("%c", estados[i]->mapa[k]);
            }
            printf("    Jogada resultou em %d\n", nova_utilidade);
            if (nova_utilidade <= utilidade) {
                utilidade = nova_utilidade;
                indice_estado = i;
            } 
        }
    }

    printf("Melhor jogada tem utilidade %d\n", utilidade);

    // A partir do indice do estado, encontra a jogada
    if (indice_estado < tamanho_jogadas_bola) {
        // É uma jogada bola
        JogadaBola_t *resultado = malloc(sizeof(JogadaBola_t));
        clona_jogada_bola(resultado, jogadas_bola[indice_estado]);
        jogada->tipo = 1;
        jogada->jogada = (void *) resultado;
    } else {
        // É uma jogada filósofo
        JogadaFilosofo_t *resultado = malloc(sizeof(JogadaFilosofo_t));
        clona_jogada_filosof(resultado, jogadas_filosofo[indice_estado - tamanho_jogadas_bola]);
        jogada->tipo = 0;
        jogada->jogada = (void *) resultado;
    }

    // Libera memória
    for (int i = 0; i < tamanho_jogadas_bola; i++) {
        destroi_jogada_bola(jogadas_bola[i]);
    }
    free(jogadas_bola);

    for (int i = 0; i < tamanho_jogadas_filosofo; i++) {
        destroi_jogada_filosofo(jogadas_filosofo[i]);
    }
    free(jogadas_filosofo);

    for (int i = 0; i < tamanho_estados; i++) {
        destroi_campo(estados[i]);
    }
    free(estados);

    // Retorna o valor heurístico de acordo com o critério
    return utilidade;
}
