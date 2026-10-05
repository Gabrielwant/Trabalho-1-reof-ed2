#include <stdlib.h>
#include "pescaria.h"
#include "tabela.h"

struct pescaria_s
{
    Arvore arv;
    Tabela formas;
    Tabela naus;
};

static void libera_forma(void *p) { forma_destroi(p); }
static void libera_nau(void *p) { nau_destroi(p); }

Pescaria pescaria_cria(void)
{
    Pescaria p = malloc(sizeof *p);
    if (!p)
        return NULL;
    p->arv = arvore_cria();
    p->formas = tabela_cria();
    p->naus = tabela_cria();
    return p;
}

void pescaria_destroi(Pescaria p)
{
    if (!p)
        return;
    arvore_destroi(p->arv, NULL);
    tabela_destroi(p->formas, libera_forma);
    tabela_destroi(p->naus, libera_nau);
    free(p);
}

int pescaria_adiciona(Pescaria p, Forma f)
{
    if (!tabela_insere(p->formas, forma_id(f), f))
        return 0;
    arvore_insere(p->arv, f);
    if (forma_tipo(f) == FORMA_RETANGULO)
        tabela_insere(p->naus, forma_id(f), nau_cria(f));
    return 1;
}

Forma pescaria_forma(Pescaria p, int id) { return tabela_busca(p->formas, id); }
Nau pescaria_nau(Pescaria p, int id) { return tabela_busca(p->naus, id); }

void pescaria_remove(Pescaria p, Forma f)
{
    arvore_remove(p->arv, f);
    tabela_remove(p->formas, forma_id(f));
    forma_destroi(f);
}

void pescaria_move(Pescaria p, Forma f, double dx, double dy)
{
    arvore_remove(p->arv, f); /* a chave muda: remove, move e reinsere */
    forma_move(f, dx, dy);
    arvore_insere(p->arv, f);
}

Arvore pescaria_arvore(Pescaria p) { return p->arv; }

typedef struct
{
    void (*cb)(Nau, void *);
    void *ctx;
} Adaptador;

static void adapta(int id, void *v, void *c)
{
    (void)id;
    Adaptador *a = c;
    a->cb(v, a->ctx);
}

void pescaria_percorre_naus(Pescaria p, void (*cb)(Nau, void *), void *ctx)
{
    Adaptador a = {cb, ctx};
    tabela_percorre(p->naus, adapta, &a);
}