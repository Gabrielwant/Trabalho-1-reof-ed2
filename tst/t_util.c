
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

int main(void) {
    assert(util_igual(1.0, 1.0 + 1e-12));
    assert(!util_igual(1.0, 1.001));
    assert(util_compara(1.0, 2.0) < 0 && util_compara(2.0, 1.0) > 0 && util_compara(3.0, 3.0) == 0);
    char *s = util_nome_base("a/b/t01.geo");
    assert(strcmp(s, "t01") == 0); free(s);
    s = util_nome_base("q1.qry"); assert(strcmp(s, "q1") == 0); free(s);
    s = util_junta("dir/", "f.svg"); assert(strcmp(s, "dir/f.svg") == 0); free(s);
    s = util_junta("dir", "f.svg"); assert(strcmp(s, "dir/f.svg") == 0); free(s);
    s = util_junta(NULL, "f.svg"); assert(strcmp(s, "f.svg") == 0); free(s);
    s = util_junta("/", "f"); assert(strcmp(s, "/f") == 0); free(s);
    assert(util_cria_dir("../tst/bin/a/b/c/"));
    assert(util_cria_dir("../tst/bin/a/b/c"));
    puts("OK util");
    return 0;
}
