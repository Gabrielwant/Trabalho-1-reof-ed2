
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "qry.h"
#include "geo.h"
#include "util.h"

static char buf[65536];

static void le(const char *caminho)
{
    FILE *f = fopen(caminho, "r");
    assert(f);
    size_t n = fread(buf, 1, sizeof buf - 1, f);
    buf[n] = '\0';
    fclose(f);
}

int main(void)
{
    FILE *f = fopen("../tst/bin/t_qry.geo", "w");
    fputs("r 1 100 100 20 40 blue lightblue\n"
          "r 2 300 100 20 40 red pink\n"
          "c 10 130 110 2 black orange\n"
          "c 11 80 110 2 black orange\n"
          "c 12 500 500 2 black orange\n"
          "l 20 125 120 128 124 brown\n"
          "t 30 126 105 black gold i $\n"
          "t 31 90 130 black green i >-|-<\n"
          "t 32 140 140 black grey m alga\n",
          f);
    fclose(f);
    f = fopen("../tst/bin/t_qry.qry", "w");
    fputs("e 1 2 300\n"
          "lr 1 BB 5 20 30\n"  /* rede em x=125..145,y=100..130: peixe 10, camarao 20, moeda 30 */
          "lr 1 EB 10 20 30\n" /* rede em x=70..90: peixe 11 */
          "mv 1 180 0\n"
          "d 1 BB 15\n"            /* destroi a nau 2 */
          "d 1 BB 999\n"           /* sem energia */
          "mc 10 10 0 0 600 600\n" /* move os peixes restantes */
          "e 5 9 1\n",
          f);
    fclose(f);

    Pescaria p = pescaria_cria();
    assert(geo_carrega("../tst/bin/t_qry.geo", p));
    StrBuf sv = strbuf_cria();
    FILE *txt = fopen("../tst/bin/t_qry.txt", "w");
    assert(qry_executa("../tst/bin/t_qry.qry", p, txt, sv));
    fclose(txt);
    le("../tst/bin/t_qry.txt");

    assert(strstr(buf, "[*] lr 1 BB 5 20 30"));
    assert(strstr(buf, "total desta captura: M$ 6.00"));          /* 5 (peixe) + 1 (camarao) */
    assert(strstr(buf, "energia antes: 300.00, depois: 278.50")); /* -24 +2.5 da moeda */
    assert(strstr(buf, "energia: 230.50 -> 194.50"));             /* mv: 180/5 = 36 */
    assert(strstr(buf, "ponto de impacto: (315.00, 120.00)"));
    assert(strstr(buf, "nau atingida e destruida"));
    assert(strstr(buf, "energia insuficiente"));
    assert(strstr(buf, "peixes transladados: 1")); /* so o peixe 12 sobrou */
    assert(strstr(buf, "nau 2: energia final=300.00"));
    assert(strstr(buf, "[destruida]"));
    assert(strstr(buf, "nau 1:"));

    assert(pescaria_forma(p, 2) == NULL); /* nau destruida saiu da arvore */
    assert(pescaria_forma(p, 10) == NULL && pescaria_forma(p, 11) == NULL);
    assert(util_igual(forma_x(pescaria_forma(p, 12)), 510));
    assert(arvore_valida(pescaria_arvore(p)));
    assert(strstr(strbuf_conteudo(sv), "stroke-dasharray")); /* regioes tracejadas */
    assert(strstr(strbuf_conteudo(sv), "fill:yellow"));      /* quadrado: tiro sem energia */

    strbuf_destroi(sv);
    pescaria_destroi(p);
    puts("OK qry");
    return 0;
}
