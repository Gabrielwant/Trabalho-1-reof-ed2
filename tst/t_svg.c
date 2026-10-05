
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "svg.h"

int main(void)
{
    Pescaria p = pescaria_cria();
    pescaria_adiciona(p, forma_retangulo(1, 10, 10, 20, 20, "red", "blue"));
    pescaria_adiciona(p, forma_circulo(2, 50, 50, 5, "red", "blue"));
    pescaria_adiciona(p, forma_linha(3, 1, 1, 9, 9, "black"));
    pescaria_adiciona(p, forma_texto(4, 5, 5, "a", "b", 'f', "a<b"));
    nau_define_energia(pescaria_nau(p, 1), 300);
    assert(svg_gera(p, "../tst/bin/t_svg.svg", "<!-- extra -->\n"));

    FILE *f = fopen("../tst/bin/t_svg.svg", "r");
    assert(f);
    char buf[8192];
    size_t n = fread(buf, 1, sizeof buf - 1, f);
    buf[n] = '\0';
    fclose(f);
    assert(strstr(buf, "<svg") && strstr(buf, "</svg>"));
    assert(strstr(buf, "<circle") && strstr(buf, "<rect") && strstr(buf, "<line"));
    assert(strstr(buf, "#800066")); /* contorno da nau com energia >= 250 */
    assert(strstr(buf, "a&lt;b"));  /* texto escapado */
    assert(strstr(buf, "<!-- extra -->"));
    pescaria_destroi(p);
    puts("OK svg");
    return 0;
}
