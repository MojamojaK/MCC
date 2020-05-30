#include <stdlib.h>
#include <stdbool.h>

#include "opnames.h"
#include "tokenizer.h"
#include "nodegen.h"

int codes = 0;
Node **code;

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

Node *new_node_lvar(Token *token, const char* name, const int len) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;
    LVar *lvar = find_or_gen_lvar(token);
    node->offset = lvar->offset;
    return node;
}

void program() {
    while (!at_eof()) {
        codes += 1;
        code = (Node**)realloc((void*)code, sizeof(Node*) * codes);
        code[codes - 1] = stmt();
    }
}

Node *stmt() {
    Node *node = expr();
    expect(OP_TSTMT);
    return node;
}

Node *expr() {
    return assign();
}

Node *assign() {
    Node* node = equality();
    if (consume(OP_ASSIGN)) {
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}

Node *equality() {
    Node *node = relational();
    for (;;) {
        if (consume(OP_EQ)) {
            node = new_node(ND_EQ, node, relational());
        } else if (consume(OP_NEQ)) {
            node = new_node(ND_NEQ, node, relational());
        } else {
            return node;
        }    
    }
}

Node *relational() {
    Node *node = add();
    for (;;) {
        if (consume(OP_LEQ)) {
            node = new_node(ND_LEQ, node, add());
        } else if (consume(OP_GEQ)) {
            node = new_node(ND_LEQ, add(), node);
        } else if (consume(OP_LT)) {
            node = new_node(ND_LT, node, add());
        } else if (consume(OP_GT)) {
            node = new_node(ND_LT, add(), node);
        } else {
            return node;
        }
    }
}

Node *add() {
    Node *node = mul();
    for (;;) {
        if (consume(OP_ADD)) {
            node = new_node(ND_ADD, node, mul());
        } else if (consume(OP_SUB)) {
            node = new_node(ND_SUB, node, mul());
        } else {
            return node;
        }
    }
}

Node *mul() {
    Node *node = unary();
    for (;;) {
        if (consume(OP_MUL)) {
            node = new_node(ND_MUL, node, unary());
        } else if (consume(OP_DIV)) {
            node = new_node(ND_DIV, node, unary());
        } else if (consume(OP_MOD)) {
            node = new_node(ND_MOD, node, unary());
        } else {
            return node;
        }
    }
}

Node *unary() {
    if (consume(OP_ADD)) {
        return primary();
    } else if (consume(OP_SUB)) {
        return new_node(ND_SUB, new_node_num(0), primary());
    }
    return primary();
}

Node *primary() {
    if (consume(OP_BR_OPEN)) {
        Node *node = expr();
        expect(OP_BR_CLOSE);
        return node;
    }
    Token *ident = consume_ident();
    if (ident != NULL) {
        return new_node_lvar(ident, ident->str, ident->len);
    }
    return new_node_num(expect_number());
}
