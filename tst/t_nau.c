
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "nau.h"
#include "util.h"

int main(void)
{
    Forma r = forma_retangulo(7, 0, 0, 10, 10, "a", "b");
    Nau n = nau_cria(r);
    assert(nau_id(n) == 7 && nau_forma(n) == r);
    assert(strcmp(nau_cor_contorno(n), "#484537") == 0);
    nau_define_energia(n, 50);
    assert(strcmp(nau_cor_contorno(n), "#FFCC00") == 0);
    nau_define_energia(n, 100);
    assert(strcmp(nau_cor_contorno(n), "#217821") == 0 && util_igual(nau_largura_contorno(n), 2));
    nau_define_energia(n, 250);
    assert(strcmp(nau_cor_contorno(n), "#800066") == 0 && util_igual(nau_largura_contorno(n), 3));
    nau_gasta(n, 1000);
    assert(util_igual(nau_energia(n), 0));
    nau_ganha(n, 2.5);
    assert(util_igual(nau_energia(n), 2.5));
    nau_soma_riqueza(n, 30);
    nau_conta(n, 1, 2, 3, 4);
    assert(util_igual(nau_riqueza(n), 30) && nau_lagostas(n) == 1 && nau_camaroes(n) == 2 &&
           nau_peixes(n) == 3 && nau_moedas(n) == 4);
    assert(!nau_destruida(n));
    nau_marca_destruida(n);
    assert(nau_destruida(n) && nau_forma(n) == NULL && nau_id(n) == 7);
    nau_destroi(n);
    forma_destroi(r);
    puts("OK nau");
    return 0;
}
