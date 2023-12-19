#include "ArgParser.hpp"
#include "Solutions.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  System::ArgParser args{argc, argv};
  auto const &sol = Solutions::getInstance(args);

  // sol.runEntry(Year::y2023, Day::d01, PuzzleChoice::RUN_INPUT);
  // sol.runEntry(Year::y2023, Day::d02, PuzzleChoice::RUN_INPUT);
  sol.runEntry(Year::y2023, Day::d03, PuzzleChoice::RUN_INPUT);
  std::cout << std::endl;
  return 0;
}
