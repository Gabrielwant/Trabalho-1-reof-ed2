
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "util.h"

int util_igual(double a, double b) { return fabs(a - b) < UTIL_EPS; }

int util_compara(double a, double b)
{
    if (util_igual(a, b))
        return 0;
    return a < b ? -1 : 1;
}

char *util_dup(const char *s)
{
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (p)
        memcpy(p, s, n);
    return p;
}

char *util_nome_base(const char *caminho)
{
    const char *b = strrchr(caminho, '/');
    b = b ? b + 1 : caminho;
    char *r = util_dup(b);
    if (!r)
        return NULL;
    char *p = strrchr(r, '.');
    if (p && p != r)
        *p = '\0';
    return r;
}

char *util_junta(const char *dir, const char *arq)
{
    if (!dir || !*dir)
        return util_dup(arq);
    size_t n = strlen(dir);
    while (n > 1 && dir[n - 1] == '/')
        n--;
    char *r = malloc(n + strlen(arq) + 2);
    if (!r)
        return NULL;
    memcpy(r, dir, n);
    r[n] = '\0';
    if (!(n == 1 && dir[0] == '/'))
        strcat(r, "/");
    strcat(r, arq);
    return r;
}

int util_cria_dir(const char *caminho)
{
    char *tmp = util_dup(caminho);
    if (!tmp)
        return 0;
    size_t n = strlen(tmp);
    while (n > 1 && tmp[n - 1] == '/')
        tmp[--n] = '\0';
    for (char *p = tmp + 1; *p; p++)
    {
        if (*p == '/')
        {
            *p = '\0';
            if (mkdir(tmp, 0777) != 0 && errno != EEXIST)
            {
                free(tmp);
                return 0;
            }
            *p = '/';
        }
    }
    int ok = (mkdir(tmp, 0777) == 0 || errno == EEXIST);
    free(tmp);
    return ok;
}
