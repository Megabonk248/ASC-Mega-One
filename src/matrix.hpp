#ifndef FILE_MATRIX
#define FILE_MATRIX

#include <iostream>
#include <algorithm>

#include "matrixexpr.hpp"

namespace ASC_bla
{

  template <typename T, typename TDISTX = std::integral_constant<size_t,1>, typename TDISTY = std::integral_constant<size_t,1>, typename TOFFX = std::integral_constant<size_t,0>, typename TOFFY = std::integral_constant<size_t,0>, typename TWIDTH = std::integral_constant<size_t,SIZE_MAX>, typename THEIGHT = std::integral_constant<size_t,SIZE_MAX> >
  class MatrixView : public MatrixExpr<MatrixView<T,TDISTX,TDISTY>>
  {
  protected:
    T * m_data;
    size_t m_width;
    size_t m_height;
    TWIDTH m_window_width;
    THEIGHT m_window_height;
    TDISTX m_dist_x;
    TDISTY m_dist_y;
    TOFFX m_offset_x;
    TOFFY m_offset_y;
  public:
    MatrixView() = default;
    MatrixView(const MatrixView &) = default;
    
    template <typename TDISTX2, typename TDISTY2>
    MatrixView (const MatrixView<T, TDISTX2, TDISTY2> & m2)
      : m_data(m2.data()), m_width(m2.full_width()), m_width(m2.full_height()), m_dist_x(m2.dist_x()), m_dist_y(m2.dist_y()), m_offset_x(m2.offset_x()), m_offset_y(m2.offset_y()), m_window_width(m2.window_width()), m_window_height(m2.window_height()) { };
    
    MatrixView (size_t width, size_t height, T * data)
      : m_data(data), m_width(width), m_height(height) { }
    
    MatrixView (size_t width, size_t height, TDISTX dist_x, TDISTY dist_y , T * data)
      : m_data(data), m_width(width), m_height(height), m_dist_x(dist_x), m_dist_y(dist_y) { }
    
    MatrixView (size_t width, size_t height, TWIDTH window_width, THEIGHT window_height, TOFFX offset_x, TOFFY offset_y, T * data)
      : m_data(data), m_width(width), m_height(height), m_window_width(width), m_window_height(height), m_offset_x(offset_x), m_offset_y(offset_y) { }
    
    template <typename TB>
    MatrixView & operator= (const MatrixExpr<TB> & m2)
    {
      assert (this->width() == m2.width());
      assert (this->height() == m2.height());
      for (size_t x = 0; x < this->width(); x++) {
        for (size_t y = 0; y < this->height(); y++) {
          (*this)(x, y) = m2(x, y);
        }
      }
      return *this;
    }

    MatrixView & operator= (T scal)
    {

      for (size_t x = 0; x < this->width(); x++) {
        for (size_t y = 0; y < this->height(); y++) {
          this(x, y) = scal;
        }
      }
      return *this;
    }

    T * data() const { return m_data; }
    size_t full_width() const { return m_width; }
    size_t full_height() const { return m_height; }
    size_t width() const { return std::min(m_width, (size_t) m_window_width) / m_dist_x; }
    size_t height() const { return std::min(m_height, (size_t) m_window_height) / m_dist_y; }
    auto dist_x() const { return m_dist_x; }
    auto dist_y() const { return m_dist_y; }
    auto offset_x() const { return m_offset_x; }
    auto offset_y() const { return m_offset_y; }
    auto window_width() const { return m_window_width; }
    auto window_height() const { return m_window_height; }
    
    T & operator()(size_t x, size_t y) { return m_data[m_dist_x * (x + m_offset_x) * m_height + m_dist_y * (y + m_offset_y)]; }
    const T & operator()(size_t x, size_t y) const { return m_data[m_dist_x * (x + m_offset_x) * m_height + m_dist_y * (y + m_offset_y)]; }
      
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
      : Matrix(m.width(), m.height())
    {
      *this = m;
    }

    Matrix (Matrix && m)
      : MatrixView<T> (0, 0, nullptr)
    {
      std::swap(m_width, m.m_width);
      std::swap(m_height, m.m_height);
      std::swap(m_data, m.m_data);
    }

    template <typename TB>
    Matrix (const MatrixExpr<TB> & m)
      : Matrix(m.width(), m.height())
    {
      *this = m;
    }

    ~Matrix () { delete [] m_data; }
    
