
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "strbuf.h"

int main(void)
{
    StrBuf s = strbuf_cria();
    assert(strlen(strbuf_conteudo(s)) == 0);
    strbuf_printf(s, "a=%d;", 5);
    for (int i = 0; i < 1000; i++)
        strbuf_printf(s, "%s", "xyz");
    strbuf_printf(s, "fim");
    assert(strlen(strbuf_conteudo(s)) == 4 + 3000 + 3);
    assert(strncmp(strbuf_conteudo(s), "a=5;xyz", 7) == 0);
    strbuf_destroi(s);
    puts("OK strbuf");
    return 0;
}
