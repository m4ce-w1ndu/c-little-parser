#ifndef PARSER_H
#define PARSER_H

#include <vector.h>

#define TOK_LEN 256

typedef enum TokenType {
	 OPEN_PAREN,
	 CLOSE_PAREN,
	 NUMBER,
	 OP_PLUS,
	 OP_MINUS,
	 OP_STAR,
	 OP_DIV,
	 OP_EXP,
	 IGN_SPACE,
	 TBD,
	 EMPTY
} TokenType;

typedef struct Token {
	TokenType type;
	char value[TOK_LEN];
} Token;

typedef struct StackNode* Stack;

/* Stack manipulation functions */
struct StackNode* make_node(Token data, struct StackNode* next);
void stack_push(Stack* s, Token t);
void stack_pop(Stack* s, Token* ret);
int stack_empty(Stack* s);

/* Parser functions */
vector read(const char* text);
double calculate(vector tokens);

double token_string_to_value(const char* value);
const char* token_value_to_string(double value);

#endif
