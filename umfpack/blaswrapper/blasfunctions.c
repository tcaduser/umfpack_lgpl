
#include "blasfunctions.h"

#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <assert.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

/* this should work, since fortran is using pointers */
typedef struct doublecomplex_struct
{
  double r;
  double i;
} doublecomplex;

typedef void (dgemm_signature)(char *transa, char *transb, int *m, int *n, int *k, double *alpha, double *a, int *lda, double *b, int *ldb, double *beta, double *c, int *ldc);

typedef void   (dgemv_signature)(char *trans, int *m, int *n, double *alpha, double *a, int *lda, double *x, int *incx, double *beta, double *y, int *incy);

typedef void (dger_signature)(int *m, int *n, double *alpha, double *x, int *incx, double *y, int *incy, double *a, int *lda);

typedef void   (dtrsm_signature)(char *side, char *uplo, char *transa, char *diag, int *m, int *n, double *alpha, double *a, int *lda, double *b, int *ldb);

typedef void   (dtrsv_signature)(char *uplo, char *trans, char *diag, int *n, double *a, int *lda, double *x, int *incx);

typedef void   (zgemm_signature)(char *transa, char *transb, int *m, int *n, int *k, doublecomplex *alpha, doublecomplex *a, int *lda, doublecomplex *b, int *ldb, doublecomplex *beta, doublecomplex *c, int *ldc);
typedef void   (zgemv_signature)(char *trans, int *m, int *n, doublecomplex *alpha, doublecomplex *a, int *lda, doublecomplex *x, int *incx, doublecomplex *beta, doublecomplex *y, int *incy);

typedef void (zgeru_signature)(int *m, int *n, doublecomplex *alpha, doublecomplex *x, int *incx, doublecomplex *y, int *incy, doublecomplex *a, int *lda);

typedef void   (ztrsm_signature)(char *side, char *uplo, char *transa, char *diag, int *m, int *n, doublecomplex *alpha, doublecomplex *a, int *lda, doublecomplex *b, int *ldb);
typedef void   (ztrsv_signature)(char *uplo, char *trans, char *diag, int *n, doublecomplex *a, int *lda, doublecomplex *x, int *incx);

struct blas_table {
  /* all functions are part of LAPACK, and should be in the same lib */
  void *dll_handle;
  dgemm_signature *dgemm;
  dgemv_signature *dgemv;
  dger_signature *dger;
  dtrsm_signature *dtrsm;
  dtrsv_signature *dtrsv;
  zgemm_signature *zgemm;
  zgemv_signature *zgemv;
  zgeru_signature *zgeru;
  ztrsm_signature *ztrsm;
  ztrsv_signature *ztrsv;
};
/* TODO: verify this implies that the table is 0 initialized */
static struct blas_table blas_tab;

struct symtable {
  const char *symbol_name;
  void ** const function_pointer;
};

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define TOBLASSTRING(x) STRINGIFY(x) "_"

static struct symtable math_function_table[] = {
  {TOBLASSTRING(dgemm), (void **) &(blas_tab.dgemm)},
  {TOBLASSTRING(dgemv), (void **) &(blas_tab.dgemv)},
  {TOBLASSTRING(dger), (void **) &(blas_tab.dger)},
  {TOBLASSTRING(dtrsm), (void **) &(blas_tab.dtrsm)},
  {TOBLASSTRING(dtrsv), (void **) &(blas_tab.dtrsv)},
  {TOBLASSTRING(zgemm), (void **) &(blas_tab.zgemm)},
  {TOBLASSTRING(zgemv), (void **) &(blas_tab.zgemv)},
  {TOBLASSTRING(zgeru), (void **) &(blas_tab.zgeru)},
  {TOBLASSTRING(ztrsm), (void **) &(blas_tab.ztrsm)},
  {TOBLASSTRING(ztrsv), (void **) &(blas_tab.ztrsv)},
  {NULL, NULL}
};

