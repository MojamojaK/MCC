#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "error.h"
#include "tokenizer.h"

static Token *token;

static LVar *locals_head = NULL;
int lvar_count() {
    int count = 0;
    for (LVar *var = locals_head; var != NULL; var = var->next) {
        ++count;
    }
    return count;
}

LVar *find_or_gen_lvar(Token *token) {
    for (LVar *var = locals_head; var != NULL; var = var->next) {
        if (var->len == token->len && memcmp(token->str, var->name, var->len) == 0) {
            return var;
        }
    }
    LVar *lvar = calloc(1, sizeof(LVar));
    lvar->name = token->str;
    lvar->len = token->len;
    lvar->next = locals_head;
    if (locals_head == NULL) 
    {
        lvar->offset = 8;
    } else {
        lvar->offset = locals_head->offset + 8;
    }
    locals_head = lvar;
    return lvar;
}

static bool token_not_reserved(OpName op) {
    return token->kind != TK_RESERVED || op != token->op;
}

Token *consume(OpName op) {
	if (token_not_reserved(op)) {
		return NULL;
	}
	Token *ret = token;
	token = token->next;
	return ret;
}

void expect(OpName op) {
	if (token_not_reserved(op)) {
        ReservedToken* rt = find_reserved_op(op);
        if (rt != NULL) {
            error_at(token->str, "Expected '%s'", rt->str);
        } else {
            error_at(token->str, "Expected Unknown");
        }
		
	}
	token = token->next;
}

static bool token_not_ident() {
    return token->kind != TK_IDENT;
}

Token *consume_ident() {
    if (token_not_ident()) {
        return NULL;
    }
    Token *ret = token;
    token = token->next;
    return ret;
}

Token *expect_number() {
	if (token->kind != TK_NUM) {
		error_at(token->str, "Not a number!");
	}
	Token *ret = token;
	token = token->next;
	return ret;
}

bool at_eof() {
	return token->kind == TK_EOF;
}

static int ident(char* c) {
	if (!isalpha(c[0])) {
		return 0;
	}
	int len = 1;
	for (;;) {
		char cl = c[len];
		if (cl == '_' || isalpha(cl) || isdigit(cl)) {
			len++;
		}
		else {
			return len;
		}
	}
}

static Token *new_token(const TokenKind kind, Token *cur, char *str) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	cur->next = tok;
	return tok;
}

Token *tokenize(char *p) {
	error_set_user_input(p);

	Token head;
	head.next = NULL;
	Token *cur = &head;

	while (*p) {
		if (isspace(*p)) {
			p++;
			continue;
		}
		
        ReservedToken *reserved = find_reserved_str(p);
		if (reserved != NULL) {
			cur = new_token(TK_RESERVED, cur, p);
            cur->len = reserved->len;
            cur->op = reserved->op;
            p += reserved->len;
			continue;
		}

		int ident_len = ident(p);
		if (ident_len > 0) {
			cur = new_token(TK_IDENT, cur, p);
            cur->len = ident_len;
            p += ident_len;
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

	return token;
}
