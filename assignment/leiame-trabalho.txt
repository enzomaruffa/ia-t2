Conteúdo do pacote:

- campo.c e campo.h: biblioteca de funções para comunicação como o
  controlador do campo via servidor redis

- controlador.c: programa que controla o campo

- exemplo1.c: programa exemplo comentado que usa as funções de
  comunicação, conecta com o controlador, recebe o campo e o movimento
  do adversário e envia para o controlador um novo movimento

- exemplo2.c: programa exemplo que implenta um jogador interativo que
  mostra na saída padrão os dados recebidos do controlador e espera na
  entrada padrão uma nova jogada

- leiame.txt: este texto

- makefile: compila os 3 programas anteriores

- redis.conf: arquivo de configuração para ser usado com o servidor
  redis

Para preparar o seu jogador para se comunicar com o controlador do
campo você deve alterar o seu programa para utilizar as funções que
enviam e recebem dados do controlador. O exemplo1 pode ajudar nesta
alteração.

Para testar o seu jogador você deve iniciar o servidor redis e
executar o controlador.

Para iniciar o servidor redis nos sistemas do DInf execute em um
terminal:

   $ redis-server redis.conf

Para iniciar o controlador execute em outro terminal:

   $ ./controlador e 13 50 0

Este comando inicia o controlador indicando que o jogador que começa é
o 'e', a partida terá 50 movimentos e não há limite de tempo para as
jogadas. Para mais detalhes sobre os parâmetros do controlador
execute:

   $ ./controlador

Para conectar duas instâncias do exemplo2 ao controlador, uma de cada
lado do campo, execute em outros dois terminais os comandos:

   $ ./exemplo2 e

   $ ./exemplo2 d

Você pode testar o seu jogador jogando interativamente contra ele
usando o exemplo2. Basta executar o seu jogador em um terminal e o
exemplo2 em outro.
