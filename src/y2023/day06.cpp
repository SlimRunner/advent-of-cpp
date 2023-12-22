#include "EntryHeaders.hpp"
#include "StringUtils.hpp"
#include <algorithm>
#include <cmath>
#include <exception>

namespace {

struct Race {
  int time;
  int dist;
};

std::vector<Race> getRaces(const std::string & path);

void solve(std::string path) {
  auto races = getRaces(path);
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
  // integer range is from ceil(th[1]) to floor(th[2])

  int timeProducts = 1;

  for (auto const & race: races) {
    auto const SQ = std::sqrt(race.time * race.time - 4 * race.dist);
    auto from = static_cast<int>(std::floor(0.5 * (race.time - SQ))) + 1;
    auto to   = static_cast<int>(std:: ceil(0.5 * (race.time + SQ))) - 1;
    timeProducts *= to - from + 1;
  }

  std::cout << "P1: " << timeProducts << std::endl;
}

std::vector<Race> getRaces(const std::string & path) {
  FileParser fp(path);
  auto lines = fp.getLines();
  auto times = parseInts(stringAfter(lines.at(0), ':'));
  auto dists = parseInts(stringAfter(lines.at(1), ':'));
  if (times.size() != dists.size()) {
    throw std::length_error("times and distances must be equal");
  }
  std::vector<Race> races;
  std::transform(
    times.begin(), times.end(), dists.begin(),
    std::back_inserter(races),
    [](const auto &t, const auto &d) { return Race{t, d}; }
  );

  return races;
}

} // anon namespace

void y2023d06(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d06);
}
