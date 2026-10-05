#include <stdlib.h>
#include "tabela.h"

#define NBALDES 2039

typedef struct entrada
{
    int id;
    void *v;
    struct entrada *prox;
} Entrada;

struct tabela_s
{
    Entrada *baldes[NBALDES];
    int n;
};

static unsigned hash(int id) { return (unsigned)id % NBALDES; }

Tabela tabela_cria(void) { return calloc(1, sizeof(struct tabela_s)); }

void tabela_destroi(Tabela t, void (*libera)(void *))
{
    if (!t)
        return;
    for (int i = 0; i < NBALDES; i++)
    {
        Entrada *e = t->baldes[i];
        while (e)
        {
            Entrada *p = e->prox;
            if (libera)
                libera(e->v);
            free(e);
            e = p;
        }
    }
    free(t);
}

int tabela_insere(Tabela t, int id, void *v)
{
    if (tabela_busca(t, id))
        return 0;
    Entrada *e = malloc(sizeof *e);
    if (!e)
        return 0;
    e->id = id;
    e->v = v;
    e->prox = t->baldes[hash(id)];
    t->baldes[hash(id)] = e;
    t->n++;
    return 1;
}

void *tabela_busca(Tabela t, int id)
{
    for (Entrada *e = t->baldes[hash(id)]; e; e = e->prox)
        if (e->id == id)
            return e->v;
    return NULL;
}

void *tabela_remove(Tabela t, int id)
{
    Entrada **pp = &t->baldes[hash(id)];
    while (*pp)
    {
        if ((*pp)->id == id)
        {
            Entrada *e = *pp;
            void *v = e->v;
            *pp = e->prox;
            free(e);
            t->n--;
            return v;
        }
        pp = &(*pp)->prox;
    }
    return NULL;
}

int tabela_tamanho(Tabela t) { return t->n; }

void tabela_percorre(Tabela t, void (*f)(int, void *, void *), void *ctx)
{
    for (int i = 0; i < NBALDES; i++)
        for (Entrada *e = t->baldes[i]; e; e = e->prox)
            f(e->id, e->v, ctx);
}