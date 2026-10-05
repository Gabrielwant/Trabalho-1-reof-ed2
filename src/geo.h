#ifndef GEO_H
#define GEO_H
#include "pescaria.h"

/*
 * Modulo geo: leitura de arquivos .geo (descricao das formas).
 *
 * Comandos (um por linha, parametros separados por espaco):
 *   c i x y r corb corp          circulo (peixe)
 *   r i x y w h corb corp        retangulo (nau)
 *   l i x1 y1 x2 y2 cor          linha (camarao)
 *   t i x y corb corp a txto     texto (a = i|m|f; txto vai ate o fim da linha)
 * Linhas vazias ou com comando desconhecido sao ignoradas.
 */

/* Le o arquivo e adiciona as formas a pescaria. Retorna 1 em sucesso,
   0 se o arquivo nao pode ser aberto. Ids repetidos sao ignorados (aviso
   em stderr). */
int geo_carrega(const char *caminho, Pescaria p);

#endif