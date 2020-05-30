#include "reserved_token.h"

#include <stdbool.h>
#include <string.h>

static ReservedToken reserved_tokens[] = {
    {"<=",      2,  OP_LEQ      },
    {">=",      2,  OP_GEQ      },
    {"==",      2,  OP_EQ       },
    {"!=",      2,  OP_NEQ      },
    {";",       1,  OP_TSTMT    },
    {"=",       1,  OP_ASSIGN   },
    {"<",       1,  OP_LT       },
    {">",       1,  OP_GT       },
    {"+",       1,  OP_ADD      },
    {"-",       1,  OP_SUB      },
    {"*",       1,  OP_MUL      },
    {"/",       1,  OP_DIV      },
    {"%",       1,  OP_MOD      },
    {"(",       1,  OP_BR_OPEN  },
    {")",       1,  OP_BR_CLOSE },
};

static bool str_shorter_than(int len, char *str) {
    for (int i = 0; i < len; i++) {
        if (str[i] == '\0') {
            return true;
        }
    }
    return false;
}

ReservedToken *find_reserved_op(OpName op) {
    // fprintf(stderr, "find_reserved_op\n");
    int n = sizeof(reserved_tokens) / sizeof(ReservedToken);
    for (int i = 0; i < n; i++) {
        ReservedToken *target = &reserved_tokens[i];
        if (target->op == op) {
            return target;
        }
    }
    return NULL;
}

ReservedToken *find_reserved_str(char* c) {
    // fprintf(stderr, "find_reserved_str\n");
    int n = sizeof(reserved_tokens) / sizeof(ReservedToken);
    for (int i = 0; i < n; i++) {
        ReservedToken *target = &reserved_tokens[i];
        if (str_shorter_than(target->len, c)) {
            continue;
        }
        if (strncmp(c, target->str, target->len) == 0) {
            return target;
        }
    }
    return NULL;
}
