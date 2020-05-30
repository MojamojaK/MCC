#include <stdlib.h>
#include <stdbool.h>

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

Node *new_node_lvar(const char* name, const int len) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;
    node->offset = (name[0] - 'a' + 1) << 3;
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
    expect(";");
    return node;
}

Node *expr() {
    return assign();
}

Node *assign() {
    Node* node = equality();
    if (consume("=")) {
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}

Node *equality() {
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
    if (consume("+")) {
        return primary();
    } else if (consume("-")) {
        return new_node(ND_SUB, new_node_num(0), primary());
    }
    return primary();
}

Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }
    Token *ident = consume_ident();
    if (ident != NULL) {
        return new_node_lvar(ident->str, ident->len);
    }
    return new_node_num(expect_number());
}
