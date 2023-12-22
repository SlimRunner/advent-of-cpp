#include "EntryHeaders.hpp"
#include "StringUtils.hpp"
#include <algorithm>

namespace {

void solve(std::string path) {
  FileParser fp(path);
  auto const lines = fp.getLines();
  auto line = lines.cbegin();
  auto const EOL = lines.cend();

  std::vector<std::vector<std::vector<long long>>> almanac;

  auto const seeds = parseLL(stringAfter(*(line++), ':'));
  auto params{seeds};
  while (line != EOL && line->size() == 0) {++line;}
  if (line != EOL) ++line;
  
  while (line != EOL) {
    almanac.push_back({});
    while (line != EOL && line->size() > 0) {
      almanac.back().push_back(parseLL(stringAfter(*line++, ':')));
    }
    while (line != EOL && line->size() == 0) {++line;}
    if (line != EOL) ++line;
  }

  for (auto && param: params) {
    for (auto const & maps: almanac) {
      for (auto const & map: maps) {
        auto const range = param - map.at(1);
        if (range >= 0 && range < map.at(2)) {
          auto const delta = map.at(0) - map.at(1);
          param += delta;
          break;
        }
      }
    }
  }

  long long minParam = -1LL;
  if (auto it = std::min_element(params.begin(), params.end()); it != params.end()) {
    minParam = *it;
  }

  std::cout << "P1: " << minParam << std::endl;
}

} // anon namespace

void y2023d05(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d05);
}
