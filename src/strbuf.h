
#ifndef STRBUF_H
#define STRBUF_H

/*
 * Modulo strbuf: buffer de texto que cresce sob demanda (acumula trechos
 * de SVG, por exemplo). TAD opaco.
 */
typedef struct strbuf_s *StrBuf;

/* Cria buffer vazio. */
StrBuf strbuf_cria(void);

/* Libera o buffer. */
void strbuf_destroi(StrBuf s);

/* Acrescenta texto formatado (estilo printf) ao final do buffer. */
void strbuf_printf(StrBuf s, const char *fmt, ...);

/* Conteudo atual (string terminada em '\0', pertence ao buffer). */
const char *strbuf_conteudo(StrBuf s);

#endif
