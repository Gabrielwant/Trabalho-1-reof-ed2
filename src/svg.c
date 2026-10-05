#include <stdio.h>
#include "svg.h"
#include "arvore.h"
#include "nau.h"

void svg_inicio(FILE *f)
{
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");
}

void svg_fim(FILE *f) { fprintf(f, "</svg>\n"); }

static void escapa(FILE *f, const char *s)
{
    for (; *s; s++)
    {
        switch (*s)
        {
        case '&':
            fputs("&amp;", f);
            break;
        case '<':
            fputs("&lt;", f);
            break;
        case '>':
            fputs("&gt;", f);
            break;
        case '"':
            fputs("&quot;", f);
            break;
        default:
            fputc(*s, f);
        }
    }
}

static void rotulo(FILE *f, double x, double y, int id)
{
    fprintf(f, "  <text x=\"%.2f\" y=\"%.2f\" style=\"font-size:5px;fill:black\">%d</text>\n", x, y, id);
}

void svg_forma(FILE *f, Forma fm, const char *cor_borda, double larg_borda)
{
    const char *borda = cor_borda ? cor_borda : forma_cor_borda(fm);
    double larg = larg_borda > 0 ? larg_borda : 1.0;
    switch (forma_tipo(fm))
    {
    case FORMA_CIRCULO:
        fprintf(f, "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" "
                   "style=\"fill:%s;fill-opacity:0.5;stroke:%s;stroke-width:%g\" />\n",
                forma_x(fm), forma_y(fm), forma_raio(fm),
                forma_cor_preenchimento(fm), borda, larg);
        rotulo(f, forma_x(fm), forma_y(fm), forma_id(fm));
        break;
    case FORMA_RETANGULO:
        fprintf(f, "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" "
                   "style=\"fill:%s;fill-opacity:0.5;stroke:%s;stroke-width:%g\" />\n",
                forma_x(fm), forma_y(fm), forma_largura(fm), forma_altura(fm),
                forma_cor_preenchimento(fm), borda, larg);
        rotulo(f, forma_x(fm) + 2, forma_y(fm) + 7, forma_id(fm));
        break;
    case FORMA_LINHA:
        fprintf(f, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" "
                   "style=\"stroke:%s;stroke-width:%g\" />\n",
                forma_x(fm), forma_y(fm), forma_extremo_x(fm), forma_extremo_y(fm),
                borda, larg);
        rotulo(f, forma_x(fm), forma_y(fm), forma_id(fm));
        break;
    default:
    {
        char a = forma_ancora_texto(fm);
        const char *anc = a == 'm' ? "middle" : (a == 'f' ? "end" : "start");
        fprintf(f, "  <text x=\"%.2f\" y=\"%.2f\" text-anchor=\"%s\" "
                   "style=\"font-size:10px;fill:%s;stroke:%s;stroke-width:0.3\">",
                forma_x(fm), forma_y(fm), anc, forma_cor_preenchimento(fm), borda);
        escapa(f, forma_texto_conteudo(fm));
        fprintf(f, "</text>\n");
    }
    }
}

typedef struct
{
    FILE *f;
    Pescaria p;
} Ctx;

static void desenha(Forma fm, void *c)
{
    Ctx *x = c;
    if (forma_tipo(fm) == FORMA_RETANGULO)
    {
        Nau n = pescaria_nau(x->p, forma_id(fm));
        if (n)
        {
            svg_forma(x->f, fm, nau_cor_contorno(n), nau_largura_contorno(n));
            return;
        }
    }
    svg_forma(x->f, fm, NULL, 0);
}

int svg_gera(Pescaria p, const char *caminho, const char *extras)
{
    FILE *f = fopen(caminho, "w");
    if (!f)
        return 0;
    Ctx c = {f, p};
    svg_inicio(f);
    arvore_percorre(pescaria_arvore(p), desenha, &c);
    if (extras)
        fputs(extras, f);
    svg_fim(f);
    fclose(f);
    return 1;
}