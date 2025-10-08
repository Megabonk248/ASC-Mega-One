#ifndef FILE_MATRIX
#define FILE_MATRIX

#include <iostream>

namespace ASC_bla
{
  
  template <typename T>
  class Matrix
  {
    size_t width;
    size_t height;
    T * data;
    
  public:
    Matrix (size_t _width, size_t _height) 
      : width(_width), height(_height), data(new T[width * height]) { ; }
    
    Matrix (const Matrix & m)
      : Matrix(m.Width(), m.Height())
    {
      *this = m;
    }

    Matrix (Matrix && m)
      : width(0), height(0), data(nullptr)
    {
      std::swap(width, m.width);
      std::swap(height, m.height);
      std::swap(data, m.data);
    }

    ~Matrix () { delete [] data; }
    
    Matrix & operator=(const Matrix & m2)
    {
      for (size_t x = 0; x < width; x++) {
        for (size_t y = 0; y < height; y++) {
            this(x,y) = m2(x,y);   
        }
      }
      return *this;
    }

    Matrix & operator= (Matrix && m2)
    {
      std::swap(width, m2.width);
      std::swap(height, m2.height);
      std::swap(data, m2.data);
      return *this;
    }
    
    size_t Width() const { return width; }
    size_t Height() const { return height; }
    T & operator()(size_t x, size_t y) { return data[x * this->height + y]; }
    const T & operator()(size_t x, size_t y) const { return data[x * this->height + y]; }
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
  
  template <typename T>
  std::ostream & operator<< (std::ostream & ost, const Matrix<T> & m)
  {
    for (size_t y = 0; y < m.Height(); y++) {
        for (size_t x = 0; x < m.Width(); x++) {
            ost << m(x,y);
            if (x < m.Height() - 1) {
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
