#ifndef _TOKENIZER
#define _TOKENIZER

#include <stdbool.h>
#include <stdarg.h>

#include "reserved_token.h"

typedef enum {
	TK_RESERVED,    // Symbols
	TK_IDENT,       // Variable Indentifiers
	TK_NUM,     // Numbers
	TK_EOF,     // End of file
} TokenKind;

typedef struct Token Token;

struct Token {
	TokenKind kind;
	Token *next;
	OpName op;
	int val;
	char *str;
	int len;
};

typedef struct LVar LVar;

struct LVar {
	LVar *next;
	char *name;
	int len;
	int offset;
};

LVar *find_or_gen_lvar(Token *token);
int lvar_count();

void error_at(char *loc, char *fmt, ...);

Token *tokenize(char *p);

bool consume(OpName op);
Token *consume_ident();
void expect(OpName op);
int expect_number();
bool at_eof();

#endif
