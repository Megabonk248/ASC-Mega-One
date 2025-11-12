add_executable (demo_vector demo_vector.cpp)
target_sources (demo_vector PUBLIC ../src/vector.hpp ../src/vecexpr.hpp)

add_executable (demo_matrix demo_matrix.cpp)
target_sources (demo_matrix PUBLIC ../src/matrix.hpp ../src/matrixexpr.hpp)

add_executable (test_simd_functions test_simd_functions.cpp)
target_sources (test_simd_functions PUBLIC ../src/simd_functions.hpp)
