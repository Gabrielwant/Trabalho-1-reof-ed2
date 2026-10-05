#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "qry.h"
#include "arvore.h"
#include "vetor.h"
#include "util.h"

#define LINHA_MAX 4096
#define ENERGIA_MOEDA 2.5
#define VALOR_LAGOSTA 20.0
#define VALOR_CAMARAO 1.0
#define VALOR_PEIXE 5.0

typedef enum { LADO_PP, LADO_PR, LADO_EB, LADO_BB, LADO_INVALIDO } Lado;

static Lado parse_lado(const char *s) {
    if (strcmp(s, "PP") == 0) return LADO_PP;
    if (strcmp(s, "PR") == 0) return LADO_PR;
    if (strcmp(s, "EB") == 0) return LADO_EB;
    if (strcmp(s, "BB") == 0) return LADO_BB;
    return LADO_INVALIDO;
}

/* Ponto medio do lado da nau (posicao do canhao). */
static void ponto_lado(Forma nau, Lado l, double *px, double *py) {
    double x = forma_x(nau), y = forma_y(nau);
    double w = forma_largura(nau), h = forma_altura(nau);
    switch (l) {
    case LADO_PP: *px = x + w / 2; *py = y; break;
    case LADO_PR: *px = x + w / 2; *py = y + h; break;
    case LADO_EB: *px = x; *py = y + h / 2; break;
    default:      *px = x + w; *py = y + h / 2;
    }
}

static void coleta(Forma f, void *ctx) { vetor_adiciona((Vetor)ctx, f); }

/* ---------- utilitarios de SVG ---------- */
static void svg_regiao_tracejada(StrBuf s, double x, double y, double w, double h) {
    strbuf_printf(s, "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" "
                     "style=\"fill:none;stroke:black;stroke-width:1;stroke-dasharray:4,2\" />\n",
                  x, y, w, h);
}

static void svg_circulo_amarelo(StrBuf s, double x, double y) {
    strbuf_printf(s, "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"2\" "
                     "style=\"fill:yellow;stroke:black;stroke-width:0.5\" />\n", x, y);
}

static void svg_quadrado_amarelo(StrBuf s, double x, double y) {
    strbuf_printf(s, "  <rect x=\"%.2f\" y=\"%.2f\" width=\"4\" height=\"4\" "
                     "style=\"fill:yellow;stroke:black;stroke-width:0.5\" />\n", x - 2, y - 2);
}

static void svg_asterisco(StrBuf s, double x, double y) {
    static const double seg[3][4] = { {-3, 0, 3, 0}, {-1.5, -2.6, 1.5, 2.6}, {-1.5, 2.6, 1.5, -2.6} };
    for (int i = 0; i < 3; i++)
        strbuf_printf(s, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" "
                         "style=\"stroke:red;stroke-width:0.8\" />\n",
                      x + seg[i][0], y + seg[i][1], x + seg[i][2], y + seg[i][3]);
}

static void svg_x(StrBuf s, double x, double y) {
    strbuf_printf(s, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" style=\"stroke:red;stroke-width:1\" />\n",
                  x - 2, y - 2, x + 2, y + 2);
    strbuf_printf(s, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" style=\"stroke:red;stroke-width:1\" />\n",
                  x - 2, y + 2, x + 2, y - 2);
}

/* ---------- ordenacao de naus por id ---------- */
static int cmp_nau(const void *a, const void *b) {
    int x = nau_id(*(Nau const *)a), y = nau_id(*(Nau const *)b);
    return (x > y) - (x < y);
}

typedef struct { Vetor v; int i, j; } CtxFaixa;

static void coleta_nau_faixa(Nau n, void *c) {
    CtxFaixa *x = c;
    if (!nau_destruida(n) && nau_id(n) >= x->i && nau_id(n) <= x->j) vetor_adiciona(x->v, n);
}

