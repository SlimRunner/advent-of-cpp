#include "EntryHeaders.hpp"

static void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();
}

void y2023d02(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d02);
}