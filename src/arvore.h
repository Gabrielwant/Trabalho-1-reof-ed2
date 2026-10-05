#ifndef ARVORE_H
#define ARVORE_H
#include <stdio.h>
#include "forma.h"

/*
 * Modulo arvore: arvore Rubro-Negra (variante Left-Leaning, 2-3) de formas.
 * Insercao, remocao e buscas sao RECURSIVAS.
 *
 * Ordenacao: ver forma_compara (X da ancora, area, Y da ancora, id).
 *
 * Cada no guarda tambem o retangulo envolvente minimo (mbb) da forma do no
 * e de todas as formas de suas sub-arvores:
 *     mbb(p) = bbox(info(p)) U mbb(p.esq) U mbb(p.dir)
 * Esse retangulo e usado para PODAR as buscas por regiao: uma sub-arvore
 * cujo mbb nao intersecta a regiao buscada nao e visitada.
 *
 * A arvore NAO e dona das formas (so guarda os ponteiros).
 */
typedef struct arvore_s *Arvore;

/* Funcao de visita: recebe a forma e o contexto do chamador. */
typedef void (*VisitaForma)(Forma f, void *ctx);

/* Cria arvore vazia. */
Arvore arvore_cria(void);

/* Libera os nos. Se libera != NULL, chama libera(forma) para cada forma. */
void arvore_destroi(Arvore a, void (*libera)(Forma));

/* Insere a forma. Se ja houver forma de mesma chave, nada e feito.
   Retorna 1 se inseriu. */
int arvore_insere(Arvore a, Forma f);

/* Remove a forma (localizada pela chave). Retorna 1 se removeu.
   A forma NAO e liberada. */
int arvore_remove(Arvore a, Forma f);

/* Numero de formas na arvore. */
int arvore_tamanho(Arvore a);

/* Percorre em ordem (crescente) chamando visita para cada forma. */
void arvore_percorre(Arvore a, VisitaForma visita, void *ctx);

/* Visita as formas cujo retangulo envolvente esta TOTALMENTE contido na
   regiao retangular de canto (x,y), largura w e altura h. Com poda. */
void arvore_busca_contidas(Arvore a, double x, double y, double w, double h,
                           VisitaForma visita, void *ctx);

/* Visita as formas cujo retangulo envolvente contem o ponto (x,y). Com poda. */
void arvore_busca_ponto(Arvore a, double x, double y,
                        VisitaForma visita, void *ctx);

/* Quantidade de nos visitados na ultima busca (util para verificar a poda). */
int arvore_nos_visitados(Arvore a);

/* Escreve a arvore em formato DOT (Graphviz); nos vermelhos ou pretos,
   rotulados com o identificador da forma. Retorna 1 em sucesso. */
int arvore_escreve_dot(Arvore a, FILE *saida);

/* Verifica os invariantes (raiz preta, sem vermelho consecutivo, sem ligacao
   vermelha a direita, mesma altura negra, ordem e mbb corretos).
   Retorna 1 se valida. Usada nos testes. */
int arvore_valida(Arvore a);

#endif