#include "EntryHeaders.hpp"
#include "StringUtils.hpp"

#include <set>
#include <map>
#include <algorithm>

namespace {

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();

  std::vector<std::vector<int>> lottoNums;
  std::vector<std::vector<int>> winNums;

  // parse input to be more tractable
  for (auto const & line: lines) {
    lottoNums.push_back(parseInts(stringBetween(line, ':', '|')));
    winNums.push_back(parseInts(stringAfter(line, '|')));
  }

  int total = 0;
  for (
    auto lotIt = lottoNums.cbegin(), winIt = winNums.cbegin();
    lotIt != lottoNums.cend() || winIt != winNums.cend();
    ++lotIt, ++winIt
  ) {
    std::set<int> winKeys(winIt->begin(), winIt->end());
    int pow = 1;
    for (auto const & num: *lotIt) {
      if (winKeys.find(num) != winKeys.end()) {
        pow *= 2;
      }
    }
    total += pow / 2;
  }

  std::cout << "P1: " << total << std::endl;
}

} // anon namespace

void y2023d04(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d04);
}