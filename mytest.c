// main function for evaluation expression exercise

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "evalexp.h"

// tree construction functions

struct exp *mkvar(char *cp)
{
  struct exp *ep = malloc(sizeof(struct exp));
  ep->tag = isvar;
  strncpy(ep->var, cp, 8);
  return ep;
}

struct exp *mkconstant(int n)
{
  struct exp *ep = malloc(sizeof(struct exp));
  ep->tag = isconstant;
  ep->constant = n;
  return ep;
}

struct exp *mkopapp(enum op op, struct explist *el)
{
  struct exp *ep = malloc(sizeof(struct exp));
  ep->tag = isopapp;
  ep->op =  op;
  ep->exps =  el;
  return ep;
}

struct exp *mklet(char *cp, struct exp *e1, struct exp *e2)
{
  struct exp *ep = malloc(sizeof(struct exp));
  ep->tag = islet;
  strncpy(ep->bvar, cp, 8);
  ep->bexp = e1;
  ep->body = e2;
  return ep;
}

struct explist *cons(struct exp *e, struct explist *l)
{
  struct explist *l2 = malloc(sizeof(struct explist));
  l2->head = e;
  l2->tail = l;
  return l2;
}

// main for testing

int main(int argc, const char *argv[])
{
  struct exp *e1, *e2;
  struct explist *l;
    
  l = NULL;
  l = cons(mkconstant(5), l);
  l = cons(mkconstant(3), l);
  l = cons(mkconstant(2), l);
  e1 = mkopapp(isplus, l);
    
  l = NULL;
  l = cons(mkvar("x"), l);
  l = cons(mkvar("x"), l);
  l = cons(mkvar("x"), l);
  e2 = mkopapp(ismult, l);
    
  e1 = mklet("x", e1, e2);
    
  printf("%d\n", evalexp(e1)); // should print  1000

  // MY TEST
  struct explist *addlist, *multlist;
  struct exp *let1, *let2, *add1, *let3, *mult1;

  multlist = NULL;
  multlist = cons(mkvar("x"), multlist);
  multlist = cons(mkconstant(6), multlist);
  multlist = cons(mkvar("y"), multlist);
  mult1 = mkopapp(ismult, multlist);
  
  let3 = mklet("x", mkconstant(6), mult1);

  addlist = NULL;
  addlist = cons(mkvar("x"), addlist);
  addlist = cons(mkvar("y"), addlist);
  addlist = cons(let3, addlist);
  add1 = mkopapp(isplus, addlist);
  
  let2 = mklet("y", mkconstant(10), add1);
  let1 = mklet("x", mkconstant(5), let2);

  
  printf("%d\n", evalexp(let1));
  
  return 0;
}


