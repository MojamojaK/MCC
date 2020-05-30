#include <stdio.h>
#include <stdlib.h>

#include "asmgen.h"
#include "tokenizer.h"
#include "codegen.h"

static char buffer[256];

void generate_header() {
    head_out_1(".intel_syntax noprefix");
    head_out_1(".globl main");
}

void generate_stack_header(char* name) {
    head_out_1(format(buffer, "%s:", name));
    out_2("push", "rbp");
    out_3("mov", "rbp", "rsp");
    out_3("sub", "rsp", format(buffer, "%d", lvar_count() << 3));
}

void generate_stack_footer() {
    out_3("mov", "rsp", "rbp");
    out_2("pop", "rbp");
    out_1("ret");
}

static void gen_lval(Node* node) {
    if (node->kind != ND_LVAR) {
        fprintf(stderr, "LHS not variable\n");
        exit(1);
    }
    out_3("mov", "rax", "rbp");
    out_3("sub", "rax", format(buffer, "%d", node->offset));
    out_2("push", "rax");
}

static void gen(Node *node) {
    switch (node->kind) {
        case ND_NUM:
            out_2("push", format(buffer, "%d", node->val));
            return;
        case ND_LVAR: // Local Variable load
            gen_lval(node);
            out_2("pop", "rax");
            out_3("mov", "rax", "[rax]");
            out_2("push", "rax");
            return;
        case ND_ASSIGN: // Local Variable store
            gen_lval(node->lhs);
            gen(node->rhs);
            out_2("pop", "rdi");
            out_2("pop", "rax");
            out_3("mov", "[rax]", "rdi");
            out_2("push", "rdi");
            return;
    }

    gen(node->lhs);
    gen(node->rhs);
    out_2("pop", "rdi");
    out_2("pop", "rax");

    switch (node->kind) {
        case ND_ADD:
            out_3("add", "rax", "rdi");
            break;
        case ND_SUB:
            out_3("sub", "rax", "rdi");
            break;
        case ND_MUL:
            out_3("imul", "rax", "rdi");
            break;
        case ND_DIV:
            out_1("cqo");
            out_2("idiv", "rdi");
            break;
        case ND_MOD:
            out_1("cqo");
            out_2("idiv", "rdi");
            out_3("mov", "rax", "rdx");
            break;
        case ND_EQ:
            out_3("cmp", "rax", "rdi");
            out_2("sete", "al");
            out_3("movzb", "rax", "al");
            break;
        case ND_NEQ:
            out_3("cmp", "rax", "rdi");
            out_2("setne", "al");
            out_3("movzb", "rax", "al");
            break;
        case ND_LEQ:
            out_3("cmp", "rax", "rdi");
            out_2("setle", "al");
            out_3("movzb", "rax", "al");
            break;
        case ND_LT:
            out_3("cmp", "rax", "rdi");
            out_2("setl", "al");
            out_3("movzb", "rax", "al");
            break;
    }

    out_2("push", "rax");
}

void generate_stmt(Node* node) {
    gen(node);
    out_2("pop", "rax");
}
