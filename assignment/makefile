CFLAGS = -g -Wall -Wno-unused-result -O3
LDLIBS = -l hiredis -l readline

all: controlador exemplo1 exemplo2 agente

controlador: controlador.c

exemplo1: campo.h campo.c exemplo1.c

exemplo2: campo.h campo.c exemplo2.c

agente: campo.h campo.c agente.c

clean:
	rm -f controlador exemplo1 exemplo2 agente *.o
