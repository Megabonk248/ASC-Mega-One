add_executable (demo_vector demo_vector.cpp)
<<<<<<< HEAD
target_sources (demo_vector PUBLIC ../src/vector.hpp)

add_executable (demo_matrix demo_matrix.cpp)
target_sources (demo_matrix PUBLIC ../src/matrix.hpp)
=======
target_sources (demo_vector PUBLIC ../src/vector.hpp ../src/vecexpr.hpp)

add_executable (demo_matrix demo_matrix.cpp)
target_sources (demo_matrix PUBLIC ../src/matrix.hpp)
>>>>>>> 8f25794f9cf4709d5dcfd9656b2a3ff33c6c2017
