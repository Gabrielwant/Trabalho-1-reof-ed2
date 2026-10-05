#ifndef PESCARIA_H
#define PESCARIA_H
#include "forma.h"
#include "arvore.h"
#include "nau.h"

/*
 * Modulo pescaria: o "banco de dados" da pescaria. Reune
 *   - a arvore Rubro-Negra com todas as formas vivas;
 *   - uma tabela id -> forma (memoria auxiliar);
 *   - uma tabela id -> nau (estado de cada retangulo).
 * A pescaria e dona das formas e dos registros de nau.
 */
typedef struct pescaria_s *Pescaria;

Pescaria pescaria_cria(void);
/* Libera tudo (formas, naus, arvore). */
void pescaria_destroi(Pescaria p);

/* Adiciona a forma (passa a ser de propriedade da pescaria). Se for
   retangulo, cria a nau correspondente. Retorna 0 se o id ja existe
   (a forma nao e adotada nesse caso). */
int pescaria_adiciona(Pescaria p, Forma f);

/* Forma viva de identificador id, ou NULL. */
Forma pescaria_forma(Pescaria p, int id);
/* Registro da nau id (inclusive destruida), ou NULL. */
Nau pescaria_nau(Pescaria p, int id);

/* Remove a forma da arvore e da tabela e a libera. */
void pescaria_remove(Pescaria p, Forma f);

/* Desloca a forma de (dx,dy), mantendo a arvore consistente. */
void pescaria_move(Pescaria p, Forma f, double dx, double dy);

/* Arvore de formas (somente leitura/busca). */
Arvore pescaria_arvore(Pescaria p);

/* Chama cb(nau, ctx) para todas as naus (inclusive destruidas), sem ordem. */
void pescaria_percorre_naus(Pescaria p, void (*cb)(Nau, void *), void *ctx);

#endif