#include "EntryHeaders.hpp"

namespace {

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();

  (void)lines; // unused variable

  std::cout << "P1: " << std::endl;
}

} // anon namespace

void y2023d08(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d08);
}
