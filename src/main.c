#include <stdio.h>

#include "tokenizer.h"
#include "nodegen.h"
#include "codegen.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Insufficient Parameters\n");
		return 1;
	}

	tokenize(argv[1]);
    // fprintf(stderr, "tokenize\n");

    program();
    // fprintf(stderr, "program\n");

    generate_header();
    generate_stack_header("main");

    for (int i = 0; i < codes; i++) {
        // fprintf(stderr, "generate_stmt start\n");
        generate_stmt(code[i]);
        // fprintf(stderr, "generate_stmt end\n");
    }

	generate_stack_footer();
    
	return 0;
}
