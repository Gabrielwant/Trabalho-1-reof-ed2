
#include <assert.h>
#include <stdio.h>
#include "pescaria.h"
#include "util.h"

int main(void)
{
    Pescaria p = pescaria_cria();
    Forma r = forma_retangulo(1, 10, 10, 20, 20, "a", "b");
    Forma c = forma_circulo(2, 50, 50, 5, "a", "b");
    assert(pescaria_adiciona(p, r) && pescaria_adiciona(p, c));
    Forma dup = forma_circulo(2, 0, 0, 1, "a", "b");
    assert(!pescaria_adiciona(p, dup));
    forma_destroi(dup);
    assert(pescaria_forma(p, 2) == c && pescaria_forma(p, 3) == NULL);
    assert(pescaria_nau(p, 1) != NULL && pescaria_nau(p, 2) == NULL);
    assert(arvore_tamanho(pescaria_arvore(p)) == 2);

    pescaria_move(p, c, 100, 0);
    assert(util_igual(forma_x(c), 150) && arvore_tamanho(pescaria_arvore(p)) == 2);
    assert(arvore_valida(pescaria_arvore(p)));

    pescaria_remove(p, c);
    assert(pescaria_forma(p, 2) == NULL && arvore_tamanho(pescaria_arvore(p)) == 1);
    pescaria_destroi(p);
    puts("OK pescaria");
    return 0;
}
