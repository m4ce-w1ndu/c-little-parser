#include <Parser.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

struct StackNode {
	Token data;
	struct StackNode* next;
};

struct StackNode* make_node(Token data, struct StackNode* next)
{
	Stack s = (struct StackNode*) malloc(sizeof(struct StackNode));
	s->data = data;
	s->next = next;
	return s;
}

void stack_push(Stack* s, Token t)
{
	*s = make_node(t, *s);
}

void stack_pop(Stack* s, Token* ret)
{
	if (!stack_empty(s)) {
		ret->type = (*s)->data.type;
		strncpy(ret->value, (*s)->data.value, 256);
		Stack del = *s;
		(*s) = (*s)->next;
		free(del);
	} else {
        ret->type = EMPTY;
        memset(ret->value, 0, TOK_LEN);
    }
}

int stack_empty(Stack* s)
{
	return (NULL == s || NULL == *s) ? 1 : 0;
}

Token next_token(const char* text)
{
	Token t;
	if (NULL == text) {
		t.type = EMPTY;
		return t;
	}

	char c = text[0];
	switch (c) {
		case '+':
			memmove((void*)text, (void*)text + 1, strlen(text));
			t.type = OP_PLUS;
			strcpy(t.value, "+");
			return t;
		case '-':
			memmove((void*)text, (void*)text + 1, strlen(text));
			t.type = OP_MINUS;
			strcpy(t.value, "-");
			return t;
		case '*':
			memmove((void*)text, (void*)text + 1, strlen(text));
			t.type = OP_STAR;
			strcpy(t.value, "*");
			return t;
		case '/':
			memmove((void*)text, (void*)text + 1, strlen(text));
			t.type = OP_DIV;
			strcpy(t.value, "/");
			return t;
		case '^':
			memmove((void*)text, (void*)text + 1, strlen(text));
			t.type = OP_EXP;
			strcpy(t.value, "^");
			return t;
		case '(':
			memmove((void*)text, (void*)text + 1, strlen(text));
			t.type = OPEN_PAREN;
			strcpy(t.value, "(");
			return t;
		case ')':
			memmove((void*)text, (void*)text + 1, strlen(text));
			t.type = CLOSE_PAREN;
			strcpy(t.value, ")");
			return t;
		default: {
			if (isspace(c)) {
				memmove((void*)text, (void*)text + 1, strlen(text));
				t.type = IGN_SPACE;
				return t;
			}

			/* Number case */
			if (isdigit(c)) {
				char value[TOK_LEN];
                memset(value, 0, TOK_LEN);
				int seen = 0;

				int value_pos = 0;
				while (isdigit(c) || c == '.') {
					c = text[0];
					if (!isdigit(c) && c != '.') break;

					if (c == '.' && seen) {
						memmove((void*)text, (void*)text + 1, strlen(text));
						continue;
					}

					if (c == '.') seen = 1;

					value[value_pos] = c;
					++value_pos;
					memmove((void*)text, (void*)text + 1, strlen(text));
				}
				t.type = NUMBER;
				strncpy(t.value, value, TOK_LEN);
				return t;
			}
			t.type = EMPTY;
			return t;
		}
	}
}

vector read(const char* text)
{
	vector res = vector_new(struct Token, 5);
	char* str = (char*)text;

	for (Token tok = next_token(str); tok.type != EMPTY; 
		 tok = next_token(str)) {
		if (strlen(tok.value) == 0) {
			fprintf(stderr, "Unknown token\n");
		}

		if (tok.type != IGN_SPACE)
			vector_push_back(struct Token, res, tok);
	}

	return res;
}

double calculate(vector tokens)
{
	Token last_five[5];
	Stack s;

	double acc = 0.0;

	while (!vector_empty(tokens)) {
		Token tok;
		memcpy(&tok, vector_at_p(struct Token, tokens, 0), sizeof(struct Token));
		stack_push(&s, tok);
		vector_erase(struct Token, tokens, 0);

		if (tok.type != CLOSE_PAREN) continue;

		int i;
		for (i = 4; i >= 0; --i) {
            stack_pop(&s, &tok);
            memcpy(last_five + i, &tok, sizeof(struct Token));
        }

		/* Syntax check */
		if (last_five[0].type != OPEN_PAREN) break;
        if (last_five[1].type != NUMBER) break;
        if (last_five[2].type != OP_PLUS &&
            last_five[2].type != OP_MINUS &&
            last_five[2].type != OP_STAR &&
            last_five[2].type != OP_DIV &&
            last_five[2].type != OP_EXP) break;
        if (last_five[3].type != NUMBER) break;
        if (last_five[4].type != CLOSE_PAREN) break;

        /* Value conversion */
        double a = token_string_to_value(last_five[1].value);
        double b = token_string_to_value(last_five[3].value);

        switch (last_five[2].type) {
            case OP_PLUS: acc = a + b; break;
            case OP_MINUS: acc = a - b; break;
            case OP_STAR: acc = a * b; break;
            case OP_DIV: acc = a / b;
            case OP_EXP: acc = pow(a, b); break;
            default: fprintf(stderr, "Syntax error\n"); exit(EXIT_FAILURE);
        }

        const char* tmp = token_value_to_string(acc);
        strcpy(tok.value, tmp);
        free((void*)tmp);
        tok.type = NUMBER;
        stack_push(&s, tok);
	}

	if (!vector_empty(tokens)) {
	    fprintf(stderr, "Syntax error\n");
	    exit(EXIT_FAILURE);
	}

	Token tok;
	stack_pop(&s, &tok);
	if (!stack_empty(&s) || tok.type != NUMBER) {
	    fprintf(stderr, "Syntax error\n");
	    exit(EXIT_FAILURE);
	}

	return token_string_to_value(tok.value);
}

double token_string_to_value(const char* value)
{
    return strtod(value, NULL);
}

const char* token_value_to_string(double value)
{
    char* tmp = (char*) malloc(32);
    snprintf(tmp, 32, "%lf", value);
    return tmp;
}
