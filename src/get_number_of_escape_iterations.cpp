#include <gmpxx.h>
#include <stddef.h>

size_t get_number_of_escape_iterations(mpf_class Real, mpf_class Imaginary,
                                       size_t &max_iterations, double &abs_z) {
  mpf_class Z_Real = 0, Z_Imaginary = 0;
  for (size_t i = 0; i < max_iterations; i++) {
    mpf_class temp = Z_Real;
    Z_Real = (Z_Real + Z_Imaginary) * (Z_Real - Z_Imaginary) + Real;
    Z_Imaginary = 2 * temp * Z_Imaginary + Imaginary;

    if (Z_Real > 4 || Z_Imaginary > 4) {
      abs_z = Z_Real.get_d() * Z_Real.get_d() +
              Z_Imaginary.get_d() * Z_Imaginary.get_d();
      return i;
    }
  }

  return max_iterations;
}