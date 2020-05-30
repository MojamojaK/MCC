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
	OpName op;	// For kind == TK_RESERVED
	int val;	// For kind == TK_NUM
	int len;	// For kind == TK_IDENT
	char *str;
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

Token *tokenize(char *p);

Token *consume(OpName op);
Token *consume_ident();
void expect(OpName op);
Token *expect_number();
bool at_eof();

#endif
