
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "forma.h"
#include "util.h"

int main(void)
{
    Forma c = forma_circulo(1, 50, 50, 10, "red", "blue");
    Forma r = forma_retangulo(2, 10, 20, 30, 40, "red", "blue");
    Forma l = forma_linha(3, 100, 80, 90, 60, "black");
    Forma t = forma_texto(4, 100, 100, "a", "b", 'm', "$");
    Forma g = forma_texto(5, 0, 0, "a", "b", 'i', ">-|-<");
    double a, b, cc, d;

    assert(util_igual(forma_area(c), 3.14159265358979323846 * 100));
    assert(util_igual(forma_area(r), 1200));
    forma_bbox(c, &a, &b, &cc, &d);
    assert(util_igual(a, 40) && util_igual(b, 40) && util_igual(cc, 60) && util_igual(d, 60));
    /* linha: ancora na extremidade de menor X */
    assert(util_igual(forma_x(l), 90) && util_igual(forma_y(l), 60));
    forma_bbox(l, &a, &b, &cc, &d);
    assert(util_igual(a, 90) && util_igual(b, 60) && util_igual(cc, 100) && util_igual(d, 80));
    /* texto com ancora no meio: 1 caractere => largura 10 */
    forma_bbox(t, &a, &b, &cc, &d);
    assert(util_igual(a, 95) && util_igual(cc, 105) && util_igual(b, 100) && util_igual(d, 100));
    assert(forma_eh_moeda(t) && !forma_eh_lagosta(t));
    assert(forma_eh_lagosta(g) && !forma_eh_moeda(g));
    assert(!forma_eh_moeda(c));

    forma_move(l, 5, -10);
    assert(util_igual(forma_x(l), 95) && util_igual(forma_extremo_x(l), 105) && util_igual(forma_extremo_y(l), 70));

    /* ordem: X, depois area, depois Y, depois id */
    Forma p = forma_circulo(10, 5, 5, 1, "a", "b");
    Forma q = forma_circulo(11, 5, 5, 2, "a", "b");
    Forma s = forma_circulo(12, 5, 5, 2, "a", "b");
    Forma u = forma_circulo(13, 4, 9, 9, "a", "b");
    assert(forma_compara(u, p) < 0); /* X menor */
    assert(forma_compara(p, q) < 0); /* mesma X, area menor */
    assert(forma_compara(q, s) < 0); /* empate total: desempata pelo id */
    assert(forma_compara(s, s) == 0);

    FILE *f = tmpfile();
    forma_descreve(r, f);
    fclose(f);
    Forma todas[] = {c, r, l, t, g, p, q, s, u};
    for (int i = 0; i < 9; i++)
        forma_destroi(todas[i]);
    puts("OK forma");
    return 0;
}
