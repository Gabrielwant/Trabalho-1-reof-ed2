
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "forma.h"
#include "util.h"

#define PI 3.14159265358979323846
#define LARGURA_CHAR 10.0
#define AREA_CHAR 20.0

struct forma_s
{
    int id;
    TipoForma tipo;
    double x, y; /* ancora */
    double a, b; /* circulo: a=r | retangulo: a=w,b=h | linha: (a,b)=outra ponta */
    char ancora; /* texto */
    char *corb, *corp, *txt;
};

static Forma aloca(int id, TipoForma t)
{
    Forma f = calloc(1, sizeof *f);
    if (!f)
    {
        fprintf(stderr, "erro: sem memoria\n");
        exit(1);
    }
    f->id = id;
    f->tipo = t;
    return f;
}

Forma forma_circulo(int id, double x, double y, double r,
                    const char *corb, const char *corp)
{
    Forma f = aloca(id, FORMA_CIRCULO);
    f->x = x;
    f->y = y;
    f->a = r;
    f->corb = util_dup(corb);
    f->corp = util_dup(corp);
    return f;
}

Forma forma_retangulo(int id, double x, double y, double w, double h,
                      const char *corb, const char *corp)
{
    Forma f = aloca(id, FORMA_RETANGULO);
    f->x = x;
    f->y = y;
    f->a = w;
    f->b = h;
    f->corb = util_dup(corb);
    f->corp = util_dup(corp);
    return f;
}

Forma forma_linha(int id, double x1, double y1, double x2, double y2,
                  const char *cor)
{
    Forma f = aloca(id, FORMA_LINHA);
    if (x2 < x1 || (util_igual(x1, x2) && y2 < y1))
    {
        double tx = x1, ty = y1;
        x1 = x2;
        y1 = y2;
        x2 = tx;
        y2 = ty;
    }
    f->x = x1;
    f->y = y1;
    f->a = x2;
    f->b = y2;
    f->corb = util_dup(cor);
    f->corp = util_dup(cor);
    return f;
}

Forma forma_texto(int id, double x, double y, const char *corb,
                  const char *corp, char ancora, const char *txt)
{
    Forma f = aloca(id, FORMA_TEXTO);
    f->x = x;
    f->y = y;
    f->ancora = ancora;
    f->corb = util_dup(corb);
    f->corp = util_dup(corp);
    f->txt = util_dup(txt);
    return f;
}

void forma_destroi(Forma f)
{
    if (!f)
        return;
    free(f->corb);
    free(f->corp);
    free(f->txt);
    free(f);
}

int forma_id(Forma f) { return f->id; }
TipoForma forma_tipo(Forma f) { return f->tipo; }
double forma_x(Forma f) { return f->x; }
double forma_y(Forma f) { return f->y; }

static double larg_texto(Forma f) { return LARGURA_CHAR * (double)strlen(f->txt); }

double forma_area(Forma f)
{
    switch (f->tipo)
    {
    case FORMA_CIRCULO:
        return PI * f->a * f->a;
    case FORMA_RETANGULO:
        return f->a * f->b;
    case FORMA_LINHA:
        return 2.0 * sqrt((f->a - f->x) * (f->a - f->x) +
                          (f->b - f->y) * (f->b - f->y));
    default:
        return AREA_CHAR * (double)strlen(f->txt);
    }
}

void forma_bbox(Forma f, double *x1, double *y1, double *x2, double *y2)
{
    switch (f->tipo)
    {
    case FORMA_CIRCULO:
        *x1 = f->x - f->a;
        *y1 = f->y - f->a;
        *x2 = f->x + f->a;
        *y2 = f->y + f->a;
        break;
    case FORMA_RETANGULO:
        *x1 = f->x;
        *y1 = f->y;
        *x2 = f->x + f->a;
        *y2 = f->y + f->b;
        break;
    case FORMA_LINHA:
        *x1 = f->x;
        *x2 = f->a;
        *y1 = f->y < f->b ? f->y : f->b;
        *y2 = f->y < f->b ? f->b : f->y;
        break;
    default:
    {
        double w = larg_texto(f);
        if (f->ancora == 'm')
        {
            *x1 = f->x - w / 2;
            *x2 = f->x + w / 2;
        }
        else if (f->ancora == 'f')
        {
            *x1 = f->x - w;
            *x2 = f->x;
        }
        else
        {
            *x1 = f->x;
            *x2 = f->x + w;
        }
        *y1 = *y2 = f->y;
    }
    }
}

void forma_move(Forma f, double dx, double dy)
{
    f->x += dx;
    f->y += dy;
    if (f->tipo == FORMA_LINHA)
    {
        f->a += dx;
        f->b += dy;
    }
}

int forma_compara(Forma a, Forma b)
{
    int c = util_compara(a->x, b->x);
    if (c)
        return c;
    c = util_compara(forma_area(a), forma_area(b));
    if (c)
        return c;
    c = util_compara(a->y, b->y);
    if (c)
        return c;
    return (a->id > b->id) - (a->id < b->id);
}

double forma_raio(Forma f) { return f->tipo == FORMA_CIRCULO ? f->a : 0.0; }
double forma_largura(Forma f)
{
    if (f->tipo == FORMA_RETANGULO)
        return f->a;
    if (f->tipo == FORMA_TEXTO)
        return larg_texto(f);
    return 0.0;
}
double forma_altura(Forma f) { return f->tipo == FORMA_RETANGULO ? f->b : 0.0; }
double forma_extremo_x(Forma f) { return f->tipo == FORMA_LINHA ? f->a : 0.0; }
double forma_extremo_y(Forma f) { return f->tipo == FORMA_LINHA ? f->b : 0.0; }
const char *forma_texto_conteudo(Forma f) { return f->tipo == FORMA_TEXTO ? f->txt : NULL; }
char forma_ancora_texto(Forma f) { return f->ancora; }
const char *forma_cor_borda(Forma f) { return f->corb; }
const char *forma_cor_preenchimento(Forma f) { return f->corp; }
