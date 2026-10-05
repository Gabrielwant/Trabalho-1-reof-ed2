
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "strbuf.h"

struct strbuf_s
{
    char *dados;
    size_t tam, cap;
};

StrBuf strbuf_cria(void)
{
    StrBuf s = malloc(sizeof *s);
    if (!s)
        return NULL;
    s->cap = 256;
    s->tam = 0;
    s->dados = malloc(s->cap);
    if (!s->dados)
    {
        free(s);
        return NULL;
    }
    s->dados[0] = '\0';
    return s;
}

void strbuf_destroi(StrBuf s)
{
    if (!s)
        return;
    free(s->dados);
    free(s);
}

void strbuf_printf(StrBuf s, const char *fmt, ...)
{
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    int n = vsnprintf(NULL, 0, fmt, ap); /* tamanho do texto a acrescentar */
    va_end(ap);
    if (n < 0)
    {
        va_end(ap2);
        return;
    }

    (void)s;
    va_end(ap2);
}

const char *strbuf_conteudo(StrBuf s) { return s->dados; }
