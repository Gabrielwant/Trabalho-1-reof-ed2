
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "util.h"

#define N 3000

static void conta(Forma f, void *ctx)
{
    (void)f;
    (*(int *)ctx)++;
}

static int cmp_id(const void *a, const void *b)
{
    return forma_id(*(Forma const *)a) - forma_id(*(Forma const *)b);
}

int main(void)
{
    srand(42);
    Arvore a = arvore_cria();
    Forma f[N];
    for (int i = 0; i < N; i++)
    {
        /* varias formas com X repetido para exercitar os criterios de desempate */
        double x = (rand() % 200) * 5.0, y = rand() % 1000, r = 1 + rand() % 4;
        f[i] = forma_circulo(i + 1, x, y, r, "black", "red");
        assert(arvore_insere(a, f[i]));
        if (i % 500 == 0)
            assert(arvore_valida(a));
    }
    assert(arvore_tamanho(a) == N && arvore_valida(a));
    assert(!arvore_insere(a, f[0])); /* duplicata */

    /* busca por regiao == forca bruta, e com poda */
    double rx = 100, ry = 100, rw = 80, rh = 80;
    int esperado = 0, obtido = 0;
    for (int i = 0; i < N; i++)
    {
        double x1, y1, x2, y2;
        forma_bbox(f[i], &x1, &y1, &x2, &y2);
        if (x1 >= rx && y1 >= ry && x2 <= rx + rw && y2 <= ry + rh)
            esperado++;
    }
    arvore_busca_contidas(a, rx, ry, rw, rh, conta, &obtido);
    assert(obtido == esperado);
    assert(arvore_nos_visitados(a) < N / 2); /* a poda funcionou */
    printf("regiao: %d achados, %d de %d nos visitados\n", obtido, arvore_nos_visitados(a), N);

    int pt = 0, pe = 0;
    for (int i = 0; i < N; i++)
    {
        double x1, y1, x2, y2;
        forma_bbox(f[i], &x1, &y1, &x2, &y2);
        if (500 >= x1 && 500 <= x2 && 500 >= y1 && 500 <= y2)
            pe++;
    }
    arvore_busca_ponto(a, 500, 500, conta, &pt);
    assert(pt == pe);

    /* remove metade (ordem embaralhada), valida a cada passo */
    for (int i = N - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Forma t = f[i];
        f[i] = f[j];
        f[j] = t;
    }
    for (int i = 0; i < N / 2; i++)
    {
        assert(arvore_remove(a, f[i]));
        assert(!arvore_remove(a, f[i]));
        if (i % 100 == 0)
            assert(arvore_valida(a));
    }
    assert(arvore_tamanho(a) == N - N / 2 && arvore_valida(a));
    /* remove o resto */
    for (int i = N / 2; i < N; i++)
        assert(arvore_remove(a, f[i]));
    assert(arvore_tamanho(a) == 0 && arvore_valida(a));

    /* reinsere ordenado por id (pior caso para BST simples) */
    qsort(f, N, sizeof f[0], cmp_id);
    for (int i = 0; i < N; i++)
        arvore_insere(a, f[i]);
    assert(arvore_valida(a));

    FILE *dot = tmpfile();
    assert(arvore_escreve_dot(a, dot));
    fclose(dot);

    arvore_destroi(a, forma_destroi);
    puts("OK arvore");
    return 0;
}
