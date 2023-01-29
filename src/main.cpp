#include <fstream>
#include <gmpxx.h>
#include <iostream>
#include <ncurses.h>

#include "get_number_of_escape_iterations.hpp"

int main(int argc, char **argv) {
  // Intializes the screen, sets up memory and clears the screen.
  initscr();

  move(0, COLS / 2 - 10);

  // Prints a string(const *char) to the window.
  printw("Mandelbrot Set Viewer\n");

  int WIDTH = COLS - 2, HEIGHT = LINES - 2;
  WINDOW *mandelbrot_set_render = newwin(LINES - 2, COLS - 2, 1, 1);
  refresh();

  // Mandelbrot set variables
  size_t MAX_ITERATIONS = 2400;
  mpf_class x_center = 0;
  mpf_class y_center = 0;
  mpf_class zoom = 1;

  // Render the mandelbrot set
  while (true) {
    wclear(mandelbrot_set_render);

    mpf_class increment = 2 / (zoom * HEIGHT);
    mpf_class y_start = y_center - 1 / zoom;
    mpf_class y_end = y_center + 1 / zoom;
    mpf_class x_start = x_center - WIDTH / (HEIGHT * zoom);
    mpf_class x_end = x_center + WIDTH / (HEIGHT * zoom);

    mpf_class y = y_end;
    for (int j = HEIGHT - 1; j >= 0; j--) {
      mpf_class x = x_start;
      for (int i = 0; i < WIDTH; i++) {
        size_t escape_iterations =
            get_number_of_escape_iterations(x, y, MAX_ITERATIONS);

        if (escape_iterations == MAX_ITERATIONS)
          wprintw(mandelbrot_set_render, "%s", "*");
        else
          wprintw(mandelbrot_set_render, "%s", " ");

        x += increment;
      }
      y -= increment;
    }

    box(mandelbrot_set_render, 0, 0);
    wrefresh(mandelbrot_set_render);
    refresh();
  }

  getch();

  // Deallocates memory and ends ncurses.
  endwin();

  return 0;
}