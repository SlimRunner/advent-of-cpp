#include "EntryHeaders.hpp"
#include "StringUtils.hpp"
#include <algorithm>
#include <cmath>
#include <exception>

namespace {

template <class T>
struct Race {
  T time;
  T dist;
};

std::vector<Race<int>> getRaces(const System::vecstring & lines);
Race<long long> getTheRace(const System::vecstring & lines);

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();
  auto races = getRaces(lines);

  int prodOfTimes = 1;

  for (auto const & race: races) {
    auto const SQ = std::sqrt(race.time * race.time - 4 * race.dist);
    auto from = static_cast<int>(std::floor(0.5 * (race.time - SQ))) + 1;
    auto to   = static_cast<int>(std:: ceil(0.5 * (race.time + SQ))) - 1;
    prodOfTimes *= to - from + 1;
  }

  std::cout << "P1: " << prodOfTimes << std::endl;

  auto theRace = getTheRace(lines);
  auto const SQ = std::sqrt(theRace.time * theRace.time - 4 * theRace.dist);
  auto from = static_cast<int>(std::floor(0.5 * (theRace.time - SQ))) + 1;
  auto to   = static_cast<int>(std:: ceil(0.5 * (theRace.time + SQ))) - 1;
  auto winningMatches = to - from + 1;

  std::cout << "P2: " << winningMatches << std::endl;
}

std::vector<Race<int>> getRaces(const System::vecstring & lines) {
  auto times = parseInts(stringAfter(lines.at(0), ':'));
  auto dists = parseInts(stringAfter(lines.at(1), ':'));
  if (times.size() != dists.size()) {
    throw std::length_error("times and distances must be equal");
  }
  std::vector<Race<int>> races;
  std::transform(
    times.begin(), times.end(), dists.begin(),
    std::back_inserter(races),
    [](int t, int d) { return Race<int>{t, d}; }
  );

  return races;
}

Race<long long> getTheRace(const System::vecstring & lines) {
  auto times = parseLLs(stringAfter(lines.at(0), ':'), ' ');
  auto dists = parseLLs(stringAfter(lines.at(1), ':'), ' ');
  if (times.size() != dists.size()) {
    throw std::length_error("times and distances must be equal");
  }
  if (times.size() != 1) {
    throw std::length_error("times and distances have a size of 1");
  }

  return Race<long long>{times.at(0), dists.at(0)};
}

} // anon namespace

void y2023d06(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d06);
}
