PROJ_NAME=ted
CC=gcc
CFLAGS=-ggdb -O0 -std=c99 -fstack-protector-all -Wall -Wextra -Werror=implicit-function-declaration
LDFLAGS=-O0
LIBS=-lm

MODULOS=util strbuf vetor tabela forma arvore nau pescaria svg geo qry
MODOBJ=$(addsuffix .o,$(MODULOS))
OBJETOS=$(MODOBJ) main.o

.PHONY: all clean saida tstall

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJETOS)
	$(CC) $(LDFLAGS) -o $(PROJ_NAME) $(OBJETOS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# dependencias de cada modulo
util.o: util.h util.c
strbuf.o: strbuf.h strbuf.c
vetor.o: vetor.h vetor.c
tabela.o: tabela.h tabela.c
forma.o: forma.h util.h forma.c
arvore.o: arvore.h forma.h util.h arvore.c
nau.o: nau.h forma.h util.h nau.c
pescaria.o: pescaria.h arvore.h tabela.h nau.h forma.h pescaria.c
svg.o: svg.h pescaria.h arvore.h nau.h forma.h svg.c
geo.o: geo.h pescaria.h forma.h geo.c
qry.o: qry.h pescaria.h arvore.h forma.h nau.h strbuf.h vetor.h util.h qry.c
main.o: pescaria.h geo.h qry.h svg.h arvore.h strbuf.h util.h main.c

# cria o diretorio output no mesmo nivel de src
saida:
	mkdir -p ../output

# testes unitarios: make tst_<modulo> compila e executa tst/t_<modulo>.c
tst_%: $(MODOBJ)
	@mkdir -p ../tst/bin
	$(CC) $(CFLAGS) -I. -o ../tst/bin/t_$* ../tst/t_$*.c $(MODOBJ) $(LIBS)
	../tst/bin/t_$*

tstall: $(addprefix tst_,$(MODULOS))

clean:
	rm -f *.o $(PROJ_NAME)
	rm -rf ../tst/bin
