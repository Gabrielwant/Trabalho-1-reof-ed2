
#ifndef NAU_H
#define NAU_H
#include "forma.h"

typedef struct nau_s *Nau;

Nau nau_cria(Forma f);
void nau_destroi(Nau n);

int nau_id(Nau n);

Forma nau_forma(Nau n);

double nau_energia(Nau n);
void nau_define_energia(Nau n, double e);

void nau_gasta(Nau n, double e);
void nau_ganha(Nau n, double e);

double nau_riqueza(Nau n);
void nau_soma_riqueza(Nau n, double v);

void nau_conta(Nau n, int lagostas, int camaroes, int peixes, int moedas);
int nau_lagostas(Nau n);
int nau_camaroes(Nau n);
int nau_peixes(Nau n);
int nau_moedas(Nau n);

int nau_destruida(Nau n);
void nau_marca_destruida(Nau n);

const char *nau_cor_contorno(Nau n);
double nau_largura_contorno(Nau n);

#endif
