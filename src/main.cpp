#include "ArgParser.hpp"
#include "Solutions.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  System::ArgParser args{argc, argv};
  auto const &sol = Solutions::getInstance(args);

  std::cout << std::endl;
  sol.runEntry(Year::y2023, Day::d01, PuzzleChoice::RUN_EXAMPLE);
  sol.runEntry(Year::y2023, Day::d02);
  return 0;
}
