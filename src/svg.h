#ifndef SVG_H
#define SVG_H
#include <stdio.h>
#include "forma.h"
#include "pescaria.h"

/*
 * Modulo svg: gera arquivos SVG a partir das formas.
 */

/* Escreve o cabecalho / rodape do documento SVG. */
void svg_inicio(FILE *f);
void svg_fim(FILE *f);

/* Desenha uma forma. Se cor_borda != NULL e larg_borda > 0, substituem a cor
   e a largura do contorno da forma (usado nas naus). */
void svg_forma(FILE *f, Forma fm, const char *cor_borda, double larg_borda);

/* Grava em 'caminho' um SVG com todas as formas vivas da pescaria (naus com
   contorno conforme a energia) seguidas do trecho SVG 'extras' (anotacoes
   dos comandos; pode ser NULL). Retorna 1 em sucesso. */
int svg_gera(Pescaria p, const char *caminho, const char *extras);

#endif