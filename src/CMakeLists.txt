cmake_minimum_required(VERSION 3.22)

project(mandelbrot)

add_executable(
    mandelbrot

    main.cpp
)

target_link_libraries(mandelbrot ncurses)
target_link_libraries(mandelbrot gmp)
target_link_libraries(mandelbrot gmpxx)