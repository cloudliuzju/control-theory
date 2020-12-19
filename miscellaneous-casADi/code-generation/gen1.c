/* This function was automatically generated by CasADi */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef CODEGEN_PREFIX
  #define NAMESPACE_CONCAT(NS, ID) _NAMESPACE_CONCAT(NS, ID)
  #define _NAMESPACE_CONCAT(NS, ID) NS ## ID
  #define CASADI_PREFIX(ID) NAMESPACE_CONCAT(CODEGEN_PREFIX, ID)
#else /* CODEGEN_PREFIX */
  #define CASADI_PREFIX(ID) gen1_ ## ID
#endif /* CODEGEN_PREFIX */

#include <stdio.h>
#include <string.h>
#ifdef MATLAB_MEX_FILE
#include <mex.h>
#endif
#include <math.h>

#ifndef real_t
#define real_t double
#endif /* real_t */

#define to_double(x) (double) x
#define to_int(x) (int) x
/* Pre-c99 compatibility */
#if __STDC_VERSION__ < 199901L
real_t CASADI_PREFIX(fmin)(real_t x, real_t y) { return x<y ? x : y;}
#define fmin(x,y) CASADI_PREFIX(fmin)(x,y)
real_t CASADI_PREFIX(fmax)(real_t x, real_t y) { return x>y ? x : y;}
#define fmax(x,y) CASADI_PREFIX(fmax)(x,y)
#endif

#ifdef MATLAB_MEX_FILE
#define PRINTF mexPrintf
#else
#define PRINTF printf
#endif
real_t CASADI_PREFIX(sq)(real_t x) { return x*x;}
#define sq(x) CASADI_PREFIX(sq)(x)

real_t CASADI_PREFIX(sign)(real_t x) { return x<0 ? -1 : x>0 ? 1 : x;}
#define sign(x) CASADI_PREFIX(sign)(x)

void CASADI_PREFIX(fill)(real_t* x, int n, real_t alpha) {
  int i;
  if (x) {
    for (i=0; i<n; ++i) *x++ = alpha;
  }
}
#define fill(x, n, alpha) CASADI_PREFIX(fill)(x, n, alpha)


#ifdef MATLAB_MEX_FILE
real_t* CASADI_PREFIX(from_mex)(const mxArray *p, real_t* y, const int* sp, real_t* w) {
  if (!mxIsDouble(p) || mxGetNumberOfDimensions(p)!=2)
    mexErrMsgIdAndTxt("Casadi:RuntimeError","\"from_mex\" failed: Not a two-dimensional matrix of double precision.");
  int nrow = *sp++, ncol = *sp++, nnz = sp[ncol];
  const int *colind=sp, *row=sp+ncol+1;
  size_t p_nrow = mxGetM(p), p_ncol = mxGetN(p);
  const double* p_data = (const double*)mxGetData(p);
  bool is_sparse = mxIsSparse(p);
  mwIndex *Jc = is_sparse ? mxGetJc(p) : 0;
  mwIndex *Ir = is_sparse ? mxGetIr(p) : 0;
  if (p_nrow==1 && p_ncol==1) {
    double v = is_sparse && Jc[1]==0 ? 0 : *p_data;
    fill(y, nnz, v);
  } else {
    bool tr = false;
    if (nrow!=p_nrow || ncol!=p_ncol) {
      tr = nrow==p_ncol && ncol==p_nrow && (nrow==1 || ncol==1);
      if (!tr) mexErrMsgIdAndTxt("Casadi:RuntimeError","\"from_mex\" failed: Dimension mismatch.");
    }
    int r,c,k;
    if (is_sparse) {
      if (tr) {
        for (c=0; c<ncol; ++c)
          for (k=colind[c]; k<colind[c+1]; ++k) w[row[k]+c*nrow]=0;
        for (c=0; c<p_ncol; ++c)
          for (k=Jc[c]; k<Jc[c+1]; ++k) w[c+Ir[k]*p_ncol] = p_data[k];
        for (c=0; c<ncol; ++c)
          for (k=colind[c]; k<colind[c+1]; ++k) y[k] = w[row[k]+c*nrow];
      } else {
        for (c=0; c<ncol; ++c) {
          for (k=colind[c]; k<colind[c+1]; ++k) w[row[k]]=0;
          for (k=Jc[c]; k<Jc[c+1]; ++k) w[Ir[k]]=p_data[k];
          for (k=colind[c]; k<colind[c+1]; ++k) y[k]=w[row[k]];
        }
      }
    } else {
      for (c=0; c<ncol; ++c) {
        for (k=colind[c]; k<colind[c+1]; ++k) {
          y[k] = p_data[row[k]+c*nrow];
        }
      }
    }
  }
  return y;
}
#define from_mex(p, y, sp, w) CASADI_PREFIX(from_mex)(p, y, sp, w)
#endif

