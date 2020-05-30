#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"
#include "codegen.h"

void generate_header() {
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
}

void generate_stack_header(char* name) {
    printf("%s:\n", name);
    printf("\tpush\trbp\n");
    printf("\tmov\trbp,\trsp\n");
    printf("\tsub\trsp,\t%d\n", lvar_count() << 3);
}

void generate_stack_footer() {
    printf("\tmov\trsp, rbp\n");
    printf("\tpop\trbp\n");
    printf("\tret\n");
}

static void gen_lval(Node* node) {
    if (node->kind != ND_LVAR) {
        fprintf(stderr, "LHS not variable\n");
        exit(1);
    }
    printf("\tmov\trax,\trbp\n");
    printf("\tsub\trax,\t%d\n", node->offset);
    printf("\tpush\trax\n");
}

static void gen(Node *node) {
    switch (node->kind) {
        case ND_NUM:
            printf("\tpush\t%d\n", node->val);
            return;
        case ND_LVAR: // Local Variable load
            gen_lval(node);
            printf("\tpop\trax\n");
            printf("\tmov\trax,\t[rax]\n");
            printf("\tpush\trax\n");
            return;
        case ND_ASSIGN: // Local Variable store
            gen_lval(node->lhs);
            gen(node->rhs);
            printf("\tpop\trdi\n");
            printf("\tpop\trax\n");
            printf("\tmov\t[rax],\trdi\n");
            printf("\tpush\trdi\n");
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

void generate_stmt(Node* node) {
    gen(node);
    printf("\tpop\trax\n");
}
