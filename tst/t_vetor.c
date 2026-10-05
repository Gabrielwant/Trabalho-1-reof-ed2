
#include <assert.h>
#include <stdio.h>
#include "vetor.h"

static int cmp(const void *a, const void *b)
{
    int x = *(int *)(*(void *const *)a), y = *(int *)(*(void *const *)b);
    return (x > y) - (x < y);
}

int main(void)
{
    int val[100];
    Vetor v = vetor_cria();
    for (int i = 0; i < 100; i++)
    {
        val[i] = 100 - i;
        vetor_adiciona(v, &val[i]);
    }
    assert(vetor_tamanho(v) == 100);
    assert(*(int *)vetor_obtem(v, 0) == 100);
    vetor_ordena(v, cmp);
    assert(*(int *)vetor_obtem(v, 0) == 1 && *(int *)vetor_obtem(v, 99) == 100);
    vetor_destroi(v);
    puts("OK vetor");
    return 0;
}
