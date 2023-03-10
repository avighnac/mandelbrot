#include <CImg.h>
#include <gmpxx.h>
#include <ncurses.h>
#include <string>

#include "color_conversion.hpp"
#include "include.hpp"

void render_frame(int width, int height, std::string filename,
                  mpf_class &x_center, mpf_class &y_center, mpf_class &zoom,
                  size_t &MAX_ITERATIONS, WINDOW *information) {
  // Create an image of size width x height.
  cimg_library::CImg<unsigned char> image(width, height, 1, 3, 0);

  mpf_class increment = 2 / (zoom * height);
  mpf_class y_start = y_center - 1 / zoom;
  mpf_class y_end = y_center + 1 / zoom;
  mpf_class x_start = x_center - width / (height * zoom);
  mpf_class x_end = x_center + width / (height * zoom);

  mpf_class y = y_end;

  double total_pixels = width * height;
  int pixels_rendered = 0;
  for (int j = height - 1; j >= 0; j--) {
    mpf_class x = x_start;
    for (int i = 0; i < width; i++) {
      pixels_rendered++;
      int y_coord = height - j - 1;
      double percent = pixels_rendered / total_pixels * 100;
      wclear(information);
      wprintw(information, "%s%d%s%d%s%d%s%d%s%f%s",
              "\n Rendering frame!\n\n Dimensions: ", width, "x", height,
              "\n Current x-coordinate: ", i,
              "\n Current y-coordinate: ", y_coord,
              "\n Percent finished: ", percent, "\n");
      wrefresh(information);
      double abs_z = 1;
      size_t escape_iterations =
          get_number_of_escape_iterations(x, y, MAX_ITERATIONS, abs_z);

      // Smooth coloring
      if (escape_iterations != MAX_ITERATIONS)
        escape_iterations = escape_iterations + 1 - log2(log(abs_z));

      // Decide color.
      HsvColor hsv_color;
      hsv_color.h = 255 * escape_iterations / MAX_ITERATIONS;
      hsv_color.s = 255;
      hsv_color.v = escape_iterations < MAX_ITERATIONS ? 255 : 0;
      // Convert to rgb.
      RgbColor rgb_color = HsvToRgb(hsv_color);

      // Draw pixel.
      image(i, y_coord, 0, 0) = rgb_color.r;
      image(i, y_coord, 0, 1) = rgb_color.g;
      image(i, y_coord, 0, 2) = rgb_color.b;

      x += increment;
    }
    y -= increment;
  }

  image.save(filename.c_str());
}