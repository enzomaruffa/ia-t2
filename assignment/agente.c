#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "campo.h"
#include "estado_campo.h"
#include "minimax.h"

#define MAXSTR 512

int main(int argc, char **argv) {
  char buf[MAXSTR];
  char lado_campo;
  int tamanho_campo;
  char *linha;

  // campo_conecta(argc, argv);

  while(1) {
    // campo_recebe(buf);

    linha = readline(NULL);
    if(linha[0] == '0')
      exit(0);
    sprintf(buf, "%s\n", linha);

    // Define as informações da linha
    lado_campo = linha[0];
    tamanho_campo = linha[2] - '0';
    char *mapa = (char *) malloc(sizeof(char) * tamanho_campo);
    for (int i = 0; i < tamanho_campo; ++i) {
      mapa[i] = linha[i+4];
      printf("%c", mapa[i]);
    }
    printf("\n");


    EstadoCampo_t *campo = (EstadoCampo_t *) malloc(sizeof(EstadoCampo_t));
    cria_campo(campo, lado_campo, 1, mapa, tamanho_campo);
    
    printf("campo criado: \n");
    for (int k = 0; k < campo->tamanho_mapa; ++k) {
      printf("%c", campo->mapa[k]);
    }
    printf("\n");
    
    JogadaFilosofo_t **jogadas_filosofo = (JogadaFilosofo_t **) malloc(sizeof(JogadaFilosofo_t *) * campo->tamanho_mapa);
    JogadaBola_t **jogadas_bola = (JogadaBola_t **) malloc(sizeof(JogadaBola_t *) * campo->tamanho_mapa);
    int tamanho_buffer_jogadas_filosofo = campo->tamanho_mapa;
    int tamanho_buffer_jogadas_bola = campo->tamanho_mapa;
    int tamanho_jogadas_filosofo = 0; 
    int tamanho_jogadas_bola = 0;

    // cria nossas jogadas possíveis
    int jogo_acabou = cria_jogadas_possiveis(campo, jogadas_filosofo, tamanho_buffer_jogadas_filosofo,
      jogadas_bola, tamanho_buffer_jogadas_bola, &tamanho_jogadas_filosofo, &tamanho_jogadas_bola);
    
    if (jogo_acabou) {
      return 1;
    }

    printf("tamanho_jogadas_filosofo: %d\n", tamanho_jogadas_filosofo);
    int menor_utilidade = 10000;
    RespostaJogada_t *menor_utilidade_jogada = malloc(sizeof(RespostaJogada_t));

    printf("tamanho_jogadas_bola: %d\n", tamanho_jogadas_bola);
    for(int i = 0; i < tamanho_jogadas_bola; ++i) {
      // copia campo
      EstadoCampo_t *campo_aux_nosso = (EstadoCampo_t *) malloc(sizeof(EstadoCampo_t));
      clona_campo(campo_aux_nosso, campo);

      // realiza jogada 
      aplica_jogada_bola(jogadas_bola[i], campo_aux_nosso);

      printf("campo_aux_nosso: ");
      for (int k = 0; k < campo_aux_nosso->tamanho_mapa; ++k) {
        printf("%c", campo_aux_nosso->mapa[k]);
      }
      printf("\n");
      int utilidade_campo = calcula_utilidade(campo_aux_nosso);
      printf("utilidade_campo: %d\n", utilidade_campo);

      // crias as jogadas possíveis do oponente
      JogadaFilosofo_t **jogadas_oponente_filosofo = (JogadaFilosofo_t **) malloc(sizeof(JogadaFilosofo_t));
      JogadaBola_t **jogadas_oponente_bola = (JogadaBola_t **) malloc(sizeof(JogadaBola_t));
      int tamanho_buffer_jogadas_oponente_filosofo = campo->tamanho_mapa;
      int tamanho_buffer_jogadas_oponente_bola = campo->tamanho_mapa;
      int tamanho_jogadas_oponente_filosofo = 0; 
      int tamanho_jogadas_oponente_bola = 0;

      jogo_acabou = cria_jogadas_possiveis(campo_aux_nosso, jogadas_oponente_filosofo, tamanho_buffer_jogadas_oponente_filosofo,
        jogadas_oponente_bola, tamanho_buffer_jogadas_oponente_bola, &tamanho_jogadas_oponente_filosofo, &tamanho_jogadas_oponente_bola);
      // itera sobre as jogadas possíveis do oponente
      if (jogo_acabou) {
        return 1;
      }
      int menor_utilidade_atual = 1110;
      int utilidade_bola = calcula_utilidade_oponente_bola(campo_aux_nosso, jogadas_oponente_bola, tamanho_buffer_jogadas_oponente_bola, &tamanho_jogadas_oponente_bola);
      int utilidade_filoso = calcula_utilidade_oponente_filosofo(campo_aux_nosso, jogadas_oponente_filosofo, tamanho_buffer_jogadas_filosofo, &tamanho_jogadas_oponente_filosofo);
      
      if(utilidade_bola > utilidade_filoso) {
        menor_utilidade_atual = utilidade_filoso;
      } else {
        menor_utilidade_atual = utilidade_bola;
      }

      if(menor_utilidade > menor_utilidade_atual) {
        printf("Mudando menor_utilidade: %d\n", menor_utilidade_atual);    
        menor_utilidade = menor_utilidade_atual;
        menor_utilidade_jogada->jogada = jogadas_bola[i];
        menor_utilidade_jogada->tipo = 1;
      }
     
    }

    for(int i = 0; i < tamanho_jogadas_filosofo; ++i) {
      // copia campo
      EstadoCampo_t *campo_aux_nosso = (EstadoCampo_t *) malloc(sizeof(EstadoCampo_t));
      clona_campo(campo_aux_nosso, campo);

      // realiza jogada 
      aplica_jogada_filosofo(jogadas_filosofo[i], campo_aux_nosso);
      printf("campo_aux_nosso: ");
      for (int k = 0; k < campo_aux_nosso->tamanho_mapa; ++k) {
        printf("%c", campo_aux_nosso->mapa[k]);
      }
      printf("\n");

      int utilidade_campo = calcula_utilidade(campo_aux_nosso);
      printf("utilidade_campo: %d\n", utilidade_campo);

      // crias as jogadas possíveis do oponente
      JogadaFilosofo_t **jogadas_oponente_filosofo = (JogadaFilosofo_t **) malloc(sizeof(JogadaFilosofo_t *) * campo->tamanho_mapa);
      JogadaBola_t **jogadas_oponente_bola = (JogadaBola_t **) malloc(sizeof(JogadaBola_t *) * campo->tamanho_mapa);
      int tamanho_buffer_jogadas_oponente_filosofo = 0;
      int tamanho_buffer_jogadas_oponente_bola = 0;
      int tamanho_jogadas_oponente_filosofo = 0; 
      int tamanho_jogadas_oponente_bola = 0;


      int jogo_acabou = cria_jogadas_possiveis(campo_aux_nosso, jogadas_oponente_filosofo, tamanho_buffer_jogadas_oponente_filosofo,
        jogadas_oponente_bola, tamanho_buffer_jogadas_oponente_bola, &tamanho_jogadas_oponente_filosofo, &tamanho_jogadas_oponente_bola);
      
      if (jogo_acabou) {
        return 1;
      }

      int menor_utilidade_atual = 0;
      int utilidade_bola = calcula_utilidade_oponente_bola(campo_aux_nosso, jogadas_oponente_bola, tamanho_buffer_jogadas_oponente_bola, &tamanho_jogadas_oponente_bola);
      int utilidade_filoso = calcula_utilidade_oponente_filosofo(campo_aux_nosso, jogadas_oponente_filosofo, tamanho_buffer_jogadas_filosofo, &tamanho_jogadas_oponente_filosofo);
      
      if(utilidade_bola > utilidade_filoso) {
        menor_utilidade_atual = utilidade_filoso;
      } else {
        menor_utilidade_atual = utilidade_bola;
      }

      if(menor_utilidade > menor_utilidade_atual) {
        printf("Mudando menor_utilidade filosofo: %d\n", menor_utilidade_atual);
        menor_utilidade = menor_utilidade_atual;
        menor_utilidade_jogada->jogada = jogadas_filosofo[i];
        menor_utilidade_jogada->tipo = 0;
      }
    }
    
    printf("menor_utilidade: %d\n", menor_utilidade);
    if (menor_utilidade_jogada->tipo == 1) {
      aplica_jogada_bola(menor_utilidade_jogada->jogada, campo);
      // escreve_jogada_bola();
    } else if(menor_utilidade_jogada->tipo == 0) {
      aplica_jogada_filosofo(menor_utilidade_jogada->jogada, campo);
      // escreve_jogada_filosofo();
    }
    printf("campo_final: ");
    for (int k = 0; k < campo->tamanho_mapa; ++k) {
      printf("%c", campo->mapa[k]);
    }
    printf("\n");
    free(linha);
    free(mapa);
    // destroi_campo(campo);

    // campo_envia(buf);
  }
}
