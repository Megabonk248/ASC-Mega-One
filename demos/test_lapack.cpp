#include <iostream>

#include <vector.hpp>
#include <matrix.hpp>
#include <lapack_interface.hpp>


using namespace ASC_bla;
using namespace std;


int main()
{
  Vector<double> x(5);
  Vector<double> y(5);

  for (int i = 0; i < x.size(); i++)
    {
      x(i) = i;
      y(i) = 2;
    }

  cout << "x = " << x << endl;
  cout << "y = " << y << endl;
  
  addVectorLapack (2, x, y);  
  cout << "y+2*x = " << y << endl;

  Matrix<double> a(2, 2);
  Matrix<double> b(2, 2);
  Matrix<double> c(2, 2);

  a(0, 0) = 1;
  a(0, 1) = 1;
  a(1, 0) = 2;
  a(1, 1) = 2;

  b(0, 0) = 0;
  b(0, 1) = 2;
  b(1, 0) = 3;
  b(1, 1) = 4;

  cout << "a = " << endl << a << endl;
  cout << "b = " << endl << b << endl;
  multMatMatLapack(a, b, c);

  cout << "a*b = " << c << endl;


}

  
