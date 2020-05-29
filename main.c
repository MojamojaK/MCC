#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Token *token;

char* user_input;

void error_at(char *loc, char *fmt, ...) {
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

bool token_reserved(char* op) {
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
char* reserved_characters[] = {
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

Token *new_token(const TokenKind kind, Token *cur, char *str) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	cur->next = tok;
	return tok;
}

Token *tokenize(char *p) {
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
	return head.next;
}

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_MOD,
    ND_NUM,
    ND_EQ,
    ND_NEQ,
    ND_LT,
    ND_LEQ,
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
};

Node *new_node(const NodeKind kind, Node *lhs, Node *rhs) {
    Node* node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(const int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

Node *expr() {
    // fprintf(stderr, "expr\n");
    return equality();
}

Node *equality() {
    // fprintf(stderr, "equality\n");
    Node *node = relational();
    for (;;) {
        if (consume("==")) {
            node = new_node(ND_EQ, node, relational());
        } else if (consume("!=")) {
            node = new_node(ND_NEQ, node, relational());
        } else {
            return node;
        }    
    }
}

Node *relational() {
    Node *node = add();
    for (;;) {
        if (consume("<=")) {
            node = new_node(ND_LEQ, node, add());
        } else if (consume(">=")) {
            node = new_node(ND_LEQ, add(), node);
        } else if (consume("<")) {
            node = new_node(ND_LT, node, add());
        } else if (consume(">")) {
            node = new_node(ND_LT, add(), node);
        } else {
            return node;
        }
    }
}

Node *add() {
    Node *node = mul();
    for (;;) {
        if (consume("+")) {
            node = new_node(ND_ADD, node, mul());
        } else if (consume("-")) {
            node = new_node(ND_SUB, node, mul());
        } else {
            return node;
        }
    }
}

Node *mul() {
    // fprintf(stderr, "mul\n");
    Node *node = unary();
    for (;;) {
        if (consume("*")) {
            node = new_node(ND_MUL, node, unary());
        } else if (consume("/")) {
            node = new_node(ND_DIV, node, unary());
        } else if (consume("%")) {
            node = new_node(ND_MOD, node, unary());
        } else {
            return node;
        }
    }
}

Node *unary() {
    // fprintf(stderr, "unary\n");
    if (consume("+")) {
        return primary();
    } else if (consume("-")) {
        return new_node(ND_SUB, new_node_num(0), primary());
    }
    return primary();
}

Node *primary() {
    // fprintf(stderr, "primary\n");
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }
    return new_node_num(expect_number());
}

void gen(Node *node) {
    if (node->kind == ND_NUM) {
        printf("\tpush\t%d\n", node->val);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);
    printf("\tpop\trdi\n");
    printf("\tpop\trax\n");

    switch (node->kind) {
        case ND_ADD:
            printf("\tadd\trax,\trdi\n");
            break;
        case ND_SUB:
            printf("\tsub\trax,\trdi\n");
            break;
        case ND_MUL:
            printf("\timul\trax,\trdi\n");
            break;
        case ND_DIV:
            printf("\tcqo\n");
            printf("\tidiv\trdi\n");
            break;
        case ND_MOD:
            printf("\tcqo\n");
            printf("\tidiv\trdi\n");
            printf("\tmov\trax,\trdx\n");
            break;
        case ND_EQ:
            printf("\tcmp\trax,\trdi\n");
            printf("\tsete\tal\n");
            printf("\tmovzb\trax,\tal\n");
            break;
        case ND_NEQ:
            printf("\tcmp\trax,\trdi\n");
            printf("\tsetne\tal\n");
            printf("\tmovzb\trax,\tal\n");
            break;
        case ND_LEQ:
            printf("\tcmp\trax,\trdi\n");
            printf("\tsetle\tal\n");
            printf("\tmovzb\trax,\tal\n");
            break;
        case ND_LT:
            printf("\tcmp\trax,\trdi\n");
            printf("\tsetl\tal\n");
            printf("\tmovzb\trax,\tal\n");
            break;
    }

    printf ("\tpush\trax\n");
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Insufficient Parameters\n");
		return 1;
	}

	user_input = argv[1];

	token = tokenize(argv[1]);
    // fprintf(stderr, "Tokenized\n");

    Node *node = expr();
    // fprintf(stderr, "Node constructed\n");

	printf(".intel_syntax noprefix\n");
	printf(".globl main\n");
	printf("main:\n");
	
    gen(node);	
	
    printf("\tpop\trax\n");
	printf("\tret\n");
	return 0;
}
