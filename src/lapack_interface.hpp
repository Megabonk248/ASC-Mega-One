#ifndef FILE_LAPACK_INTERFACE_HPP
#define FILE_LAPACK_INTERFACE_HPP

#include <iostream>
#include <string>

#include "vector.hpp"
#include "matrix.hpp"



#include <complex>

typedef int integer;
typedef integer logical;
typedef float real;
typedef double doublereal;
typedef std::complex<float> singlecomplex;
typedef std::complex<double> doublecomplex;

// Windows SDK defines VOID in the file WinNT.h
#ifndef VOID
typedef void VOID;
#endif
typedef int ftnlen;
typedef int L_fp;  


extern "C" {
#include <clapack.h>
}


namespace ASC_bla
{


  // BLAS-1 functions:

  /*
    int daxpy_(integer *n, doublereal *da, doublereal *dx, 
    integer *incx, doublereal *dy, integer *incy);
  */
  // y += alpha x
  template <typename SX, typename SY>
  void addVectorLapack (double alpha, VectorView<double,SX> x, VectorView<double,SY> y)
  {
    integer n = x.size();
    integer incx = x.dist();
    integer incy = y.dist();
    daxpy_ (&n, &alpha, &x(0),  &incx, &y(0), &incy);
  }
  
  
  // BLAS-2 functions:

  // BLAS-3 functions:
  
  // int dgemm_ (char *transa, char *transb, integer *m, integer * n,
  // integer *k, doublereal *alpha, doublereal *a, integer *lda, 
  // doublereal *b, integer *ldb, doublereal *beta, doublereal *c__, 
  // integer *ldc);

 
  // c = a*b
  void multMatMatLapack (MatrixView<double> a,
                         MatrixView<double> b,
                         MatrixView<double> c)
  {
    integer n = c.width();
    integer m = c.height();
    integer k = a.width();
  
    double alpha = 1.0;
    double beta = 0;
    integer lda = std::max((unsigned long)m, 1ul);
    integer ldb = std::max((unsigned long)k, 1ul);
    integer ldc = std::max((unsigned long)m, 1ul);

    char transa_ ='N';
    char transb_ ='N';
    std::cout << "help" << ldb;
    int err =
      dgemm_ (&transa_, &transb_, &m, &n, &k, &alpha, 
              a.data(), &lda, b.data(), &ldb, &beta, c.data(), &ldc);
    std::cout << "help2: " << err;
    if (err != 0)
      throw std::runtime_error(std::string("MultMatMat got error "+std::to_string(err)));
    std::cout << "help3?";
  }

  

  /*
  template <ORDERING ORD>
  class LapackLU {
    Matrix <double, ORD> a;
    std::vector<integer> ipiv;
    
  public:
    LapackLU (Matrix<double,ORD> _a)
      : a(std::move(_a)), ipiv(a.rows()) {
      integer m = a.rows();
      if (m == 0) return;
      integer n = a.cols();
      integer lda = a.dist();
      integer info;
    
      // int dgetrf_(integer *m, integer *n, doublereal *a, 
      //             integer * lda, integer *ipiv, integer *info);

      dgetrf_(&n, &m, &a(0,0), &lda, &ipiv[0], &info);
    }
    
    // b overwritten with A^{-1} b
    void solve (VectorView<double> b) const {
      char transa =  (ORD == ColMajor) ? 'N' : 'T';
      integer n = a.rows();
      integer nrhs = 1;
      integer lda = a.dist();
      integer ldb = b.size();
      integer info;

      // int dgetrs_(char *trans, integer *n, integer *nrhs, 
      //             doublereal *a, integer *lda, integer *ipiv,
      //             doublereal *b, integer *ldb, integer *info);

      dgetrs_(&transa, &n, &nrhs, a.data(), &lda, (integer*)ipiv.data(), b.data(), &ldb, &info);
    }
  
    Matrix<double,ORD> inverse() && {
      double hwork;
      integer lwork = -1;
      integer n = a.Height();      
      integer lda = a.Dist();
      integer info;

      // int dgetri_(integer *n, doublereal *a, integer *lda, 
      //             integer *ipiv, doublereal *work, integer *lwork, 
      //             integer *info);

      // query work-size
      dgetri_(&n, &a(0,0), &lda, ipiv.data(), &hwork, &lwork, &info);
      lwork = integer(hwork);
      std::vector<double> work(lwork);
      dgetri_(&n, &a(0,0), &lda, ipiv.data(), &work[0], &lwork, &info);
      return std::move(a);      
    }

    // Matrix<double,ORD> LFactor() const { ... }
    // Matrix<double,ORD> UFactor() const { ... }
    // Matrix<double,ORD> PFactor() const { ... }
  };
  */ 

  
}


#endif
