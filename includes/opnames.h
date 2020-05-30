#ifndef _OPNAMES
#define _OPNAMES

typedef enum {
	OP_NULL,		// NOT OP
	OP_LEQ,     	// <=
	OP_GEQ,     	// >=
	OP_EQ,      	// ==
	OP_NEQ,     	// !=
	OP_TSTMT,   	// ;
	OP_ASSIGN,		// =
	OP_LT,      	// <
	OP_GT,      	// >
	OP_ADD,     	// +
	OP_SUB,     	// -
	OP_MUL,     	// *
	OP_DIV,     	// /
	OP_MOD,     	// %
	OP_BR_OPEN, 	// (
	OP_BR_CLOSE,	// )
} OpName;

#endif