void dgemm_(char *transa, char *transb, int *m, int *n, int *k, double *alpha, double *a, int *lda, double *b, int *ldb, double *beta, double *c, int *ldc)
{
  assert(blas_tab.dgemm);
  blas_tab.dgemm(transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}

void dgemv_(char *trans, int *m, int *n,
                              double *alpha, double *a,
                              int *lda, double *x, int *incx,
                              double *beta, double *y, int *incy)
{
  assert(blas_tab.dgemv);
  blas_tab.dgemv(trans, m, n, alpha, a, lda, x, incx, beta, y, incy);
}

void dger_(int *m, int *n, double *alpha, double *x, int *incx, double *y, int *incy, double *a, int *lda)
{
  assert(blas_tab.dger);
  blas_tab.dger(m, n, alpha, x, incx, y, incy, a, lda);
}

void dtrsm_(char *side, char *uplo, char *transa, char *diag, int *m, int *n, double *alpha, double *a, int *lda, double *b, int *ldb)
{
  assert(blas_tab.dtrsm);
  blas_tab.dtrsm(side, uplo, transa, diag, m, n, alpha, a, lda, b, ldb);
}

void dtrsv_(char *a, char *b, char *c, int *d, double *e, int *f, double *g, int *h)
{
  assert(blas_tab.dtrsv);
  blas_tab.dtrsv(a, b, c, d, e, f, g, h);
}

void   zgemm_(char *transa, char *transb, int *m, int *n, int *k, doublecomplex *alpha, doublecomplex *a, int *lda, doublecomplex *b, int *ldb, doublecomplex *beta, doublecomplex *c, int *ldc)
{
  assert(blas_tab.zgemm);
  blas_tab.zgemm(transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}


void   zgemv_(char *trans, int *m, int *n, doublecomplex *alpha, doublecomplex *a, int *lda, doublecomplex *x, int *incx, doublecomplex *beta, doublecomplex *y, int *incy)
{
  assert(blas_tab.zgemv);
  blas_tab.zgemv(trans, m, n, alpha, a, lda, x, incx, beta, y, incy);
}

void zgeru_(int *m, int *n, doublecomplex *alpha, doublecomplex *x, int *incx, doublecomplex *y, int *incy, doublecomplex *a, int *lda)
{
  assert(blas_tab.zgeru);
  blas_tab.zgeru(m, n, alpha, x, incx, y, incy, a, lda);
}

void   ztrsm_(char *side, char *uplo, char *transa, char *diag, int *m, int *n, doublecomplex *alpha, doublecomplex *a, int *lda, doublecomplex *b, int *ldb)
{
  assert(blas_tab.ztrsm);
  blas_tab.ztrsm(side, uplo, transa, diag, m, n, alpha, a, lda, b, ldb);
}

void   ztrsv_(char *uplo, char *trans, char *diag, int *n, doublecomplex *a, int *lda, doublecomplex *x, int *incx)
{
  assert(blas_tab.ztrsv);
  blas_tab.ztrsv(uplo, trans, diag, n, a, lda, x, incx);
}

void blasw_clear_table()
{
  struct symtable *tbl = math_function_table;
  while (tbl->symbol_name)
  {
    *(tbl->function_pointer) = NULL;
    ++tbl;
  }
}

int blasw_load_functions(void *dll_p)
{
  int missing_count = 0;
  struct symtable *tbl = math_function_table;
  while (tbl->symbol_name)
  {
    if (*(tbl->function_pointer) == NULL)
    {
#ifdef _WIN32
      void *h = (void *) GetProcAddress(dll_p, tbl->symbol_name);
#else
      void *h = dlsym(dll_p, tbl->symbol_name);
#endif
      if (h != NULL)
      {
        *(tbl->function_pointer) = h;
      }
      else
      {
        ++missing_count;
      }
    }
    ++tbl;
  }
  return missing_count;
}

/* TODO: load using dlopen */
/* TODO: close things up */
void *blasw_load_dll(const char *dllname, const char **error)
{
  *error = NULL;
#if defined(_WIN32)
  HINSTANCE *dll_p = NULL;
#else
  void *dll_p = NULL;
#endif

  if (!dllname)
  {
    return NULL;
  }

#if defined(_WIN32)
  dll_p = LoadLibrary(dllname);
#else
#if defined(__APPLE__)
  int dllflags = RTLD_LOCAL | RTLD_NOW;
#elif defined(__linux__)
  int dllflags = RTLD_LOCAL | RTLD_NOW | RTLD_DEEPBIND;
#endif
  dll_p = dlopen(dllname, RTLD_NOLOAD | dllflags);
  if (!dll_p)
  {
    // Reference count is zero until this is called
    dll_p = dlopen(dllname, dllflags);
  }
  else // reference count is now 2, but we need to make sure someone else does not close
  {
  }

  if (!dll_p)
  {
    *error = dlerror();
  }
#endif
  return dll_p;
}

/* see amd_global.c */
extern int (*amd_printf) (const char *, ...);        /* pointer to printf */

void blasw_set_printf_function(blasw_printf_function_type pf)
{
  amd_printf = pf;
}

static blasw_rawprint_cb *rawprint_cb = NULL;

static int blasw_rawprint_helper(const char *msg, ...)
{
  static char buf[8192];
  va_list args;
  va_start(args, msg);
  int cnt = vsnprintf(buf, 8192, msg, args);
  va_end(args);
  assert(rawprint_cb);
  rawprint_cb(buf);
  return cnt;
}

void blasw_set_printer_callback(blasw_rawprint_cb rpcb)
{
  assert(rpcb);
  rawprint_cb = rpcb;
  amd_printf = blasw_rawprint_helper;
}


