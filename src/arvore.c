#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "util.h"

typedef struct no
{
  Forma info;
  int vermelho;
  struct no *esq, *dir;
  double x1, y1, x2, y2; /* mbb da sub-arvore */
} No;

struct arvore_s
{
  No *raiz;
  int n;
  int visitados;
};

static int eh_vermelho(No *n) { return n && n->vermelho; }

/* Recalcula o mbb do no a partir da sua forma e dos mbb dos filhos. */
static void atualiza(No *n)
{
  forma_bbox(n->info, &n->x1, &n->y1, &n->x2, &n->y2);
  No *f[2] = {n->esq, n->dir};
  for (int i = 0; i < 2; i++)
  {
    if (!f[i])
      continue;
    if (f[i]->x1 < n->x1)
      n->x1 = f[i]->x1;
    if (f[i]->y1 < n->y1)
      n->y1 = f[i]->y1;
    if (f[i]->x2 > n->x2)
      n->x2 = f[i]->x2;
    if (f[i]->y2 > n->y2)
      n->y2 = f[i]->y2;
  }
}

static No *novo_no(Forma f)
{
  No *n = calloc(1, sizeof *n);
  if (!n)
  {
    fprintf(stderr, "erro: sem memoria\n");
    exit(1);
  }
  n->info = f;
  n->vermelho = 1;
  atualiza(n);
  return n;
}

static No *rot_esq(No *h)
{
  No *x = h->dir;
  h->dir = x->esq;
  x->esq = h;
  x->vermelho = h->vermelho;
  h->vermelho = 1;
  atualiza(h);
  atualiza(x);
  return x;
}

static No *rot_dir(No *h)
{
  No *x = h->esq;
  h->esq = x->dir;
  x->dir = h;
  x->vermelho = h->vermelho;
  h->vermelho = 1;
  atualiza(h);
  atualiza(x);
  return x;
}

static void inverte_cores(No *h)
{
  h->vermelho = !h->vermelho;
  h->esq->vermelho = !h->esq->vermelho;
  h->dir->vermelho = !h->dir->vermelho;
}

static No *balanceia(No *h)
{
  if (eh_vermelho(h->dir) && !eh_vermelho(h->esq))
    h = rot_esq(h);
  if (eh_vermelho(h->esq) && eh_vermelho(h->esq->esq))
    h = rot_dir(h);
  if (eh_vermelho(h->esq) && eh_vermelho(h->dir))
    inverte_cores(h);
  atualiza(h);
  return h;
}

/* ---------- insercao (recursiva) ---------- */
static No *insere_rec(No *h, Forma f, int *inseriu)
{
  if (!h)
  {
    *inseriu = 1;
    return novo_no(f);
  }
  int c = forma_compara(f, h->info);
  if (c < 0)
    h->esq = insere_rec(h->esq, f, inseriu);
  else if (c > 0)
    h->dir = insere_rec(h->dir, f, inseriu);
  return balanceia(h);
}

int arvore_insere(Arvore a, Forma f)
{
  int ins = 0;
  a->raiz = insere_rec(a->raiz, f, &ins);
  a->raiz->vermelho = 0;
  if (ins)
    a->n++;
  return ins;
}

/* ---------- busca exata (recursiva) ---------- */
static int contem(No *h, Forma f)
{
  if (!h)
    return 0;
  int c = forma_compara(f, h->info);
  if (c == 0)
    return 1;
  return contem(c < 0 ? h->esq : h->dir, f);
}

/* ---------- remocao (recursiva) ---------- */
static No *move_vermelho_esq(No *h)
{
  inverte_cores(h);
  if (eh_vermelho(h->dir->esq))
  {
    h->dir = rot_dir(h->dir);
    h = rot_esq(h);
    inverte_cores(h);
  }
  return h;
}

static No *move_vermelho_dir(No *h)
{
  inverte_cores(h);
  if (eh_vermelho(h->esq->esq))
  {
    h = rot_dir(h);
    inverte_cores(h);
  }
  return h;
}

