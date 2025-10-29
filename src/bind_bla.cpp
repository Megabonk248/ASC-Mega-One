#include <sstream>
#include <pybind11/pybind11.h>

#include "vector.hpp"
#include "matrix.hpp"

using namespace ASC_bla;
namespace py = pybind11;




PYBIND11_MODULE(bla, m) {
    m.doc() = "Basic linear algebra module"; // optional module docstring
    
    py::class_<Vector<double>> (m, "Vector")
      .def(py::init<size_t>(),
           py::arg("size"), "create vector of given size")
      .def("__len__", &Vector<double>::size,
           "return size of vector")
      
      .def("__setitem__", [](Vector<double> & self, int i, double v) {
        if (i < 0) i += self.size();
        if (i < 0 || i >= self.size()) throw py::index_error("vector index out of range");
        self(i) = v;
      })
      .def("__getitem__", [](Vector<double> & self, int i) { return self(i); })
      
      .def("__setitem__", [](Vector<double> & self, py::slice inds, double val)
      {
        size_t start, stop, step, n;
        if (!inds.compute(self.size(), &start, &stop, &step, &n))
          throw py::error_already_set();
        self.range(start, stop).slice(0,step) = val;
      })
      
      .def("__add__", [](Vector<double> & self, Vector<double> & other)
      { return Vector<double> (self+other); })

      .def("__rmul__", [](Vector<double> & self, double scal)
      { return Vector<double> (scal*self); })
      
      .def("__str__", [](const Vector<double> & self)
      {
        std::stringstream str;
        str << self;
        return str.str();
      })

     .def(py::pickle(
        [](Vector<double> & self) { // __getstate__
            /* return a tuple that fully encodes the state of the object */
          return py::make_tuple(self.size(),
                                py::bytes((char*)(void*)&self(0), self.size()*sizeof(double)));
        },
        [](py::tuple t) { // __setstate__
          if (t.size() != 2)
            throw std::runtime_error("should be a 2-tuple!");

          Vector<double> v(t[0].cast<size_t>());
          py::bytes mem = t[1].cast<py::bytes>();
          std::memcpy(&v(0), PYBIND11_BYTES_AS_STRING(mem.ptr()), v.size()*sizeof(double));
          return v;
        }))
    ;

  py::class_<Matrix<double>> (m, "Matrix")
      .def(py::init<size_t, size_t>(),
           py::arg("width"), py::arg("height"), "create matrix of given dimensions")
      
      .def("__setitem__", [](Matrix<double> & self, std::tuple<int, int> i, double v) {
        if (std::get<1>(i) < 0 || std::get<1>(i) >= self.width()) throw py::index_error("Column index out of range");
        if (std::get<0>(i) < 0 || std::get<0>(i) >= self.height()) throw py::index_error("Row index out of range");
        self(std::get<1>(i),std::get<0>(i)) = v;
      })
      .def("__getitem__", [](Matrix<double> & self, std::tuple<int, int> i, int y) { return self(std::get<1>(i), std::get<0>(i)); })
      
      .def("__setitem__", [](Matrix<double> & self, std::tuple<py::slice, py::slice> inds, double val)
      {
        size_t start_y, stop_y, step_y, start_x, stop_x, step_x, n;
        if (!std::get<0>(inds).compute(self.height(), &start_y, &stop_y, &step_y, &n))
          throw py::error_already_set();
        if (!std::get<1>(inds).compute(self.height(), &start_x, &stop_x, &step_x, &n))
          throw py::error_already_set();

        for (int x = start_x; x < stop_x; x += step_x) {
          for (int y = start_y; y < stop_y; y += step_y) {
            self(x,y) = val;
          }
        }
      })

      
      .def_property_readonly("shape", [](const Matrix<double>& self) {
           return std::tuple(self.height(), self.width());
      })
      
      .def("__add__", [](Matrix<double> & self, Matrix<double> & other)
      { return Matrix<double> (self+other); })

      .def("__rmul__", [](Matrix<double> & self, double scal)
      { return Matrix<double> (scal*self); })

      .def("__mul__", [](Matrix<double> & self, Matrix<double> & other)
      { return Matrix<double> (self*other); })
      
      .def("__str__", [](const Matrix<double> & self)
      {
        std::stringstream str;
        str << self;
        return str.str();
      })

     .def(py::pickle(
        [](Matrix<double> & self) { // __getstate__
            /* return a tuple that fully encodes the state of the object */
          return py::make_tuple(self.width(), self.height(),
            py::bytes((char*)(void*)&self(0, 0), self.width() * self.height() * sizeof(double)));
        },
        [](py::tuple t) { // __setstate__
          if (t.size() != 3)
            throw std::runtime_error("should be a 3-tuple!");

          Matrix<double> v(t[0].cast<size_t>(), t[1].cast<size_t>());
          py::bytes mem = t[2].cast<py::bytes>();
          std::memcpy(&v(0, 0), PYBIND11_BYTES_AS_STRING(mem.ptr()), v.width() * v.height() * sizeof(double));
          return v;
        }))
    ;
}