static void coleta_nau_todas(Nau n, void *c) { vetor_adiciona((Vetor)c, n); }

/* ---------- comandos ---------- */
static void cmd_e(Pescaria p, const char *args, FILE *txt) {
    int i, j;
    double v;
    if (sscanf(args, "%d %d %lf", &i, &j, &v) != 3) { fprintf(txt, "  comando invalido\n"); return; }
    CtxFaixa c = { vetor_cria(), i, j };
    pescaria_percorre_naus(p, coleta_nau_faixa, &c);
    vetor_ordena(c.v, cmp_nau);
    for (int k = 0; k < vetor_tamanho(c.v); k++) {
        Nau n = vetor_obtem(c.v, k);
        nau_define_energia(n, v);
        fprintf(txt, "  ");
        forma_descreve(nau_forma(n), txt);
        fprintf(txt, " energia=%.2f\n", nau_energia(n));
    }
    if (vetor_tamanho(c.v) == 0) fprintf(txt, "  nenhuma nau no intervalo\n");
    vetor_destroi(c.v);
}

static void cmd_mv(Pescaria p, const char *args, FILE *txt) {
    int i;
    double dx, dy;
    if (sscanf(args, "%d %lf %lf", &i, &dx, &dy) != 3) { fprintf(txt, "  comando invalido\n"); return; }
    Forma f = pescaria_forma(p, i);
    if (!f) { fprintf(txt, "  forma %d inexistente\n", i); return; }
    double x0 = forma_x(f), y0 = forma_y(f);
    Nau n = forma_tipo(f) == FORMA_RETANGULO ? pescaria_nau(p, i) : NULL;
    double e0 = n ? nau_energia(n) : 0.0;
    /* O enunciado nao exige energia minima para deslocar: a nau sempre se
       move e a energia nunca fica negativa. */
    if (n) nau_gasta(n, sqrt(dx * dx + dy * dy) / 5.0);
    pescaria_move(p, f, dx, dy);
    fprintf(txt, "  ");
    forma_descreve(f, txt);
    fprintf(txt, "\n  posicao inicial: (%.2f, %.2f)\n  posicao final: (%.2f, %.2f)\n",
            x0, y0, forma_x(f), forma_y(f));
    if (n) fprintf(txt, "  energia: %.2f -> %.2f\n", e0, nau_energia(n));
}

