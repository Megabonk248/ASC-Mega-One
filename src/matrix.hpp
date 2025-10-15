#ifndef FILE_MATRIX
#define FILE_MATRIX

#include <iostream>

#include "matrixexpr.hpp"

namespace ASC_bla
{

  template <typename T, typename TDISTX = std::integral_constant<size_t,1>, typename TDISTY = std::integral_constant<size_t,1> >
  class MatrixView : public MatrixExpr<MatrixView<T,TDISTX,TDISTY>>
  {
  protected:
    T * m_data;
    size_t m_width;
    size_t m_height;
    TDISTX m_dist_x;
    TDISTY m_dist_y;
  public:
    MatrixView() = default;
    MatrixView(const MatrixView &) = default;
    
    template <typename TDISTX2, typename TDISTY2>
    MatrixView (const MatrixView<T, TDISTX2, TDISTY2> & m2)
      : m_data(m2.data()), m_width(m2.full_width()), m_width(m2.full_height()), m_dist_x(m2.dist_x()), m_dist_y(m2.dist_y()) { };
    
    MatrixView (size_t width, size_t height, T * data)
      : m_data(data), m_width(width), m_height(height) { }
    
    MatrixView (size_t width, size_t height, TDISTX dist_x, TDISTY dist_y , T * data)
      : m_data(data), m_width(width), m_height(height), m_dist_x(dist_x), m_dist_y(dist_y) { }
    
    template <typename TB>
    MatrixView & operator= (const MatrixExpr<TB> & m2)
    {
      assert (this.width() == m2.width());
      assert (this.height() == m2.height());
      for (size_t x = 0; x < this.width(); x++) {
        for (size_t y = 0; y < this.height(); y++) {
          this(x, y) = m2(x, y);
        }
      }
      return *this;
    }

    MatrixView & operator= (T scal)
    {

      for (size_t x = 0; x < this.width(); x++) {
        for (size_t y = 0; y < this.height(); y++) {
          this(x, y) = scal;
        }
      }
      return *this;
    }

    T * data() const { return m_data; }
    size_t full_width() const { return m_width; }
    size_t full_height() const { return m_height; }
    size_t width() const { return m_width / m_dist_x; }
    size_t height() const { return m_height / m_dist_y; }
    auto dist_x() const { return m_dist_x; }
    auto dist_y() const { return m_dist_y; }
    
    T & operator()(size_t x, size_t y) { return m_data[m_dist_x * x * m_height + m_dist_y * y]; }
    const T & operator()(size_t x, size_t y) const { return m_data[m_dist_x * x * m_height + m_dist_y * y]; }
      
  };
  
  template <typename T>
  class Matrix : public MatrixView<T>
  {
    typedef MatrixView<T> BASE;
    using BASE::m_width;
    using BASE::m_height;
    using BASE::m_data;
    
  public:
    Matrix (size_t width, size_t height) 
      : MatrixView<T> (width, height, new T[width * height]) { ; }
    
    Matrix (const Matrix & m)
      : Matrix(m.Width(), m.Height())
    {
      *this = m;
    }

    Matrix (Matrix && m)
      : MatrixView<T> (0, nullptr)
    {
      std::swap(m_width, m.full_width());
      std::swap(m_height, m.full_height());
      std::swap(m_data, m.data());
    }

    ~Matrix () { delete [] m_data; }
    
    using BASE::operator=;
    Matrix & operator=(const Matrix & m2)
    {
      assert (this.width() == m2.width());
      assert (this.height() == m2.height());
      for (size_t x = 0; x < this.width(); x++) {
        for (size_t y = 0; y < this.height(); y++) {
            this(x,y) = m2(x,y);   
        }
      }
      return *this;
    }

    Matrix & operator= (Matrix && m2)
    {
      assert (this.width() == m2.width());
      assert (this.height() == m2.height());
      for (size_t x = 0; x < this.width(); x++) {
        for (size_t y = 0; y < this.height(); y++) {
            this(x,y) = m2(x,y);   
        }
      }
      return *this;
    }
  };


  template <typename T>
  Matrix<T> operator+ (const Matrix<T> & a, const Matrix<T> & b)
  {
    Matrix<T> sum(a.Size());
    for (size_t x = 0; x < a.Width(); x++) {
        for (size_t y = 0; y < a.Height(); y++) {
            sum(x,y) = a(x,y)+b(x,y);
        }
    }
    return sum;
  }

  template <typename T>
  Matrix<T> operator- (const Matrix<T> & a, const Matrix<T> & b)
  {
    Matrix<T> diff(a.Size());
    for (size_t x = 0; x < a.Width(); x++) {
        for (size_t y = 0; y < a.Height(); y++) {
            diff(x,y) = a(x,y)+b(x,y);
        }
    }
    return diff;
  }
  
  template <typename ...Args>
  std::ostream & operator<< (std::ostream & ost, const MatrixView<Args...> & m)
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
