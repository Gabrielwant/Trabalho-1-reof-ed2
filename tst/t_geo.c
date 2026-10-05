
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "geo.h"
#include "util.h"

int main(void)
{
    const char *arq = "../tst/bin/t_geo.geo";
    FILE *f = fopen(arq, "w");
    assert(f);
    fputs("c 1 50.00 50.0 30.00 grey magenta\n"
          "r 6 121.0 46.0 100.0 30.0 cyan yellow\n"
          "l 3 10 10 5 5 black\n"
          "t 4 20 20 red blue m texto com espacos  \n"
          "t 5 30 30 red blue i $\n"
          "\n"
          "x comando desconhecido\n"
          "c 1 0 0 1 a b\n",
          f); /* id repetido: ignorado */
    fclose(f);

    Pescaria p = pescaria_cria();
    assert(!geo_carrega("../tst/bin/nao_existe.geo", p));
    assert(geo_carrega(arq, p));
    assert(arvore_tamanho(pescaria_arvore(p)) == 5);
    assert(util_igual(forma_raio(pescaria_forma(p, 1)), 30.0));
    assert(pescaria_nau(p, 6) != NULL);
    Forma l = pescaria_forma(p, 3);
    assert(util_igual(forma_x(l), 5) && util_igual(forma_extremo_x(l), 10));
    assert(strcmp(forma_texto_conteudo(pescaria_forma(p, 4)), "texto com espacos") == 0);
    assert(forma_ancora_texto(pescaria_forma(p, 4)) == 'm');
    assert(forma_eh_moeda(pescaria_forma(p, 5)));
    pescaria_destroi(p);
    puts("OK geo");
    return 0;
}
