// abstract syntax of Lisp style expressions
// as C trees


enum op { isplus, ismult };

enum exptag { islet, isconstant, isvar, isopapp };

struct exp {
  enum exptag tag;
  union {
    int constant;
    char var[8];
    struct {
      enum op op;
      struct explist *exps;
    } ; // anonymous as per C99
    struct {
      char bvar[8];
      struct exp *bexp;
      struct exp *body;
    }; // anonymous
  }; // anonymous
};

struct explist {
  struct exp *head;
  struct explist *tail;
};

int evalexp(struct exp *e); // implement me!



