#include <stdio.h>

#include "asmgen.h"

char* format(char* buff, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff, fmt, ap);
	va_end (ap);
	return buff;
}

void head_out_1(char *a1) {
	printf("%s\n", a1);
}

void out_1(char *a1) {
	printf("\t%s\n", a1);
}

void out_2(char *a1, char *a2) {
	printf("\t%s\t%s\n", a1, a2);
}

void out_3(char *a1, char *a2, char *a3) {
	printf("\t%s\t%s,\t%s\n", a1, a2, a3);
}
