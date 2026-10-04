
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "geo.h"

#define LINHA_MAX 4096

int geo_carrega(const char *caminho, Pescaria p)
{
    FILE *f = fopen(caminho, "r");
    if (!f)
        return 0;
    char linha[LINHA_MAX];
    while (fgets(linha, sizeof linha, f))
    {
        char cmd[8], cb[64], cp[64];
        int off = 0, id;
        double x, y, a, b;
        if (sscanf(linha, "%7s%n", cmd, &off) != 1)
            continue;
        const char *r = linha + off;
        Forma fm = NULL;
        if (strcmp(cmd, "c") == 0)
        {
            if (sscanf(r, "%d %lf %lf %lf %63s %63s", &id, &x, &y, &a, cb, cp) == 6)
                fm = forma_circulo(id, x, y, a, cb, cp);
        }
        else if (strcmp(cmd, "r") == 0)
        {
            if (sscanf(r, "%d %lf %lf %lf %lf %63s %63s", &id, &x, &y, &a, &b, cb, cp) == 7)
                fm = forma_retangulo(id, x, y, a, b, cb, cp);
        }
        else if (strcmp(cmd, "l") == 0)
        {
            if (sscanf(r, "%d %lf %lf %lf %lf %63s", &id, &x, &y, &a, &b, cb) == 6)
                fm = forma_linha(id, x, y, a, b, cb);
        }
        else if (strcmp(cmd, "t") == 0)
        {
            char anc;
            int n = 0;
            if (sscanf(r, "%d %lf %lf %63s %63s %c%n", &id, &x, &y, cb, cp, &anc, &n) == 6)
            {
                const char *t = r + n;
                while (*t == ' ' || *t == '\t')
                    t++;
                char txt[LINHA_MAX];
                strncpy(txt, t, sizeof txt - 1);
                txt[sizeof txt - 1] = '\0';
                size_t len = strlen(txt);
                while (len > 0 && isspace((unsigned char)txt[len - 1]))
                    txt[--len] = '\0';
                fm = forma_texto(id, x, y, cb, cp, anc, txt);
            }
        }
        if (fm && !pescaria_adiciona(p, fm))
        {
            fprintf(stderr, "aviso: id %d repetido, forma ignorada\n", forma_id(fm));
            forma_destroi(fm);
        }
    }
    fclose(f);
    return 1;
}
