#include "EntryHeaders.hpp"
#include "StringUtils.hpp"

namespace {

void solve(std::string path) {
  FileParser fp(path);
  auto const lines = fp.getLines();
  auto line = lines.cbegin();
  auto const EOL = lines.cend();

  std::vector<std::vector<std::vector<int>>> maps;

  auto seeds = parseInts(stringAfter(*(line++), ':'));
  while (line != EOL && (line)->size() == 0) {line++;}
  if (line != EOL) line++;
  
  while (line != EOL) {
    maps.push_back({});
    while (line != EOL && (line)->size() > 0) {
      maps.back().push_back(parseInts(stringAfter(*line++, ':')));
    }
    while (line != EOL && (line)->size() == 0) {line++;}
    if (line != EOL) line++;
  }

  for (auto i: maps) {
    for (auto j: i) {
      for (auto k: j) {
        std::cout << k << " " << std::flush;
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  std::cout << "P1: " << std::endl;
}

} // anon namespace

void y2023d05(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d05);
}
