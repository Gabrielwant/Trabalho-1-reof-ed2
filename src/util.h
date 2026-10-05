#ifndef UTIL_H
#define UTIL_H

/*
 * Modulo util: funcoes auxiliares (comparacao de reais, manipulacao de
 * nomes de arquivos e diretorios).
 */

/* Tolerancia usada na comparacao de numeros reais. */
#define UTIL_EPS 1e-9

/* Retorna 1 se |a-b| < UTIL_EPS, 0 caso contrario. */
int util_igual(double a, double b);

/* Compara dois reais com tolerancia: retorna -1 (a<b), 0 (a=b) ou 1 (a>b). */
int util_compara(double a, double b);

/* Duplica a string s (alocada com malloc; o chamador deve dar free). */
char *util_dup(const char *s);

/* Retorna (malloc) o nome-base de um caminho: sem diretorios e sem extensao.
   Ex.: "a/b/t01.geo" -> "t01". */
char *util_nome_base(const char *caminho);

/* Retorna (malloc) "dir/arq", tratando dir com ou sem barra final.
   Se dir for NULL ou vazio, retorna copia de arq. */
char *util_junta(const char *dir, const char *arq);

/* Cria o diretorio (e os pais que faltarem). Retorna 1 se existe ao final. */
int util_cria_dir(const char *caminho);

#endif