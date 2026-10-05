#ifndef TABELA_H
#define TABELA_H

/*
 * Modulo tabela: tabela de dispersao (hash) com chave inteira (identificador)
 * e valor ponteiro. TAD opaco. Usada como "memoria auxiliar" para achar uma
 * forma/nau a partir de seu identificador.
 */
typedef struct tabela_s *Tabela;

/* Cria tabela vazia. */
Tabela tabela_cria(void);

/* Libera a tabela. Se libera != NULL, e chamada para cada valor armazenado. */
void tabela_destroi(Tabela t, void (*libera)(void *));

/* Insere (id,v). Retorna 1 se inseriu, 0 se id ja existia (nada e alterado). */
int tabela_insere(Tabela t, int id, void *v);

/* Valor associado a id, ou NULL se nao existir. */
void *tabela_busca(Tabela t, int id);

/* Remove id e retorna o valor que estava associado (NULL se nao existia). */
void *tabela_remove(Tabela t, int id);

/* Numero de entradas. */
int tabela_tamanho(Tabela t);

/* Chama f(id, valor, ctx) para cada entrada (ordem indefinida).
   f nao deve inserir/remover entradas. */
void tabela_percorre(Tabela t, void (*f)(int, void *, void *), void *ctx);

#endif