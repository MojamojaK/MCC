#include <stdio.h>

#include "codegen.h"

void generate_header() {
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
}

void generate_footer() {
    printf("\tpop\trax\n");
    printf("\tret\n");
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
