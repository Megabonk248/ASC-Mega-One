#include <iostream>

#include <vector.hpp>
#include <matrix.hpp>

namespace bla = ASC_bla;

int main()
{
  size_t n = 5;
  bla::Vector<double> x(n), y(n);

  for (size_t i = 0; i < x.Size(); i++)
    {
      x(i) = i;
      y(i) = 10;
    }

  bla::Vector<double> z = x+y;
  
  std::cout << "x+y = " << z << std::endl;

  size_t width = 2;
  size_t height = 2;
  bla::Matrix<double> A(width, height), B(width, height);

  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      A(x,y) = x * width + y;
    }
  }
  
  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      B(x,y) = -(x * width + y) * 2;
    }
  }

  std::cout << "A:\n" << A;
  std::cout << "B:\n" << B;
}
