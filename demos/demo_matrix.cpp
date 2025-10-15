#include <iostream>

#include <matrix.hpp>

namespace bla = ASC_bla;

int main()
{
  size_t width = 2;
  size_t height = 2;
  bla::Matrix<double> A(width, height), B(width, height);

  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      A(x,y) = x + y * width;
    }
  }
  
  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      B(x,y) = (x + y * width) * 2;
    }
  }

  std::cout << "A:\n" << A;
  std::cout << "B:\n" << B;
  std::cout << "Inverse von A:\n" << A.Inverse();
}
