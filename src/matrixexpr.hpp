#ifndef FILE_EXPRESSION_MATRIX
#define FILE_EXPRESSION_MATRIX

#include <cassert>

namespace ASC_bla
{

  template <typename T>
  class MatrixExpr
  {
  public:
    auto derived() const { return static_cast<const T&> (*this); }
    size_t width() const { return derived().width(); }
    size_t height() const { return derived().height(); }
    auto operator() (size_t x, size_t y) const { return derived()(x,y); }
  };
  
 // ***************** Sum of two matrices *****************

  template <typename TA, typename TB>
  class SumMatrixExpr : public MatrixExpr<SumMatrixExpr<TA,TB>>
  {
    TA a;
    TB b;
  public:
    SumMatrixExpr (TA _a, TB _b) : a(_a), b(_b) { }
    auto operator() (size_t x, size_t y) const { return a(x,y)+b(x,y); }
    size_t width() const { return a.width(); }      
    size_t height() const { return a.height(); }      
  };
  
  template <typename TA, typename TB>
  auto operator+ (const MatrixExpr<TA> & a, const MatrixExpr<TB> & b)
  {
    assert (a.width() == b.width());
    assert (a.height() == b.height());
    return SumMatrixExpr(a.derived(), b.derived());
  }


// ***************** Scaling a matrix *****************
  
  template <typename TSCAL, typename TV>
  class ScaleMatrixExpr : public MatrixExpr<ScaleMatrixExpr<TSCAL,TV>>
  {
    TSCAL scal;
    TV vec;
  public:
    ScaleMatrixExpr (TSCAL _scal, TV _vec) : scal(_scal), vec(_vec) { }
    auto operator() (size_t x, size_t y) const { return scal*vec(x,y); }
    size_t width() const { return vec.width(); }      
    size_t height() const { return vec.height(); }      
  };
  
  template <typename T>
  auto operator* (double scal, const MatrixExpr<T> & v)
  {
    return ScaleMatrixExpr(scal, v.derived());
  }


// ***************** Multiplying matrices *****************
  
  template <typename TA, typename TB>
  class MultiplyMatrixExpr : public MatrixExpr<MultiplyMatrixExpr<TA,TB>>
  {
    TA a;
    TB b;
  public:
    MultiplyMatrixExpr (TA _a, TB _b) : a(_a), b(_b) { }
    auto operator() (size_t x, size_t y) const { 
      auto s = 0;
      for (size_t i = 0; i < a.width(); i++) {
        s += a(x, i) * b(i, y);
      }
      return s;
     }
    size_t width() const { return a.width(); }      
    size_t height() const { return a.height(); }      
  };
  
  template <typename TA, typename TB>
  auto operator* (const MatrixExpr<TA> & a, const MatrixExpr<TB> & b)
  {
    assert (a.width() == b.width());
    assert (a.height() == b.height());
    return MultiplyMatrixExpr(a.derived(), b.derived());
  }

  // ***************** Output operator *****************

  template <typename T>
  std::ostream & operator<< (std::ostream & ost, const MatrixExpr<T> &m)
  {
    for (size_t y = 0; y < m.height(); y++) {
        for (size_t x = 0; x < m.width(); x++) {
            ost << m(x,y);
            if (x < m.height() - 1) {
                ost << ", ";
            } else {
                ost << "\n";
            }
        }
    }
    return ost;
  }
  
}
 
#endif
