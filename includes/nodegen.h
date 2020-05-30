#ifndef _NODEGEN
#define _NODEGEN

#include "tokenizer.h"

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
    ND_ASSIGN,
    ND_LVAR,
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    int offset;
    Token *token;
};

Node *new_node(const NodeKind kind, Node *lhs, Node *rhs, Token *token);
Node *new_node_virtual_num(int val, Token *token);
Node *new_node_num(Token *token);
Node *new_node_lvar(Token *token);

void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

extern int codes;
extern Node** code;

#endif
