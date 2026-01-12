/* Lax.h */

#ifndef _Lax_H
#define _Lax_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STANDALONE
# define STANDALONE 1
#endif

#ifndef _MSC_VER
# define USE_STRCASECMP 1
# ifndef USE_STRLWR
#   define USE_STRLWR 1
# endif
# define Lax_EXPORT
#else
# define USE_STRCASECMP 0
# define USE_STRLWR 0
# ifdef _Lax_SOURCE
#  define Lax_EXPORT __declspec(dllexport)
# else
#  define Lax_EXPORT __declspec(dllimport)
# endif
#endif

#if USE_NO_FEATURES
# define USE_MATH 0
# define USE_CHAR_CLASSIFIERS 0
# define USE_ASCII_NAMES 0
# define USE_STRING_PORTS 0
# define USE_ERROR_HOOK 0
# define USE_TRACING 0
# define USE_COLON_HOOK 0
# define USE_PLIST 0
#endif

#define USE_Lax_STACK


#ifndef USE_MATH
# define USE_MATH 1
#endif

#ifndef USE_CHAR_CLASSIFIERS
# define USE_CHAR_CLASSIFIERS 1
#endif

#ifndef USE_ASCII_NAMES
# define USE_ASCII_NAMES 1
#endif

#ifndef USE_STRING_PORTS
# define USE_STRING_PORTS 1
#endif

#ifndef USE_TRACING
# define USE_TRACING 1
#endif

#ifndef USE_PLIST
# define USE_PLIST 0
#endif

#ifndef USE_ERROR_HOOK
# define USE_ERROR_HOOK 1
#endif

#ifndef USE_COLON_HOOK
# define USE_COLON_HOOK 1
#endif

#ifndef USE_STRCASECMP
# define USE_STRCASECMP 0
#endif

#ifndef USE_STRLWR
# define USE_STRLWR 1
#endif

#ifndef STDIO_ADDS_CR
# define STDIO_ADDS_CR 0
#endif

#ifndef INLINE
# define INLINE
#endif

#ifndef USE_INTERFACE
# define USE_INTERFACE 0
#endif

#ifndef SHOW_ERROR_LINE
# define SHOW_ERROR_LINE 1
#endif

typedef struct Lax Lax;
typedef struct cell *pointer;

typedef void * (*func_alloc)(size_t);
typedef void (*func_dealloc)(void *);

typedef struct num {
     char is_fixnum;
     union {
          long ivalue;
          double rvalue;
     } value;
} num;

Lax_EXPORT Lax *Lax_init_new(void);
Lax_EXPORT Lax *Lax_init_new_custom_alloc(func_alloc malloc, func_dealloc free);
Lax_EXPORT int Lax_init(Lax *sc);
Lax_EXPORT int Lax_init_custom_alloc(Lax *sc, func_alloc, func_dealloc);
Lax_EXPORT void Lax_deinit(Lax *sc);
void Lax_set_input_port_file(Lax *sc, FILE *fin);
void Lax_set_input_port_string(Lax *sc, char *start, char *past_the_end);
Lax_EXPORT void Lax_set_output_port_file(Lax *sc, FILE *fin);
void Lax_set_output_port_string(Lax *sc, char *start, char *past_the_end);
Lax_EXPORT void Lax_load_file(Lax *sc, FILE *fin);
Lax_EXPORT void Lax_load_named_file(Lax *sc, FILE *fin, const char *filename);
Lax_EXPORT void Lax_load_string(Lax *sc, const char *cmd);
Lax_EXPORT pointer Lax_apply0(Lax *sc, const char *procname);
Lax_EXPORT pointer Lax_call(Lax *sc, pointer func, pointer args);
Lax_EXPORT pointer Lax_eval(Lax *sc, pointer obj);
void Lax_set_external_data(Lax *sc, void *p);
Lax_EXPORT void Lax_define(Lax *sc, pointer env, pointer symbol, pointer value);