static No *remove_min(No *h)
{
  if (!h->esq)
  {
    free(h);
    return NULL;
  }
  if (!eh_vermelho(h->esq) && !eh_vermelho(h->esq->esq))
    h = move_vermelho_esq(h);
  h->esq = remove_min(h->esq);
  return balanceia(h);
}

/* Pre-condicao: f esta na sub-arvore h. */
static No *remove_rec(No *h, Forma f)
{
  if (forma_compara(f, h->info) < 0)
  {
    if (!eh_vermelho(h->esq) && !eh_vermelho(h->esq->esq))
      h = move_vermelho_esq(h);
    h->esq = remove_rec(h->esq, f);
  }
  else
  {
    if (eh_vermelho(h->esq))
      h = rot_dir(h);
    if (forma_compara(f, h->info) == 0 && !h->dir)
    {
      free(h);
      return NULL;
    }
    if (!eh_vermelho(h->dir) && !eh_vermelho(h->dir->esq))
      h = move_vermelho_dir(h);
    if (forma_compara(f, h->info) == 0)
    {
      No *m = h->dir;
      while (m->esq)
        m = m->esq;
      h->info = m->info;
      h->dir = remove_min(h->dir);
    }
    else
    {
      h->dir = remove_rec(h->dir, f);
    }
  }
  return balanceia(h);
}

int arvore_remove(Arvore a, Forma f)
{
  if (!contem(a->raiz, f))
    return 0;
  No *r = a->raiz;
  if (!eh_vermelho(r->esq) && !eh_vermelho(r->dir))
    r->vermelho = 1;
  a->raiz = remove_rec(r, f);
  if (a->raiz)
    a->raiz->vermelho = 0;
  a->n--;
  return 1;
}

/* ---------- criacao / destruicao ---------- */
Arvore arvore_cria(void) { return calloc(1, sizeof(struct arvore_s)); }

static void destroi_rec(No *n, void (*libera)(Forma))
{
  if (!n)
    return;
  destroi_rec(n->esq, libera);
  destroi_rec(n->dir, libera);
  if (libera)
    libera(n->info);
  free(n);
}

void arvore_destroi(Arvore a, void (*libera)(Forma))
{
  if (!a)
    return;
  destroi_rec(a->raiz, libera);
  free(a);
}

int arvore_tamanho(Arvore a) { return a->n; }

/* ---------- percursos e buscas ---------- */
static void percorre_rec(No *n, VisitaForma v, void *ctx)
{
  if (!n)
    return;
  percorre_rec(n->esq, v, ctx);
  v(n->info, ctx);
  percorre_rec(n->dir, v, ctx);
}

void arvore_percorre(Arvore a, VisitaForma visita, void *ctx)
{
  percorre_rec(a->raiz, visita, ctx);
}

static void contidas_rec(No *n, double rx1, double ry1, double rx2, double ry2,
                         VisitaForma v, void *ctx, int *vis)
{
  if (!n)
    return;
  /* poda: o mbb da sub-arvore nao intersecta a regiao */
  if (n->x2 < rx1 - UTIL_EPS || n->x1 > rx2 + UTIL_EPS ||
      n->y2 < ry1 - UTIL_EPS || n->y1 > ry2 + UTIL_EPS)
    return;
  (*vis)++;
  contidas_rec(n->esq, rx1, ry1, rx2, ry2, v, ctx, vis);
  double a, b, c, d;
  forma_bbox(n->info, &a, &b, &c, &d);
  if (a >= rx1 - UTIL_EPS && b >= ry1 - UTIL_EPS &&
      c <= rx2 + UTIL_EPS && d <= ry2 + UTIL_EPS)
    v(n->info, ctx);
  contidas_rec(n->dir, rx1, ry1, rx2, ry2, v, ctx, vis);
}

void arvore_busca_contidas(Arvore a, double x, double y, double w, double h,
                           VisitaForma visita, void *ctx)
{
  a->visitados = 0;
  contidas_rec(a->raiz, x, y, x + w, y + h, visita, ctx, &a->visitados);
}

