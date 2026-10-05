#include <stdlib.h>
#include "nau.h"
#include "util.h"

struct nau_s
{
    int id;
    Forma forma;
    double energia, riqueza;
    int lagostas, camaroes, peixes, moedas;
    int destruida;
};

Nau nau_cria(Forma f)
{
    Nau n = calloc(1, sizeof *n);
    if (!n)
        return NULL;
    n->id = forma_id(f);
    n->forma = f;
    return n;
}

void nau_destroi(Nau n) { free(n); }
int nau_id(Nau n) { return n->id; }
Forma nau_forma(Nau n) { return n->forma; }
double nau_energia(Nau n) { return n->energia; }
void nau_define_energia(Nau n, double e) { n->energia = e < 0 ? 0 : e; }

void nau_gasta(Nau n, double e)
{
    n->energia -= e;
    if (n->energia < 0)
        n->energia = 0;
}

void nau_ganha(Nau n, double e) { n->energia += e; }
double nau_riqueza(Nau n) { return n->riqueza; }
void nau_soma_riqueza(Nau n, double v) { n->riqueza += v; }

void nau_conta(Nau n, int l, int c, int p, int m)
{
    n->lagostas += l;
    n->camaroes += c;
    n->peixes += p;
    n->moedas += m;
}

int nau_lagostas(Nau n) { return n->lagostas; }
int nau_camaroes(Nau n) { return n->camaroes; }
int nau_peixes(Nau n) { return n->peixes; }
int nau_moedas(Nau n) { return n->moedas; }
int nau_destruida(Nau n) { return n->destruida; }

void nau_marca_destruida(Nau n)
{
    n->destruida = 1;
    n->forma = NULL;
}

const char *nau_cor_contorno(Nau n)
{
    double e = n->energia;
    if (util_igual(e, 0.0) || e < 0.0)
        return "#484537";
    if (e < 100.0)
        return "#FFCC00";
    if (e < 250.0)
        return "#217821";
    return "#800066";
}

double nau_largura_contorno(Nau n) { return n->energia >= 250.0 ? 3.0 : 2.0; }