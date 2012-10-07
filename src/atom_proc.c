/*
 * atom_proc.c
 *
 * Operators on objects of type Atom
 *
 * Copyright (C) 2012-10-05 liutos
 */
#include "object.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SymbolTable symbol_table = NULL;
Boolean lt_true;		/* The inner representation of logical true in defining language. */
Boolean lt_false;		/* The inner representation of logical false in defining language. */
LispObject lt_void;		/* The inner representation of void in defining language. */

Symbol lt_quote;
Symbol lt_if;
Symbol lt_begin;
Symbol lt_lambda;

Symbol make_symbol(char *symbol_name)
{
    Symbol sym;

    sym = new_object();
    sym->type = SYMBOL;
    sym->symbol_name = symbol_name;

    return sym;
}

Symbol get_symbol(char *symbol_name)
{
    SymbolTable tmp;

    tmp = symbol_table->next;	/* Skip the head node */
    while (tmp != NULL) {
	if (0 == strcmp(tmp->symbol_name, symbol_name))
	    return tmp->symbol;
	tmp = tmp->next;
    }

    return NULL;
}

void put_symbol(Symbol symbol)
{
    struct StrSymMap *node;

    node = malloc(sizeof(struct StrSymMap));
    node->symbol_name = symbol->symbol_name;
    node->symbol = symbol;
    /* Insert the new node at the first position */
    node->next = symbol_table->next;
    symbol_table->next = node;
}

Symbol ensure_symbol_exists(char *symbol_name)
{
    Symbol symbol;

    symbol = get_symbol(symbol_name);
    if (symbol != NULL)
	return symbol;
    else {
	symbol = make_symbol(symbol_name);
	put_symbol(symbol);

	return symbol;
    }
}

void init_symbol_table(void)
{
    symbol_table = malloc(sizeof(struct StrSymMap));
    symbol_table->next = NULL;
    lt_quote = ensure_symbol_exists("quote");
    lt_if = ensure_symbol_exists("if");
    lt_begin = ensure_symbol_exists("begin");
    lt_lambda = ensure_symbol_exists("lambda");
}

Function make_c_fun_object(primitive_t prim)
{
    Function func;

    func = new_object();
    TYPE(func) = FUNCTION;
    FUNC_FLAG(func) = TRUE;
    PRIMITIVE(func) = prim;

    return func;
}

BOOL is_true_obj(LispObject obj)
{
    return lt_false != obj;	/* Everything is true except the object lt_false */
}

Function make_i_fun_object(Cons parms, LispObject expr, Environment cenv)
/* The parameter 'cenv' points to the environment at the creation time. */
{
    Function fun;

    fun = new_object();
    TYPE(fun) = FUNCTION;
    FUNC_FLAG(fun) = FALSE;
    PARAMETERS(fun) = parms;
    EXPRESSION(fun) = expr;
    LOCAL_ENV(fun) = cenv;	/* This environment could be modified after
				   the creation of the closure */

    return fun;
}