    using BASE::operator=;
    Matrix & operator=(const Matrix & m2)
    {
      assert (this->width() == m2.width());
      assert (this->height() == m2.height());
      for (size_t x = 0; x < this->width(); x++) {
        for (size_t y = 0; y < this->height(); y++) {
            (*this)(x,y) = m2(x,y);   
        }
      }
      return *this;
    }

    Matrix & operator= (Matrix && m2)
    {
      assert (this->width() == m2.width());
      assert (this->height() == m2.height());
      for (size_t x = 0; x < this->width(); x++) {
        for (size_t y = 0; y < this->height(); y++) {
            this(x,y) = m2(x,y);   
        }
      }
      return *this;
    }
    
    Matrix<T> Inverse() const
      {
        if (m_width != m_height)
          throw std::runtime_error("Matrix muss quadratisch sein, um invertiert zu werden.");

        size_t n = m_width;
        Matrix<T> A(*this);
        Matrix<T> I(n, n);

        for (size_t i = 0; i < n; ++i) {
          for (size_t j = 0; j < n; ++j) {
            I(i, j) = (i == j) ? 1 : 0;
          }
        }
          
        for (size_t i = 0; i < n; ++i)
        {
          T pivot = A(i, i);
          if (std::fabs(pivot) < 1e-12)
          {
            size_t swapRow = i + 1;
            while (swapRow < n && std::fabs(A(swapRow, i)) < 1e-12)
              ++swapRow;

            if (swapRow == n)
              throw std::runtime_error("Matrix ist singulär und kann nicht invertiert werden.");

            for (size_t k = 0; k < n; ++k)
            {
              std::swap(A(i, k), A(swapRow, k));
              std::swap(I(i, k), I(swapRow, k));
            }
            pivot = A(i, i);
          }

          // Pivot-Zeile normieren
          for (size_t k = 0; k < n; ++k)
          {
            A(i, k) /= pivot;
            I(i, k) /= pivot;
          }

          // Andere Zeilen eliminieren
          for (size_t j = 0; j < n; ++j)
          {
            if (j == i) continue;
            T factor = A(j, i);
            for (size_t k = 0; k < n; ++k)
            {
              A(j, k) -= factor * A(i, k);
              I(j, k) -= factor * I(i, k);
            }
          }
        }

        return I;
      }
  };

  template<typename T>
  void addMatMat (MatrixView<T> A, MatrixView<T> B, MatrixView<T> C) {
  constexpr size_t BH=96;
  constexpr size_t BW=96;
  alignas (64) double memBA[BH*BW];
  for (size_t i1 = 0; i1 < A.height(); i1 += BH)
    for (size_t j1 = 0; j1 < A.width(); j1 += BW) {
      size_t i2 = min(A.height(), i1+BH);
      size_t j2 = min(A.width(), j1+BW);

      size_t w = j2-j1;
      size_t h = i2-i1;
      MatrixView<T> Ablock(w, h,  memBA);
      MatrixView<T> ASub(A.full_width(), A.full_height(), w, h, j1, i1, A.data());
      MatrixView<T> BSub(B.full_width(), B.full_height(), B.window_width(), w, B.offset_x(), j1, B.data());
      MatrixView<T> CSub(C.full_width(), C.full_height(), C.window_width(), w, C.offset_x(), j1, C.data());
      Ablock = ASub;
      addMatMat2(Ablock, BSub, CSub);
    }
  }

  template<typename T>
  void addMatMat2 (MatrixView<T> A, MatrixView<T> B, MatrixView<T> C) {
    constexpr size_t H=4;
    constexpr size_t W=12;

    /*for (size_t j = 0; j+W <= C.width(); j += W) 
      for (size_t i = 0; i+H <= C.height(); i += H)
        AddMatMatKernel<H,W> (A.width(), &A(i,0), A.dist(),
                            &B(0,j), B.dist(), &C(i,j), C.dist());*/
    // leftover rows and cols

    std::cout << "aargh";
  }


  /*template <typename T>
  Matrix<T> operator+ (const Matrix<T> & a, const Matrix<T> & b)
  {
    Matrix<T> sum(a.width(), a.height());
    for (size_t x = 0; x < a.width(); x++) {
        for (size_t y = 0; y < a.height(); y++) {
            sum(x,y) = a(x,y)+b(x,y);
        }
    }
    return sum;
  }*/

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
