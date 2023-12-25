#include "EntryHeaders.hpp"
#include "StringUtils.hpp"
#include <algorithm>
#include <utility>
#include <exception>
#include <limits>

namespace {

template <typename T>
struct Range {
  T from;
  T to;
};

template <typename T>
struct AlmanacEntry {
  T destStart;
  T sourceStart;
  T rangeSpan;

  AlmanacEntry(std::vector<T> values) {
    if (values.size() != 3) {
      throw std::invalid_argument("An almanac entry must be of size 3.");
    } else {
      destStart = values.at(0);
      sourceStart = values.at(1);
      rangeSpan = values.at(2);
    }
  }
};

void solve(std::string path) {
  FileParser fp(path);
  auto const lines = fp.getLines();
  auto line = lines.cbegin();
  auto const EOL = lines.cend();

  std::vector<std::vector<AlmanacEntry<long long>>> almanac;

  auto const seeds = parseLLs(stringAfter(*(line++), ':'));
  auto params{seeds};
  while (line != EOL && line->size() == 0) {++line;}
  if (line != EOL) ++line;
  
  while (line != EOL) {
    almanac.push_back({});
    while (line != EOL && line->size() > 0) {
      almanac.back().push_back(parseLLs(*line++));
    }
    while (line != EOL && line->size() == 0) {++line;}
    if (line != EOL) ++line;
  }

  for (auto && param: params) {
    for (auto const & maps: almanac) {
      for (auto const & map: maps) {
        auto const offset = param - map.sourceStart;
        if (offset >= 0 && offset < map.rangeSpan) {
          auto const delta = map.destStart - map.sourceStart;
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

  std::vector<Range<long long>> seedRanges;
  for (auto it = seeds.begin(); it != seeds.end(); it += 2) {
    seedRanges.push_back({*it, *it + *(it + 1) - 1});
  }
  long long smallest = std::numeric_limits<long long>().max();

  // this is a probably going to take until the heat death of the universe
  // I have an idea
  // compute the piecewise in reverse from the lowest possible value
  // until both the value is possible and is within any of the ranges of seeds
  for (auto const & seedRange: seedRanges) {
    for (long long i = seedRange.from; i <= seedRange.to; ++i) {
      auto param = i;
      for (auto const &maps : almanac) {
        for (auto const &map : maps) {
          auto const offset = param - map.sourceStart;
          if (offset >= 0 && offset < map.rangeSpan) {
            auto const delta = map.destStart - map.sourceStart;
            param += delta;
            break;
          }
        }
      }

      if (smallest > param) {
        smallest = param;
      }
    }
  }

  std::cout << "P2: " << smallest << std::endl;
}

} // anon namespace

void y2023d05(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d05);
}
