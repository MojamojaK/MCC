#include <stdlib.h>
#include <stdbool.h>

#include "opnames.h"
#include "nodegen.h"

int codes = 0;
Node **code;

Node *new_node(const NodeKind kind, Node *lhs, Node *rhs, Token *token) {
    Node* node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    node->token = token;
    return node;
}

Node *new_node_virtual_num(int val, Token *token) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    node->token = token;
    return node;
}

Node *new_node_num(Token *token) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = token->val;
    node->token = token;
    return node;
}

Node *new_node_lvar(Token *token) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;
    LVar *lvar = find_or_gen_lvar(token);
    node->offset = lvar->offset;
    node->token = token;
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
    Token *token = consume(OP_ASSIGN);
    if (token != NULL) {
        node = new_node(ND_ASSIGN, node, assign(), token);
    }
    return node;
}

Node *equality() {
    Node *node = relational();
    for (;;) {
        Token *eq_token = consume(OP_EQ);
        if (eq_token != NULL) {
            node = new_node(ND_EQ, node, relational(), eq_token);
            continue;
        }
        Token *neq_token = consume(OP_NEQ);
        if (neq_token != NULL) {
            node = new_node(ND_NEQ, node, relational(), neq_token);
            continue;
        }
        return node;
    }
}

Node *relational() {
    Node *node = add();
    for (;;) {
        Token *leq_token = consume(OP_LEQ);
        if (leq_token != NULL) {
            node = new_node(ND_LEQ, node, add(), leq_token);
            continue;
        }
        Token *geq_token = consume(OP_GEQ);
        if (geq_token != NULL) {
            node = new_node(ND_LEQ, add(), node, geq_token);
            continue;
        }
        Token *lt_token = consume(OP_LT);
        if (lt_token != NULL) {
            node = new_node(ND_LT, node, add(), lt_token);
            continue;
        }
        Token *gt_token = consume(OP_GT);
        if (gt_token != NULL) {
            node = new_node(ND_LT, add(), node, gt_token);
            continue;
        }
        return node;
    }
}

Node *add() {
    Node *node = mul();
    for (;;) {
        Token *add_token = consume(OP_ADD);
        if (add_token != NULL) {
            node = new_node(ND_ADD, node, mul(), add_token);
            continue;
        }
        Token *sub_token = consume(OP_SUB);
        if (sub_token != NULL) {
            node = new_node(ND_SUB, node, mul(), sub_token);
            continue;
        }
        return node;
    }
}

Node *mul() {
    Node *node = unary();
    for (;;) {
        Token *mul_token = consume(OP_MUL);
        if (mul_token != NULL) {
            node = new_node(ND_MUL, node, unary(), mul_token);
            continue;
        }

        Token *div_token = consume(OP_DIV);
        if (div_token != NULL) {
            node = new_node(ND_DIV, node, unary(), div_token);
            continue;
        }

        Token *mod_token = consume(OP_MOD);
        if (mod_token != NULL) {
            node = new_node(ND_MOD, node, unary(), mod_token);
            continue;
        }
        return node;
    }
}

Node *unary() {
    Token *pos_token = consume(OP_ADD);
    if (pos_token != NULL) {
        return primary();
    }
    Token *neg_token = consume(OP_SUB);
    if (neg_token != NULL) {
        return new_node(ND_SUB, new_node_virtual_num(0, neg_token), primary(), neg_token);
    }
    return primary();
}

Node *primary() {
    if (consume(OP_BR_OPEN) != NULL) {
        Node *node = expr();
        expect(OP_BR_CLOSE);
        return node;
    }
    Token *ident = consume_ident();
    if (ident != NULL) {
        return new_node_lvar(ident);
    }
    Token* number_token = expect_number();
    return new_node_num(number_token);
}
