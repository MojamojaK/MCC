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

    Node *node = expr();

    generate_header();
    gen(node);	
	generate_footer();
    
	return 0;
}
