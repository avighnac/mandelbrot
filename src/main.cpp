#include <gmpxx.h>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <thread>

#include "get_number_of_escape_iterations.hpp"

void process_input(mpf_class *x_center, mpf_class *y_center,
                   mpf_class *increment, mpf_class *zoom) {
  while (true) {
    move(0, 0);
    char c = getch();
    switch (c) {
    case 'w':
      *y_center += *increment;
      break;
    case 'a':
      *x_center -= *increment;
      break;
    case 's':
      *y_center -= *increment;
      break;
    case 'd':
      *x_center += *increment;
      break;
    case '=':
      *zoom *= 1.05;
      break;
    case '-':
      *zoom /= 1.05;
      break;
    default:
      break;
    }
  }
}

std::string mpf_class_to_string(const mpf_class &number) {
  std::stringstream ss;
  ss << number;
  return ss.str();
}

int main(int argc, char **argv) {
  // Intializes the screen, sets up memory and clears the screen.
  initscr();

  move(0, COLS / 2 - 10);

  // Prints a string(const *char) to the window.
  printw("Mandelbrot Set Viewer\n");

  int WIDTH = COLS / 2, HEIGHT = LINES - 2;
  WINDOW *mandelbrot_set_render = newwin(HEIGHT, WIDTH, 1, 1);
  WINDOW *information = newwin(HEIGHT, WIDTH - 1, 1, COLS / 2 + 1);
  refresh();

  // Mandelbrot set variables
  size_t MAX_ITERATIONS = 2400;
  mpf_class x_center = 0;
  mpf_class y_center = 0;
  mpf_class zoom = 1;
  mpf_class increment;

  // Render the mandelbrot set
  std::thread input_handler(process_input, &x_center, &y_center, &increment,
                            &zoom);
  while (true) {
    wclear(mandelbrot_set_render);

    increment = 2 / (zoom * HEIGHT);
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

    wclear(information);
    wprintw(information, "%s%s%s%s%s%s%s%s%zu", "\n Information: \n   Real: ",
            mpf_class_to_string(x_center).c_str(),
            "\n   Imaginary: ", mpf_class_to_string(y_center).c_str(),
            "\n   Zoom: ", mpf_class_to_string(zoom).c_str(), "x",
            "\n   Max iterations: ", MAX_ITERATIONS);

    box(mandelbrot_set_render, 0, 0);
    box(information, 0, 0);
    wrefresh(information);
    wrefresh(mandelbrot_set_render);
    refresh();
  }

  getch();

  // Deallocates memory and ends ncurses.
  endwin();

  return 0;
}