#ifdef MATLAB_MEX_FILE
mxArray* CASADI_PREFIX(to_mex)(const int* sp, const real_t* x) {
  int nrow = *sp++, ncol = *sp++, nnz = sp[ncol];
  mxArray* p = mxCreateSparse(nrow, ncol, nnz, mxREAL);
  int i;
  mwIndex* j;
  for (i=0, j=mxGetJc(p); i<=ncol; ++i) *j++ = *sp++;
  for (i=0, j=mxGetIr(p); i<nnz; ++i) *j++ = *sp++;
  if (x) {
    double* d = (double*)mxGetData(p);
    for (i=0; i<nnz; ++i) *d++ = to_double(*x++);
  }
  return p;
}
#define to_mex(sp, x) CASADI_PREFIX(to_mex)(sp, x)
#endif

static const int CASADI_PREFIX(s0)[5] = {1, 1, 0, 1, 0};
#define s0 CASADI_PREFIX(s0)
/* f */
int f(const real_t** arg, real_t** res, int* iw, real_t* w, int mem) {
  int i, j, k, *ii, *jj, *kk;
  const int *cii;
  real_t r, s, t, *rr, *ss, *tt;
  const real_t *cr, *cs, *ct;
  const real_t** arg1=arg+1;
  real_t** res1=res+1;
  real_t w0;
  /* #0: Input 0 (x), part 0 (x) */
  w0 = arg[0] ? arg[0][0] : 0;
  /* #1: @0 = sq(@0) */
  w0 = sq( w0 );
  /* #2: Output 0 (r) */
  if (res[0]) *res[0] = w0;
  return 0;
}

void f_incref(void) {
}

void f_decref(void) {
}

int f_n_in(void) { return 1;}

int f_n_out(void) { return 1;}

const char* f_name_in(int i){
  switch (i) {
  case 0: return "x";
  default: return 0;
  }
}

const char* f_name_out(int i){
  switch (i) {
  case 0: return "r";
  default: return 0;
  }
}

const int* f_sparsity_in(int i) {
  switch (i) {
  case 0: return s0;
  default: return 0;
  }
}

const int* f_sparsity_out(int i) {
  switch (i) {
  case 0: return s0;
  default: return 0;
  }
}

int f_work(int *sz_arg, int* sz_res, int *sz_iw, int *sz_w) {
  if (sz_arg) *sz_arg = 2;
  if (sz_res) *sz_res = 2;
  if (sz_iw) *sz_iw = 0;
  if (sz_w) *sz_w = 1;
  return 0;
}

#ifdef MATLAB_MEX_FILE
void mex_f(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {
  int i, j;
  if (argc>1) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"f\" failed. Too many input arguments (%d, max 1)", argc);
  if (resc>1) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"f\" failed. Too many output arguments (%d, max 1)", resc);
  int *iw = 0;
  real_t w[3];
  const real_t* arg[1] = {0};
  if (--argc>=0) arg[0] = from_mex(argv[0], w, s0, w+2);
  real_t* res[1] = {0};
  --resc;
  res[0] = w+1;
  i = f(arg, res, iw, w+2, 0);
  if (i) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"f\" failed.");
  if (res[0]) resv[0] = to_mex(s0, res[0]);
}
#endif

int main_f(int argc, char* argv[]) {
  int *iw = 0;
  real_t w[3];
  const real_t* arg[2] = {w+0};
  real_t* res[2] = {w+1};
  int j;
  real_t* a = w;
  for (j=0; j<1; ++j) scanf("%lf", a++);
  int flag = f(arg, res, iw, w+2, 0);
  if (flag) return flag;
  const real_t* r = w+1;
  for (j=0; j<1; ++j) PRINTF("%g ", *r++);
  PRINTF("\n");
  return 0;
}


#ifdef MATLAB_MEX_FILE
void mexFunction(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {
  char buf[2];
  int buf_ok = --argc >= 0 && !mxGetString(*argv++, buf, sizeof(buf));
  if (!buf_ok) {
    /* name error */
  } else if (strcmp(buf, "f")==0) {
    return mex_f(resc, resv, argc, argv);
  }
  mexErrMsgTxt("First input should be a command string. Possible values: 'f'");
}
#endif
int main(int argc, char* argv[]) {
  if (argc<2) {
    /* name error */
  } else if (strcmp(argv[1], "f")==0) {
    return main_f(argc-2, argv+2);
  }
  fprintf(stderr, "First input should be a command string. Possible values: 'f'\n");
  return 1;
}
#ifdef __cplusplus
} /* extern "C" */
#endif
