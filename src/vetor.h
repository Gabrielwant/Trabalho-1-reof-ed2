
#ifndef VETOR_H
#define VETOR_H

/*
 * Modulo vetor: vetor dinamico de ponteiros (void *). TAD opaco.
 * O vetor nao e dono dos elementos apontados.
 */
typedef struct vetor_s *Vetor;

/* Cria vetor vazio. */
Vetor vetor_cria(void);

/* Libera o vetor (nao libera os elementos). */
void vetor_destroi(Vetor v);

/* Acrescenta o elemento p ao final. */
void vetor_adiciona(Vetor v, void *p);

/* Numero de elementos. */
int vetor_tamanho(Vetor v);

/* Elemento na posicao i (0 <= i < tamanho). */
void *vetor_obtem(Vetor v, int i);

/* Ordena o vetor. cmp recebe ponteiros para os elementos (como em qsort). */
void vetor_ordena(Vetor v, int (*cmp)(const void *, const void *));

#endif
