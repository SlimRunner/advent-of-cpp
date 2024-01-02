#include "ArgParser.hpp"
#include "Solutions.hpp"
#include <chrono>
#include <iostream>

int main(int argc, char const *argv[]) {
  System::ArgParser args{argc, argv};
  auto const &sol = Solutions::getInstance(args);

  using steady = std::chrono::steady_clock;
  using chrono_ms = std::chrono::microseconds;

  steady::time_point begin = steady::now();

  sol.runEntry(Year::y2023, Day::d01, PuzzleChoice::RUN_ALL);
  sol.runEntry(Year::y2023, Day::d02, PuzzleChoice::RUN_ALL);
  sol.runEntry(Year::y2023, Day::d03, PuzzleChoice::RUN_ALL);
  sol.runEntry(Year::y2023, Day::d04, PuzzleChoice::RUN_ALL);
  sol.runEntry(Year::y2023, Day::d05, PuzzleChoice::RUN_ALL);
  sol.runEntry(Year::y2023, Day::d06, PuzzleChoice::RUN_ALL);
  sol.runEntry(Year::y2023, Day::d07, PuzzleChoice::RUN_ALL);
  sol.runEntry(Year::y2023, Day::d08, PuzzleChoice::RUN_ALL);
  sol.runEntry(Year::y2023, Day::d09, PuzzleChoice::RUN_ALL);
  sol.runEntry(Year::y2023, Day::d10, PuzzleChoice::RUN_ALL);

  steady::time_point end = steady::now();
  std::cout << "Above puzzles ran in ";
  std::cout << std::chrono::duration_cast<chrono_ms>(end - begin).count() / 1000;
  std::cout << " ms" << std::endl;
  return 0;
}
