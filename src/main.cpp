#include <gmpxx.h>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <thread>

#include "include.hpp"

bool update_information_in_main_loop = true;

void process_input(mpf_class *x_center, mpf_class *y_center,
                   mpf_class *increment, mpf_class *zoom,
                   size_t *MAX_ITERATIONS, WINDOW *information) {
  char c;
  bool input_string = false;
  std::string input;
  while (true) {
    if (input_string) {
      move(LINES - 1, 0);
      while ((c = getch()) != '\n')
        input.push_back(c);
      input_string = false;
      int y, x;
      getyx(stdscr, y, x);
      move(y, 0);
      clrtoeol();

      // Process the request.
      // First tokenize.
      std::vector<std::string> tokens;
      std::string token;
      for (size_t i = 0; i < input.length(); i++) {
        if (input[i] == ' ') {
          if (!token.empty())
            tokens.push_back(token);
          token.clear();
        } else
          token.push_back(input[i]);
      }
      tokens.push_back(token);

      if (!tokens.empty()) {
        if (tokens[0] == "iterations" && tokens.size() >= 2) {
          *MAX_ITERATIONS = std::stoul(tokens[1]);
        }
        if (tokens[0] == "render" && tokens.size() >= 2 &&
            tokens[1].find('x') != std::string::npos) {
          int width = std::stoi(tokens[1].substr(0, tokens[1].find('x')));
          int height = std::stoi(
              tokens[1].substr(tokens[1].find('x') + 1, tokens[1].length()));
          std::string filename = "render.bmp";
          if (tokens.size() >= 3)
            filename = tokens[2];

          update_information_in_main_loop = false;

          render_frame(width, height, filename, *x_center, *y_center, *zoom,
                       *MAX_ITERATIONS, information);

          update_information_in_main_loop = true;
        }
      }

      input.clear();
    }

    move(0, 0);
    c = getch();
    switch (c) {
    case ':':
      input_string = true;
      break;
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
                            &zoom, &MAX_ITERATIONS, information);
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
        double abs_z = 1;
        size_t escape_iterations =
            get_number_of_escape_iterations(x, y, MAX_ITERATIONS, abs_z);

        if (escape_iterations == MAX_ITERATIONS)
          wprintw(mandelbrot_set_render, "%s", "*");
        else
          wprintw(mandelbrot_set_render, "%s", " ");

        x += increment;
      }
      y -= increment;
    }

    box(mandelbrot_set_render, 0, 0);
    box(information, 0, 0);
    if (update_information_in_main_loop) {
      wclear(information);
      wprintw(information, "%s%s%s%s%s%s%s%s%zu", "\n Information: \n   Real: ",
              mpf_class_to_string(x_center).c_str(),
              "\n   Imaginary: ", mpf_class_to_string(y_center).c_str(),
              "\n   Zoom: ", mpf_class_to_string(zoom).c_str(), "x",
              "\n   Max iterations: ", MAX_ITERATIONS);
      wrefresh(information);
      wrefresh(mandelbrot_set_render);
      refresh();
    }
  }

  getch();

  // Deallocates memory and ends ncurses.
  endwin();

  return 0;
}