cmake_minimum_required(VERSION 3.22)

project(mandelbrot)

add_executable(
    mandelbrot

    main.cpp
    get_number_of_escape_iterations.cpp
    render_frame.cpp
)

target_link_libraries(mandelbrot ncurses)
target_link_libraries(mandelbrot gmp)
target_link_libraries(mandelbrot gmpxx)
target_link_libraries(mandelbrot m)
target_link_libraries(mandelbrot pthread)
target_link_libraries(mandelbrot X11)