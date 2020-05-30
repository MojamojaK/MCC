#ifndef _ASMGEN
#define _ASMGEN

#include <stdarg.h>

char* format(char* buff, char *fmt, ...);

void head_out_1(char *a1);
void out_1(char *a1);
void out_2(char *a1, char *a2);
void out_3(char *a1, char *a2, char *a3);

#endif