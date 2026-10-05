#include <stdlib.h>
#include "vetor.h"

struct vetor_s
{
    void **dados;
    int n, cap;
};

Vetor vetor_cria(void)
{
    Vetor v = malloc(sizeof *v);
    if (!v)
        return NULL;
    v->n = 0;
    v->cap = 8;
    v->dados = malloc((size_t)v->cap * sizeof(void *));
    if (!v->dados)
    {
        free(v);
        return NULL;
    }
    return v;
}

void vetor_destroi(Vetor v)
{
    if (!v)
        return;
    free(v->dados);
    free(v);
}

void vetor_adiciona(Vetor v, void *p)
{
    if (v->n == v->cap)
    {
        int nc = v->cap * 2;
        void **d = realloc(v->dados, (size_t)nc * sizeof(void *));
        if (!d)
            return;
        v->dados = d;
        v->cap = nc;
    }
    v->dados[v->n++] = p;
}

int vetor_tamanho(Vetor v) { return v->n; }

void *vetor_obtem(Vetor v, int i) { return v->dados[i]; }

void vetor_ordena(Vetor v, int (*cmp)(const void *, const void *))
{
    if (v->n > 1)
        qsort(v->dados, (size_t)v->n, sizeof(void *), cmp);
}