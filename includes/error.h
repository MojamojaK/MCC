#ifndef _ERROR
#define _ERROR

#include <stdarg.h>

void error_set_user_input(char* str);
void error_at(char *loc, char *fmt, ...);


#endif
