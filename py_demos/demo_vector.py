# search for libraray like bla.cpython-312-darwin.so in the build directory:
import sys
sys.path.append('../build/Debug')
from bla import Vector
from bla import Matrix

# import from the installed ASCsoft package:
#from ASCsoft.bla import Vector

x = Vector(3)
y = Vector(3)

for i in range(len(x)):
    x[i] = i
y[:] = 2    

print ("x =", x)
print ("y =", y)
print ("x+3*y =", x+3*y)


x = Vector(10)
x[0:] = 1
print (x)

x[3:7] = 2
print (x)

x[0:10:2] = 3
print (x)

A = Matrix(2,2)
A[(slice(0,2), slice(0,2))] = 1

B = Matrix(2,2)
B[(slice(0,2), slice(0,2))] = -2
B[(0,1)] = 3

print(A + 2 * B)


