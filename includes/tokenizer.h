#ifndef _TOKENIZER
#define _TOKENIZER

#include <stdbool.h>
#include <stdarg.h>

typedef enum {
	TK_RESERVED,	// Symbols
	TK_IDENT,		// Variable Indentifiers
	TK_NUM,		// Numbers
	TK_EOF,		// End of file
} TokenKind;

typedef struct Token Token;

struct Token {
	TokenKind kind;
	Token *next;
	int val;
	char *str;
    int len;
};

void error_at(char *loc, char *fmt, ...);

Token *tokenize(char *p);

bool consume(char* op);
Token *consume_ident();
void expect(char* op);
int expect_number();
bool at_eof();

#endif