static void cmd_lr(Pescaria p, const char *args, FILE *txt, StrBuf sv) {
    int i;
    char ls[16];
    double d, w, h;
    if (sscanf(args, "%d %15s %lf %lf %lf", &i, ls, &d, &w, &h) != 5) { fprintf(txt, "  comando invalido\n"); return; }
    Nau n = pescaria_nau(p, i);
    if (!n || nau_destruida(n)) { fprintf(txt, "  nau %d inexistente\n", i); return; }
    Lado l = parse_lado(ls);
    if (l == LADO_INVALIDO) { fprintf(txt, "  lado invalido: %s\n", ls); return; }
    Forma nf = nau_forma(n);
    double custo = (w * h) / 25.0 * (d / 5.0), e0 = nau_energia(n);
    if (e0 + UTIL_EPS < custo) {
        double px, py;
        ponto_lado(nf, l, &px, &py);
        svg_circulo_amarelo(sv, px, py);
        fprintf(txt, "  energia insuficiente: necessaria %.2f, disponivel %.2f\n", custo, e0);
        return;
    }
    double rx = forma_x(nf), ry = forma_y(nf);
    switch (l) {
    case LADO_PP: ry = forma_y(nf) - d - h; break;
    case LADO_PR: ry = forma_y(nf) + forma_altura(nf) + d; break;
    case LADO_EB: rx = forma_x(nf) - d - w; break;
    default:      rx = forma_x(nf) + forma_largura(nf) + d;
    }
    svg_regiao_tracejada(sv, rx, ry, w, h);
    Vetor v = vetor_cria();
    arvore_busca_contidas(pescaria_arvore(p), rx, ry, w, h, coleta, v);
    nau_gasta(n, custo);
    int lag = 0, cam = 0, pei = 0, moe = 0, det = 0;
    double total = 0;
    fprintf(txt, "  rede em (%.2f, %.2f) %.2f x %.2f; custo de energia %.2f\n", rx, ry, w, h, custo);
    for (int k = 0; k < vetor_tamanho(v); k++) {
        Forma f = vetor_obtem(v, k);
        const char *classe;
        double valor = 0;
        if (forma_tipo(f) == FORMA_RETANGULO) continue; /* naus nao sao capturadas */
        if (forma_tipo(f) == FORMA_CIRCULO)      { classe = "peixe";   valor = VALOR_PEIXE;   pei++; }
        else if (forma_tipo(f) == FORMA_LINHA)   { classe = "camarao"; valor = VALOR_CAMARAO; cam++; }
        else if (forma_eh_lagosta(f))            { classe = "lagosta"; valor = VALOR_LAGOSTA; lag++; }
        else if (forma_eh_moeda(f))              { classe = "moeda";   moe++; }
        else                                     { classe = "descartado"; det++; }
        total += valor;
        fprintf(txt, "  capturado: ");
        forma_descreve(f, txt);
        fprintf(txt, " | %s | valor M$ %.2f\n", classe, valor);
        pescaria_remove(p, f);
    }
    nau_ganha(n, ENERGIA_MOEDA * moe);
    nau_soma_riqueza(n, total);
    nau_conta(n, lag, cam, pei, moe);
    fprintf(txt, "  total desta captura: M$ %.2f (lagostas=%d camaroes=%d peixes=%d moedas=%d descartados=%d)\n",
            total, lag, cam, pei, moe, det);
    fprintf(txt, "  soma de todas as capturas da nau %d: M$ %.2f\n", i, nau_riqueza(n));
    fprintf(txt, "  energia antes: %.2f, depois: %.2f\n", e0, nau_energia(n));
    vetor_destroi(v);
}

static void cmd_d(Pescaria p, const char *args, FILE *txt, StrBuf sv) {
    int i;
    char ls[16];
    double dist;
    if (sscanf(args, "%d %15s %lf", &i, ls, &dist) != 3) { fprintf(txt, "  comando invalido\n"); return; }
    Nau n = pescaria_nau(p, i);
    if (!n || nau_destruida(n)) { fprintf(txt, "  nau %d inexistente\n", i); return; }
    Lado l = parse_lado(ls);
    if (l == LADO_INVALIDO) { fprintf(txt, "  lado invalido: %s\n", ls); return; }
    Forma nf = nau_forma(n);
    double px, py, e0 = nau_energia(n);
    ponto_lado(nf, l, &px, &py);
    if (e0 + UTIL_EPS < dist) {
        svg_quadrado_amarelo(sv, px, py);
        fprintf(txt, "  energia insuficiente: necessaria %.2f, disponivel %.2f\n", dist, e0);
        return;
    }
    nau_gasta(n, dist);
    double ix = px, iy = py;
    switch (l) {
    case LADO_PP: iy -= dist; break;
    case LADO_PR: iy += dist; break;
    case LADO_EB: ix -= dist; break;
    default:      ix += dist;
    }
    svg_asterisco(sv, ix, iy);
    fprintf(txt, "  canhao %s da nau %d disparado; ponto de impacto: (%.2f, %.2f); energia %.2f -> %.2f\n",
            ls, i, ix, iy, e0, nau_energia(n));
    Vetor v = vetor_cria();
    arvore_busca_ponto(pescaria_arvore(p), ix, iy, coleta, v);
    int atingidas = 0;
    for (int k = 0; k < vetor_tamanho(v); k++) {
        Forma f = vetor_obtem(v, k);
        if (forma_tipo(f) != FORMA_RETANGULO || forma_id(f) == i) continue;
        Nau alvo = pescaria_nau(p, forma_id(f));
        if (!alvo || nau_destruida(alvo)) continue;
        double riq = nau_riqueza(alvo);
        fprintf(txt, "  nau atingida e destruida: ");
        forma_descreve(f, txt);
        fprintf(txt, "\n  riqueza adquirida: M$ %.2f\n", riq);
        nau_soma_riqueza(n, riq);
        nau_soma_riqueza(alvo, -riq);
        svg_x(sv, forma_x(f), forma_y(f));
        nau_marca_destruida(alvo);
        pescaria_remove(p, f);
        atingidas++;
    }
    if (!atingidas) fprintf(txt, "  nenhuma nau atingida\n");
    vetor_destroi(v);
}

