CFLAGS = -g -Wall -Wno-unused-result -O3
LDLIBS = -l hiredis -l readline

all: controlador agente

controlador: controlador.c

exemplo1: campo.h campo.c exemplo1.c

exemplo2: campo.h campo.c exemplo2.c

agente: campo.o estado_campo.o jogada.o minimax.o agente.c

minimax.o: minimax.h minimax.c estado_campo.h
jogada.o: jogada.h jogada.c
estado_campo.o: jogada.h estado_campo.h estado_campo.c
campo.o: campo.h campo.c

clean:
	rm -f controlador exemplo1 exemplo2 agente *.o
