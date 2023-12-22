#include "EntryHeaders.hpp"
#include "StringUtils.hpp"

#include <set>
#include <algorithm>

namespace {

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();

  std::vector<std::vector<int>> lottoNums;
  std::vector<std::vector<int>> winNums;
  std::vector<int> cardCount(lines.size());

  // parse input to be more tractable
  for (auto const & line: lines) {
    lottoNums.push_back(parseInts(stringBetween(line, ':', '|')));
    winNums.push_back(parseInts(stringAfter(line, '|')));
  }

  int points = 0;
  int cards = 0;
  auto countIt = cardCount.begin();

  for (
    auto lotIt = lottoNums.cbegin(), winIt = winNums.cbegin();
    lotIt != lottoNums.cend() || winIt != winNums.cend();
    ++lotIt, ++winIt, ++countIt
  ) {
    std::set<int> winKeys(winIt->begin(), winIt->end());
    int pow = 1;

    *countIt += 1;
    cards += (*countIt);
    auto forwardIt = countIt;

    for (auto const & num: *lotIt) {
      if (winKeys.find(num) != winKeys.end()) {
        *(++forwardIt) += (*countIt);
        pow *= 2;
      }
    }
    points += pow / 2;
  }

  std::cout << "P1: " << points << std::endl;
  std::cout << "P2: " << cards << std::endl;
}

} // anon namespace

void y2023d04(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d04);
}