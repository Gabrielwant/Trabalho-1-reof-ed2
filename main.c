
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pescaria.h"
#include "geo.h"
#include "qry.h"
#include "svg.h"
#include "strbuf.h"
#include "util.h"

static char *nome_saida(const char *dir, const char *base, const char *qbase, const char *ext)
{
    size_t n = strlen(base) + (qbase ? strlen(qbase) + 1 : 0) + strlen(ext) + 2;
    char *nome = malloc(n);
    if (!nome)
        return NULL;
    if (qbase)
        snprintf(nome, n, "%s-%s.%s", base, qbase, ext);
    else
        snprintf(nome, n, "%s.%s", base, ext);
    char *r = util_junta(dir, nome);
    free(nome);
    return r;
}

static void grava_dot(Pescaria p, const char *caminho)
{
    FILE *f = fopen(caminho, "w");
    if (!f)
    {
        fprintf(stderr, "erro: nao foi possivel criar %s\n", caminho);
        return;
    }
    arvore_escreve_dot(pescaria_arvore(p), f);
    fclose(f);
}

int main(int argc, char *argv[])
{
    const char *dir_e = NULL, *arq_f = NULL, *arq_q = NULL, *dir_o = NULL;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc)
            dir_e = argv[++i];
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
            arq_f = argv[++i];
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
            dir_o = argv[++i];
        else
            fprintf(stderr, "aviso: parametro ignorado: %s\n", argv[i]);
    }
    if (!arq_f || !dir_o)
    {
        fprintf(stderr, "uso: %s [-e dir_entrada] -f arq.geo [-q consulta.qry] -o dir_saida\n", argv[0]);
        return 1;
    }

    char *geo_path = util_junta(dir_e, arq_f);
    char *base = util_nome_base(arq_f);
    Pescaria p = pescaria_cria();
    if (!geo_carrega(geo_path, p))
    {
        fprintf(stderr, "erro: nao foi possivel abrir %s\n", geo_path);
        return 1;
    }

    char *svg0 = nome_saida(dir_o, base, NULL, "svg");
    svg_gera(p, svg0, NULL);
    free(svg0);

    if (arq_q)
    {
    }
    else
    {
        char *dot0 = nome_saida(dir_o, base, NULL, "dot");
        grava_dot(p, dot0);
        free(dot0);
    }

    pescaria_destroi(p);
    free(geo_path);
    free(base);
    return 0;
}
