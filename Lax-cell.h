/* Lax-cell.h */

#ifndef _Lax_CELL_H
#define _Lax_CELL_H

#include "Lax.h"

#ifdef __cplusplus
extern "C" {
#endif

enum Lax_port_kind {
  port_free=0,
  port_file=1,
  port_string=2,
  port_srfi6=4,
  port_input=16,
  port_output=32,
  port_saw_EOF=64
};

typedef struct port {
  unsigned char kind;
  union {
    struct {
      FILE *file;
      int closeit;
#if SHOW_ERROR_LINE
      int curr_line;
      char *filename;
#endif
    } stdio;
    struct {
      char *start;
      char *past_the_end;
      char *curr;
    } string;
  } rep;
} port;

struct cell {
  unsigned int _flag;
  union {
    struct {
      char   *_svalue;
      int   _length;
    } _string;
    num _number;
    port *_port;
    foreign_func _ff;
    struct {
      struct cell *_car;
      struct cell *_cdr;
    } _cons;
  } _object;
};

struct Lax {
func_alloc malloc;
func_dealloc free;

int retcode;
int tracing;


#ifndef CELL_SEGSIZE
#define CELL_SEGSIZE    5000  
#endif
#ifndef CELL_NSEGMENT
#define CELL_NSEGMENT   500    
#endif
char *alloc_seg[CELL_NSEGMENT];
pointer cell_seg[CELL_NSEGMENT];
int     last_cell_seg;

pointer args;
pointer envir;
pointer code;
pointer dump;

int interactive_repl;

struct cell _sink;
pointer sink;
struct cell _NIL;
pointer NIL;
struct cell _HASHT;
pointer T;
struct cell _HASHF;
pointer F;
struct cell _EOF_OBJ;
pointer EOF_OBJ;
pointer oblist;
pointer global_env;
pointer c_nest;

pointer LAMBDA;
pointer QUOTE;

pointer QQUOTE;
pointer UNQUOTE;
pointer UNQUOTESP;
pointer FEED_TO;
pointer COLON_HOOK;
pointer ERROR_HOOK;
pointer SHARP_HOOK;
pointer COMPILE_HOOK;

pointer free_cell;
long    fcells;

pointer inport;
pointer outport;
pointer save_inport;
pointer loadport;

#ifndef MAXFIL
#define MAXFIL 64
#endif
port load_stack[MAXFIL];
int nesting_stack[MAXFIL];
int file_i;
int nesting;

char    gc_verbose;
char    no_memory;

#ifndef LINESIZE
#define LINESIZE 1024
#endif
char    linebuff[LINESIZE];
#ifndef STRBUFFSIZE
#define STRBUFFSIZE 256
#endif
char    strbuff[STRBUFFSIZE];

FILE *tmpfp;
int tok;
int print_flag;
pointer value;
int op;

void *ext_data;
long gensym_cnt;
int paren_error_printed;

struct Lax_interface *vptr;
void *dump_base;
int dump_size;
};

enum Lax_opcodes {
#define _OP_DEF(A,B,C,D,E,OP) OP,
#include "com.h"
  OP_MAXDEFINED
};


#define cons(sc,a,b) _cons(sc,a,b,0)
#define immutable_cons(sc,a,b) _cons(sc,a,b,1)

int is_string(pointer p);
char *string_value(pointer p);
int is_number(pointer p);
num nvalue(pointer p);
long ivalue(pointer p);
double rvalue(pointer p);
int is_integer(pointer p);
int is_real(pointer p);
int is_character(pointer p);
long charvalue(pointer p);
int is_vector(pointer p);

int is_port(pointer p);

int is_pair(pointer p);
pointer pair_car(pointer p);
pointer pair_cdr(pointer p);
pointer set_car(pointer p, pointer q);
pointer set_cdr(pointer p, pointer q);

int is_symbol(pointer p);
char *symname(pointer p);
int hasprop(pointer p);

int is_syntax(pointer p);
int is_proc(pointer p);
int is_foreign(pointer p);
char *syntaxname(pointer p);
int is_closure(pointer p);
#ifdef USE_MACRO
int is_macro(pointer p);
#endif
pointer closure_code(pointer p);
pointer closure_env(pointer p);

int is_continuation(pointer p);
int is_promise(pointer p);
int is_environment(pointer p);
int is_immutable(pointer p);
void setimmutable(pointer p);

#ifdef __cplusplus
}
#endif

#endif
