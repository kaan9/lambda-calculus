#include <stdio.h>
#include <stdlib.h>


/*
 * A lambda expression is a variable (internally represented as an int,
 * mapped to strings provided externally), a function from a var to an exp,
 * or a function application e1 e2.
 * exp =
 *     | Var var [aka int]
 *     | Fun (int, exp)
 *     | App (exp, exp)
 */
typedef struct exp exp;
typedef struct val val;

struct exp {
	enum {
		VAR, FUN, APP
	} t;
	union {
		int var;
		struct {
			int var;
			exp *e;
		} f;
		struct {
			exp *e1, *e2;
		} a;
	} u;
};

/* a value is a (closed) function */
struct val {
	int var;
	exp *e;
};

/* duplicates an expression */
exp *dup_exp(exp *e)
{
	exp *e2 = malloc(sizeof(*e2));
	e2->t = e->t;

	switch (e->t) {
	case VAR:
		e2->u.var = e->u.var;
		break;
	case FUN:
		e2->u.f.var = e->u.f.var;
		e2->u.f.e = dup_exp(e->u.f.e);
		break;
	case APP:
		e2->u.a.e1 = dup_exp(e->u.a.e1);
		e2->u.a.e2 = dup_exp(e->u.a.e2);
		break;
	}
	
	return e2;
}

void free_exp(exp *e)
{
	switch (e->t) {
	case FUN:
		free_exp(e->u.f.e);
		break;
	case APP:
		free_exp(e->u.a.e1);
		free_exp(e->u.a.e2);
		break;
	case VAR:
		break;
	}
	free(e);
}


void print_exp_helper(exp *e)
{
	switch (e->t) {
	case VAR:
		printf("%d\n", e->u.var);
		break;
	case FUN:
		printf("%d => (", e->u.f.var);
		print_exp_helper(e->u.f.e);
		printf(")");
		break;
	case APP:
		printf("(");
		print_exp_helper(e->u.a.e1);
		printf(" ");
		print_exp_helper(e->u.a.e2);
		printf(")");
		break;
	}
}

void print_exp(exp *e)
{
	print_exp_helper(e);
	printf("\n");
}

/* creates a new expression from val, doesn't modify val */
exp *exp_of_val(val *v)
{
	exp *e;
	e = malloc(sizeof(*e));
	e->t = FUN;
	e->u.f.var = v->var;
	e->u.f.e = dup_exp(v->e);
	return e;
}


