#ifndef _RESERVED_TOKEN
#define _RESERVED_TOKEN

#include "opnames.h"

typedef struct ReservedToken {
    char* str;
    int len;
    OpName op;
} ReservedToken;

ReservedToken *find_reserved_op(OpName op);
ReservedToken *find_reserved_str(char* c);

#endif
