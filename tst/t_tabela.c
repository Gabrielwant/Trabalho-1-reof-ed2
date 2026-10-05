
#include <assert.h>
#include <stdio.h>
#include "tabela.h"

static void conta(int id, void *v, void *ctx)
{
    (void)id;
    (void)v;
    (*(int *)ctx)++;
}

int main(void)
{
    int val[5000];
    Tabela t = tabela_cria();
    for (int i = 0; i < 5000; i++)
        assert(tabela_insere(t, i + 1, &val[i]));
    assert(!tabela_insere(t, 7, &val[0]));
    assert(tabela_tamanho(t) == 5000);
    assert(tabela_busca(t, 123) == &val[122]);
    assert(tabela_busca(t, 99999) == NULL);
    assert(tabela_remove(t, 123) == &val[122]);
    assert(tabela_busca(t, 123) == NULL && tabela_remove(t, 123) == NULL);
    int n = 0;
    tabela_percorre(t, conta, &n);
    assert(n == 4999);
    tabela_destroi(t, NULL);
    puts("OK tabela");
    return 0;
}