typedef pointer (*foreign_func)(Lax *, pointer);

pointer _cons(Lax *sc, pointer a, pointer b, int immutable);
pointer mk_integer(Lax *sc, long num);
pointer mk_real(Lax *sc, double num);
pointer mk_symbol(Lax *sc, const char *name);
pointer gensym(Lax *sc);
pointer mk_string(Lax *sc, const char *str);
pointer mk_counted_string(Lax *sc, const char *str, int len);
pointer mk_empty_string(Lax *sc, int len, char fill);
pointer mk_character(Lax *sc, int c);
pointer mk_foreign_func(Lax *sc, foreign_func f);
void putstr(Lax *sc, const char *s);
int list_length(Lax *sc, pointer a);
int eqv(pointer a, pointer b);


#if USE_INTERFACE
struct Lax_interface {
  void (*Lax_define)(Lax *sc, pointer env, pointer symbol, pointer value);
  pointer (*cons)(Lax *sc, pointer a, pointer b);
  pointer (*immutable_cons)(Lax *sc, pointer a, pointer b);
  pointer (*reserve_cells)(Lax *sc, int n);
  pointer (*mk_integer)(Lax *sc, long num);
  pointer (*mk_real)(Lax *sc, double num);
  pointer (*mk_symbol)(Lax *sc, const char *name);
  pointer (*gensym)(Lax *sc);
  pointer (*mk_string)(Lax *sc, const char *str);
  pointer (*mk_counted_string)(Lax *sc, const char *str, int len);
  pointer (*mk_character)(Lax *sc, int c);
  pointer (*mk_vector)(Lax *sc, int len);
  pointer (*mk_foreign_func)(Lax *sc, foreign_func f);
  void (*putstr)(Lax *sc, const char *s);
  void (*putcharacter)(Lax *sc, int c);

  int (*is_string)(pointer p);
  char *(*string_value)(pointer p);
  int (*is_number)(pointer p);
  num (*nvalue)(pointer p);
  long (*ivalue)(pointer p);
  double (*rvalue)(pointer p);
  int (*is_integer)(pointer p);
  int (*is_real)(pointer p);
  int (*is_character)(pointer p);
  long (*charvalue)(pointer p);
  int (*is_list)(Lax *sc, pointer p);
  int (*is_vector)(pointer p);
  int (*list_length)(Lax *sc, pointer vec);
  long (*vector_length)(pointer vec);
  void (*fill_vector)(pointer vec, pointer elem);
  pointer (*vector_elem)(pointer vec, int ielem);
  pointer (*set_vector_elem)(pointer vec, int ielem, pointer newel);
  int (*is_port)(pointer p);

  int (*is_pair)(pointer p);
  pointer (*pair_car)(pointer p);
  pointer (*pair_cdr)(pointer p);
  pointer (*set_car)(pointer p, pointer q);
  pointer (*set_cdr)(pointer p, pointer q);

  int (*is_symbol)(pointer p);
  char *(*symname)(pointer p);

  int (*is_syntax)(pointer p);
  int (*is_proc)(pointer p);
  int (*is_foreign)(pointer p);
  char *(*syntaxname)(pointer p);
  int (*is_closure)(pointer p);
  int (*is_macro)(pointer p);
  pointer (*closure_code)(pointer p);
  pointer (*closure_env)(pointer p);

  int (*is_continuation)(pointer p);
  int (*is_promise)(pointer p);
  int (*is_environment)(pointer p);
  int (*is_immutable)(pointer p);
  void (*setimmutable)(pointer p);
  void (*load_file)(Lax *sc, FILE *fin);
  void (*load_string)(Lax *sc, const char *input);
};
#endif

#if !STANDALONE
typedef struct Lax_registerable
{
  foreign_func  f;
  const char *  name;
}
Lax_registerable;

void Lax_register_foreign_func_list(Lax * sc,
                                       Lax_registerable * list,
                                       int n);

#endif

#ifdef __cplusplus
}
#endif

#endif
