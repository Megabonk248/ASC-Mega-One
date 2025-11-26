#include <iostream>

#include <matrix.hpp>

namespace bla = ASC_bla;

int main()
{
  size_t width = 200;
  size_t height = 200;
  bla::Matrix<double> A(width, height), B(width, height), C(width, height);

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

  ASC_bla::addMatMat(A, B, C);

  /*std::cout << "A:\n" << A;
  std::cout << "B:\n" << B;
  std::cout << "A+B:\n" << (A+B);
  std::cout << "2*A:\n" << (2*A);
  std::cout << "B*A:\n" << (B*A);

  A = A+2*B;
  std::cout << "A=A+2B:\n" << A;

  std::cout << "Inverse von A:\n" << A.Inverse();*/
}
