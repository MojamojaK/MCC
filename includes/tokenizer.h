#ifndef _TOKENIZER
#define _TOKENIZER

#include <stdbool.h>

typedef enum {
	TK_RESERVED,	// Symbols
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

Token *tokenize(char *p);

bool consume(char* op);
void expect(char* op);
int expect_number();
bool at_eof();

#endif
