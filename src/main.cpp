#include "ArgParser.hpp"
#include "Solutions.hpp"

int main(int argc, char const *argv[]) {
  System::ArgParser args{argc, argv};
  auto const &sol = Solutions::getInstance(args);

  sol.runEntry(Year::y2023, Day::d01);
  return 0;
}
