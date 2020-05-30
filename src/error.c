#include <stdio.h>
#include <stdlib.h>

#include "error.h"

static char* user_input = NULL;

void error_set_user_input(char* str) {
	user_input = str;
}

void error_at(char *loc, char *fmt, ...) {
    // fprintf(stderr, "error_at\n");
	va_list ap;
	va_start(ap, fmt);
	if (user_input != NULL) {
		int pos = loc - user_input;
		fprintf(stderr, "%s\n", user_input);
		fprintf(stderr, "%*s", pos, "");
		fprintf(stderr, "^ ");
	} else {
		fprintf(stderr, "Error: ");
	}
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}
