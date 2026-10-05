#ifndef NAU_H
#define NAU_H
#include "forma.h"

/*
 * Modulo nau: estado de uma nau (retangulo) da pescaria: nivel de energia,
 * riqueza acumulada (em M$ - merrecas) e contagem de capturas.
 * TAD opaco. Uma nau destruida continua existindo como registro (para a
 * contabilidade final), mas sem forma associada.
 */
typedef struct nau_s *Nau;

/* Cria a nau associada a forma f (retangulo); energia e riqueza = 0. */
Nau nau_cria(Forma f);
void nau_destroi(Nau n);

int nau_id(Nau n);
/* Forma da nau (NULL se destruida). A nau nao e dona da forma. */
Forma nau_forma(Nau n);

double nau_energia(Nau n);
void nau_define_energia(Nau n, double e);
/* Subtrai energia (o nivel nunca fica negativo). */
void nau_gasta(Nau n, double e);
void nau_ganha(Nau n, double e);

double nau_riqueza(Nau n);
void nau_soma_riqueza(Nau n, double v);

/* Acumula as quantidades capturadas. */
void nau_conta(Nau n, int lagostas, int camaroes, int peixes, int moedas);
int nau_lagostas(Nau n);
int nau_camaroes(Nau n);
int nau_peixes(Nau n);
int nau_moedas(Nau n);

int nau_destruida(Nau n);
void nau_marca_destruida(Nau n);

/* Cor e largura do contorno conforme o nivel de energia:
   0 -> #484537 (2px) | (0,100) -> #FFCC00 (2px)
   [100,250) -> #217821 (2px) | >=250 -> #800066 (3px). */
const char *nau_cor_contorno(Nau n);
double nau_largura_contorno(Nau n);

#endif