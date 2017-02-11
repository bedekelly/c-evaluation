#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "evalexp.h"

#define MAX_NAME_LENGTH 8

/**
 * An "environment" is essentially a linked-list of (name, value) pairs.
 * The trick is that we append to the *start* of the linked list, so
 * the most recently added (i.e. innermost) variables take precedence.
 */
struct env {
  char name[MAX_NAME_LENGTH];
  int value;
  struct env *next;
};


int evalexpenv(struct exp *exp, struct env *env);


/**
 * Add a variable to a given environment.
 * name: The name of the variable to assign.
 * namec: The length of the variable name.
 * value: The value to assign to this name.
 * env: A pointer to the environment to which to add this variable.
 */
struct env *
envAdd(char *name, size_t namec, int value, struct env *env) {
  struct env *result = malloc(sizeof(struct env));
  if (namec > MAX_NAME_LENGTH) {
    fprintf(stderr, "Variable name too long.");
    return NULL;
  }
  strncpy(result->name, name, namec);
  result->value = value;
  result->next = env;
  return result;
}


/**
 * Retrieve the value of a variable in an environment.
 * name: The name of the variable to retrieve.
 * env: The environment to search.
 */
int envGet(char *name, struct env *env) {
  struct env *var = env;
  while (var != NULL) {
    if (strcmp(name, var->name) == 0) {
      return var->value;
    }
    var = var->next;
  }
  fprintf(stderr, "Couldn't find variable %s in environment.", name);
  return -1;
}

/**
 * Create an environment suitable for adding variables with envAdd.
 */
struct env *envMake() {
  return NULL;
}


/**
 * Remove the last-added variable from an environment.
 */
struct env *envPop(struct env *env) {
  struct env *result = env->next;
  free(env);
  return result;
}


/**
 * Create an "environment" (i.e. a linked list of variables) and
 * delegate to the version of evalexp which takes an environment.
 */
int evalexp(struct exp *exp) {
  return evalexpenv(exp, envMake());
}


/**
 * Implement operator application. In this case there are only
 * two operators: plus and mult. For each of them, evaluate the
 * head of the parameter list, and either "plus" or "mult" it 
 * with a recursive call to apply the operator to the tail of
 * the parameter list.
 */
int opapp(enum op op, struct explist *exps, struct env *env) {
  switch(op) {
  case isplus: {
    if (exps == NULL) return 0;
    return evalexpenv(exps->head, env) + opapp(op, exps->tail, env);
  }
  case ismult: {
    if (exps == NULL) return 1;
    return evalexpenv(exps->head, env) * opapp(op, exps->tail, env);
  }
  }
}


/**
 * Evaluate an expression, given also a linked list of
 * variable names and their expression values. We switch on
 * the type of expression we've been given and act accordingly.
 */
int evalexpenv(struct exp *exp, struct env *env) {
  switch(exp->tag) {
  case islet: {
    int value = evalexpenv(exp->bexp, env);
    env = envAdd(exp->bvar, strlen(exp->bvar), value, env);
    int result = evalexpenv(exp->body, env);
    envPop(env);
    return result;
  }
  case isvar: {
    return envGet(exp->var, env);
  }
  case isconstant: {
    return exp->constant;
  }
  case isopapp:
    return opapp(exp->op, exp->exps, env);
  }
}
