#ifndef _CODEGEN
#define _CODEGEN

#include "nodegen.h"

void generate_header();

void generate_stack_header(char* name);
void generate_stack_footer();

void generate_stmt(Node* node);

#endif
