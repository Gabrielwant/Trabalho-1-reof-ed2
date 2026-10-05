#ifndef QRY_H
#define QRY_H
#include <stdio.h>
#include "pescaria.h"
#include "strbuf.h"

/*
 * Modulo qry: execucao dos comandos do arquivo .qry.
 *
 *   e i j v            energiza as naus com id em [i,j] com o nivel v
 *   mv i dx dy         desloca a forma i (nau: gasta d/5 de energia, d=|(dx,dy)|)
 *   lr i lado d w h    nau i lanca rede (w x h) a distancia d do lado
 *                      (custo (w*h/25)*(d/5)); captura formas totalmente
 *                      dentro da rede
 *   d i lado dist      nau i dispara o canhao do lado (custo dist)
 *   mc dx dy x y w h   translada de (dx,dy) os peixes dentro da regiao
 *
 * Posicao da rede em relacao a nau (x,y,W,H) -- a rede mantem o alinhamento
 * da ancora da nau:
 *   PP (Y menor): rede em (x, y-d-h)      PR (Y maior): rede em (x, y+H+d)
 *   EB (X menor): rede em (x-d-w, y)      BB (X maior): rede em (x+W+d, y)
 * O canhao de cada lado fica no ponto medio do lado e atira para fora.
 */

/* Executa o arquivo de consultas.
 *   txt    : recebe, para cada comando, "[*] <comando>" e o resultado; ao
 *            final, a contabilidade por nau.
 *   extras : recebe os trechos SVG (anotacoes) a desenhar sobre o estado final.
 * Retorna 1 em sucesso, 0 se o arquivo nao pode ser aberto. */
int qry_executa(const char *caminho, Pescaria p, FILE *txt, StrBuf extras);

#endif