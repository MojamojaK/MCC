#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer.h"

static Token *token;

static char* user_input;

static void error_at(char *loc, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, "");
	fprintf(stderr, "^ ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

static bool token_reserved(char* op) {
    return token->kind != TK_RESERVED || strlen(op) != token->len || memcmp((void*)token->str, (void*)op, token->len);
}

bool consume(char* op) {
	if (token_reserved(op)) {
		return false;
	}
	token = token->next;
	return true;
}

void expect(char* op) {
	if (token_reserved(op)) {
		error_at(token->str, "Not '%c'", op);
	}
	token = token->next;
}

int expect_number() {
	if (token->kind != TK_NUM) {
		error_at(token->str, "Not a number!");
	}
	int val = token->val;
	token = token->next;
	return val;
}

bool at_eof() {
	return token->kind == TK_EOF;
}

// Make sure to put lengthy strings in first
static char* reserved_characters[] = {
    "<=", ">=", "==", "!=", ">", "<", "+", "-", "*", "/", "%", "(", ")"
};

int reserved(char* c) {
    int n = sizeof(reserved_characters) / sizeof(char*);
    for (int i = 0; i < n; i++) {
        char* target = reserved_characters[i];
        int len = strlen(target);
        if (strncmp(c, target, len) == 0) {
            return len;
        }
    }
    return 0;
}

static Token *new_token(const TokenKind kind, Token *cur, char *str) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	cur->next = tok;
	return tok;
}

Token *tokenize(char *p) {
	user_input = p;

	Token head;
	head.next = NULL;
	Token *cur = &head;

	while (*p) {
		if (isspace(*p)) {
			p++;
			continue;
		}
		
        int len = reserved(p);
		if (len > 0) {
			cur = new_token(TK_RESERVED, cur, p);
            cur->len = len;
            p += len;
			continue;
		}

		if (isdigit(*p)) {
			cur = new_token(TK_NUM, cur, p);
			cur->val = strtol(p, &p, 10);
			continue;
		}

		error_at(token->str, "Unable to Tokenize");
	}

	new_token(TK_EOF, cur, p);
	token = head.next;
}