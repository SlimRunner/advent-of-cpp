#include "EntryHeaders.hpp"
#include "StringUtils.hpp"
#include <algorithm>
#include <cmath>
#include <exception>

namespace {

template <typename T>
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

  // a = 1 mm/(ms^2)
  // 'a' builds-up and increases at once, so assume a running start that counts
  // distance only once you let go off the accelerator
  // tt: time total (given)
  // dr: record distance (given)
  // th: time holding accelerator (independent)
  // th <= tt
  // tr: time remaining
  // tr = tt - th
  // v0 = th * a

  // d(th): distance traveled
  // d(th) = tr * v0 = tr * th * a
  //       = tr * th = (tt - th) * th
  //       = tt * th - th^2
  // distance-record difference
  // d(th) - dr
  // find 0 crossings
  // ->  0  = d(th) - dr
  //     0  = tt * th - th^2 - dr
  // quad formula: a = -1, b = tt, c = -dr
  // ->  th = -(1/2) * (-tt +- sqrt(tt^2 - 4 * -1 * -dr))
  //     th = (1/2) * (tt -+ sqrt(tt^2 - 4 * dr))
  // 1-> th = (1/2) * (tt - sqrt(tt^2 - 4 * dr))
  // 2-> th = (1/2) * (tt + sqrt(tt^2 - 4 * dr))

  int timeProducts = 1;

  for (auto const & race: races) {
    auto const SQ = std::sqrt(race.time * race.time - 4 * race.dist);
    auto from = static_cast<int>(std::floor(0.5 * (race.time - SQ))) + 1;
    auto to   = static_cast<int>(std:: ceil(0.5 * (race.time + SQ))) - 1;
    timeProducts *= to - from + 1;
  }

  std::cout << "P1: " << timeProducts << std::endl;

  auto theRace = getTheRace(lines);
  (void)theRace;
  auto const SQ = std::sqrt(theRace.time * theRace.time - 4 * theRace.dist);
  auto from = static_cast<int>(std::floor(0.5 * (theRace.time - SQ))) + 1;
  auto to   = static_cast<int>(std:: ceil(0.5 * (theRace.time + SQ))) - 1;
  auto result = to - from + 1;

  std::cout << "P2: " << result << std::endl;
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
  auto times = parseLL(stringAfter(lines.at(0), ':'), ' ');
  auto dists = parseLL(stringAfter(lines.at(1), ':'), ' ');
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
