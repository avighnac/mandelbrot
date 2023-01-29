#pragma once

#include <cstddef>
#include <gmpxx.h>

extern size_t get_number_of_escape_iterations(mpf_class Real,
                                              mpf_class Imaginary,
                                              size_t &max_iterations,
                                              double &abs_z);

extern void render_frame(int width, int height, std::string filename,
                         mpf_class &x_center, mpf_class &y_center,
                         mpf_class &zoom, size_t &MAX_ITERATIONS);