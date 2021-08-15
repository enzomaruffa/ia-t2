#ifndef __JOGADA_H_
#define __JOGADA_H_

typedef struct {
    int posicao;
} JogadaFilosofo_t;

// Escreve a jogada do filosofo
int escreve_jogada_filosofo(char lado, JogadaFilosofo_t *jogada, char *vetor, int tamanho_vetor);

// Clona uma jogada filósofo
int clona_jogada_filosof(JogadaFilosofo_t *dest, JogadaFilosofo_t *jogada);

// Destrói uma jogada filosofo
void destroi_jogada_filosofo(JogadaFilosofo_t *jogada);

typedef struct {
    int pulos;
    int* posicao_pulos;

    // Se 0, não. Se 1, sim e pra nós. Se -1, sim e pra eles.
    char resulta_em_gol;
} JogadaBola_t;

// Escreve a jogada da bola
int escreve_jogada_bola(char lado, JogadaBola_t *jogada, char *vetor, int tamanho_vetor);

// Clona uma jogada bola
int clona_jogada_bola(JogadaBola_t *dest, JogadaBola_t *jogada);

// Destrói uma jogada bola
void destroi_jogada_bola(JogadaBola_t *jogada);

// Deve ser typecasted por quem for usar
typedef struct {
    // Se 0, jogada filósofo. Se 1, jogada bola. Se 2, não faz nada.
    char tipo;
    void *jogada;
} RespostaJogada_t;

#endif