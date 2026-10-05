#ifndef FORMA_H
#define FORMA_H
#include <stdio.h>

/*
 * Modulo forma: formas geometricas do plano (circulo, retangulo, linha e
 * texto). TAD opaco: a estrutura so e conhecida em forma.c.
 *
 * Papel de cada forma na pescaria:
 *   retangulo = nau | circulo = peixe | linha = camarao
 *   texto ">-|-<" = lagosta | texto "$" = moeda | outro texto = alga/detrito
 *
 * Convencoes geometricas (o eixo Y cresce para baixo):
 *   - circulo : ancora = centro.
 *   - retangulo: ancora = canto de menor X e menor Y.
 *   - linha   : ancora = extremidade de menor X (menor Y em caso de empate).
 *   - texto   : ancora conforme o parametro 'i' (inicio), 'm' (meio) ou
 *               'f' (fim). O texto ocupa um segmento horizontal de
 *               10 unidades por caractere.
 *   - area: circulo = pi*r^2; retangulo = w*h; linha = 2*comprimento;
 *           texto = 20*numero_de_caracteres.
 */
typedef struct forma_s *Forma;

typedef enum
{
   FORMA_CIRCULO,
   FORMA_RETANGULO,
   FORMA_LINHA,
   FORMA_TEXTO
} TipoForma;

/* Construtores: copiam as strings recebidas. */
Forma forma_circulo(int id, double x, double y, double r,
                    const char *corb, const char *corp);
Forma forma_retangulo(int id, double x, double y, double w, double h,
                      const char *corb, const char *corp);
Forma forma_linha(int id, double x1, double y1, double x2, double y2,
                  const char *cor);
Forma forma_texto(int id, double x, double y, const char *corb,
                  const char *corp, char ancora, const char *txt);

/* Libera a forma. */
void forma_destroi(Forma f);

/* Identificador, tipo, ancora e area. */
int forma_id(Forma f);
TipoForma forma_tipo(Forma f);
double forma_x(Forma f);
double forma_y(Forma f);
double forma_area(Forma f);

/* Retangulo envolvente (minimo) da forma: (x1,y1) canto superior esquerdo,
   (x2,y2) canto inferior direito. */
void forma_bbox(Forma f, double *x1, double *y1, double *x2, double *y2);

/* Desloca a forma de (dx,dy). ATENCAO: se a forma estiver numa arvore, ela
   deve ser removida antes e reinserida depois (a chave de ordenacao muda);
   o modulo pescaria faz isso. */
void forma_move(Forma f, double dx, double dy);

/* Ordem total usada na arvore: X da ancora; empate: area; empate: Y da
   ancora; empate: identificador. Retorna <0, 0 ou >0. */
int forma_compara(Forma a, Forma b);

/* Atributos especificos (0/NULL quando nao se aplicam ao tipo). */
double forma_raio(Forma f);
double forma_largura(Forma f);   /* retangulo (e texto: largura estimada) */
double forma_altura(Forma f);    /* retangulo */
double forma_extremo_x(Forma f); /* linha: X da outra extremidade */
double forma_extremo_y(Forma f); /* linha: Y da outra extremidade */
const char *forma_texto_conteudo(Forma f);
char forma_ancora_texto(Forma f);
const char *forma_cor_borda(Forma f);
const char *forma_cor_preenchimento(Forma f);

/* 1 se for texto ">-|-<" (lagosta) / texto "$" (moeda); 0 caso contrario. */
int forma_eh_lagosta(Forma f);
int forma_eh_moeda(Forma f);

/* Escreve (sem quebra de linha) uma descricao dos dados da forma. */
void forma_descreve(Forma f, FILE *saida);

#endif