static void ponto_rec(No *n, double px, double py, VisitaForma v, void *ctx, int *vis)
{
  if (!n)
    return;
  if (px < n->x1 - UTIL_EPS || px > n->x2 + UTIL_EPS ||
      py < n->y1 - UTIL_EPS || py > n->y2 + UTIL_EPS)
    return;
  (*vis)++;
  ponto_rec(n->esq, px, py, v, ctx, vis);
  double a, b, c, d;
  forma_bbox(n->info, &a, &b, &c, &d);
  if (px >= a - UTIL_EPS && px <= c + UTIL_EPS &&
      py >= b - UTIL_EPS && py <= d + UTIL_EPS)
    v(n->info, ctx);
  ponto_rec(n->dir, px, py, v, ctx, vis);
}

void arvore_busca_ponto(Arvore a, double x, double y, VisitaForma visita, void *ctx)
{
  a->visitados = 0;
  ponto_rec(a->raiz, x, y, visita, ctx, &a->visitados);
}

int arvore_nos_visitados(Arvore a) { return a->visitados; }

/* ---------- DOT ---------- */
static void dot_rec(No *n, FILE *f)
{
  if (!n)
    return;
  fprintf(f, "  n%d [label=\"%d\", fillcolor=%s, fontcolor=white];\n",
          forma_id(n->info), forma_id(n->info), n->vermelho ? "red" : "black");
  if (n->esq)
  {
    fprintf(f, "  n%d -> n%d;\n", forma_id(n->info), forma_id(n->esq->info));
    dot_rec(n->esq, f);
  }
  if (n->dir)
  {
    fprintf(f, "  n%d -> n%d;\n", forma_id(n->info), forma_id(n->dir->info));
    dot_rec(n->dir, f);
  }
}

int arvore_escreve_dot(Arvore a, FILE *saida)
{
  if (fprintf(saida, "digraph arvore_rubro_negra {\n  node [shape=circle, style=filled];\n") < 0)
    return 0;
  dot_rec(a->raiz, saida);
  return fprintf(saida, "}\n") >= 0;
}

/* ---------- validacao ---------- */
static int valida_cores(No *n)
{ /* altura negra, ou -1 se invalida */
  if (!n)
    return 1;
  if (eh_vermelho(n->dir))
    return -1;
  if (eh_vermelho(n) && eh_vermelho(n->esq))
    return -1;
  int e = valida_cores(n->esq), d = valida_cores(n->dir);
  if (e < 0 || d < 0 || e != d)
    return -1;
  return e + (n->vermelho ? 0 : 1);
}

static int valida_ordem(No *n, Forma *ant)
{
  if (!n)
    return 1;
  if (!valida_ordem(n->esq, ant))
    return 0;
  if (*ant && forma_compara(*ant, n->info) >= 0)
    return 0;
  *ant = n->info;
  return valida_ordem(n->dir, ant);
}

static int valida_mbb(No *n)
{
  if (!n)
    return 1;
  double a, b, c, d;
  forma_bbox(n->info, &a, &b, &c, &d);
  No *f[2] = {n->esq, n->dir};
  for (int i = 0; i < 2; i++)
  {
    if (!f[i])
      continue;
    if (f[i]->x1 < a)
      a = f[i]->x1;
    if (f[i]->y1 < b)
      b = f[i]->y1;
    if (f[i]->x2 > c)
      c = f[i]->x2;
    if (f[i]->y2 > d)
      d = f[i]->y2;
  }
  if (!util_igual(a, n->x1) || !util_igual(b, n->y1) ||
      !util_igual(c, n->x2) || !util_igual(d, n->y2))
    return 0;
  return valida_mbb(n->esq) && valida_mbb(n->dir);
}

int arvore_valida(Arvore a)
{
  if (!a->raiz)
    return 1;
  Forma ant = NULL;
  return !eh_vermelho(a->raiz) && valida_cores(a->raiz) > 0 &&
         valida_ordem(a->raiz, &ant) && valida_mbb(a->raiz);
}