static void cmd_mc(Pescaria p, const char *args, FILE *txt, StrBuf sv) {
    double dx, dy, x, y, w, h;
    if (sscanf(args, "%lf %lf %lf %lf %lf %lf", &dx, &dy, &x, &y, &w, &h) != 6) {
        fprintf(txt, "  comando invalido\n");
        return;
    }
    svg_regiao_tracejada(sv, x, y, w, h);
    svg_regiao_tracejada(sv, x + dx, y + dy, w, h);
    Vetor v = vetor_cria();
    arvore_busca_contidas(pescaria_arvore(p), x, y, w, h, coleta, v);
    int movidos = 0;
    for (int k = 0; k < vetor_tamanho(v); k++) {
        Forma f = vetor_obtem(v, k);
        if (forma_tipo(f) != FORMA_CIRCULO) continue;
        pescaria_move(p, f, dx, dy);
        movidos++;
    }
    fprintf(txt, "  peixes transladados: %d\n", movidos);
    vetor_destroi(v);
}

static void contabilidade(Pescaria p, FILE *txt) {
    Vetor v = vetor_cria();
    pescaria_percorre_naus(p, coleta_nau_todas, v);
    vetor_ordena(v, cmp_nau);
    fprintf(txt, "\n=== CONTABILIDADE FINAL DA PESCARIA ===\n");
    for (int k = 0; k < vetor_tamanho(v); k++) {
        Nau n = vetor_obtem(v, k);
        fprintf(txt, "nau %d: energia final=%.2f riqueza=M$ %.2f (lagostas=%d camaroes=%d peixes=%d moedas=%d)%s\n",
                nau_id(n), nau_energia(n), nau_riqueza(n), nau_lagostas(n), nau_camaroes(n),
                nau_peixes(n), nau_moedas(n), nau_destruida(n) ? " [destruida]" : "");
    }
    vetor_destroi(v);
}

int qry_executa(const char *caminho, Pescaria p, FILE *txt, StrBuf extras) {
    FILE *f = fopen(caminho, "r");
    if (!f) return 0;
    char linha[LINHA_MAX];
    while (fgets(linha, sizeof linha, f)) {
        size_t len = strlen(linha);
        while (len > 0 && isspace((unsigned char)linha[len - 1])) linha[--len] = '\0';
        char cmd[8];
        int off = 0;
        if (sscanf(linha, "%7s%n", cmd, &off) != 1) continue;
        const char *args = linha + off;
        fprintf(txt, "[*] %s\n", linha);
        if (strcmp(cmd, "e") == 0)        cmd_e(p, args, txt);
        else if (strcmp(cmd, "mv") == 0)  cmd_mv(p, args, txt);
        else if (strcmp(cmd, "lr") == 0)  cmd_lr(p, args, txt, extras);
        else if (strcmp(cmd, "d") == 0)   cmd_d(p, args, txt, extras);
        else if (strcmp(cmd, "mc") == 0)  cmd_mc(p, args, txt, extras);
        else fprintf(txt, "  comando desconhecido\n");
    }
    fclose(f);
    contabilidade(p, txt);
    return 1;
}