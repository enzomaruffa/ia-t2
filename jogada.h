#ifndef __JOGADA_H_
#define __JOGADA_H_

typedef struct {
    int posicao;
} JogadaFilosofo_t;

// Escreve a jogada do filosofo
void escreve_jogada_filosofo(char lado, JogadaFilosofo_t *jogada, char *vetor, int tamanho_vetor);

typedef struct {
    int pulos;
    int* posicao_pulos;

    // Se 0, não. Se 1, sim e pra nós. Se -1, sim e pra eles.
    char resulta_em_gol;
} JogadaBola_t;

// Escreve a jogada da bola
void escreve_jogada_bola(char lado, JogadaBola_t *jogada, char *vetor, int tamanho_vetor);

// Deve ser typecasted por quem for usar
typedef struct {
    // Se 0, jogada filósofo. Se 1, jogada bola. Se 2, não faz nada.
    char tipo;
    void *jogada;
} RespostaJogada_t;

#endif