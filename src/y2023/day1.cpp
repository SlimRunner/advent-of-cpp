#include "EntryHeaders.hpp"
#include <array>
#include <cmath>
#include <iostream>
#include <limits>

namespace {

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();

  // part 1
  char chr1, chr2;
  int total1 = 0;
  for(auto const & line:lines) {
    bool first = true;
    for (auto const & letter: line) {
      if (!std::isdigit(letter)) {
        continue;
      }
      if (first) {
        chr1 = letter;
        chr2 = chr1;
      } else {
        chr2 = letter;
      }
      first = false;
    }
    total1 += static_cast<int>(chr1 - '0') * 10 + static_cast<int>(chr2 - '0');
  }
  std::cout << "P1: ";
  std::cout << total1 << std::endl;

  // part 2
  constexpr const size_t DNAME_SIZE = 10;
  const std::array<std::string, DNAME_SIZE> DNAMES = {
      "zero", "one", "two",   "three", "four",
      "five", "six", "seven", "eight", "nine",
  };
  int total2 = 0;
  int dig1, dig2;
  for (auto const & line:lines) {
    size_t smallest = std::numeric_limits<size_t>::max();
    size_t largest = std::numeric_limits<size_t>::min();
    size_t here;
    for (size_t i = 1; i < DNAME_SIZE; ++i) {
      here = line.find(DNAMES.at(i));
      if (here != std::string::npos && here < smallest) {
        smallest = here;
        dig1 = static_cast<int>(i);
      }
      here = line.find(DNAMES.at(i));
      while (here != std::string::npos) {
        here = line.find(DNAMES.at(i), here + 1);
      }
      here = line.rfind(DNAMES.at(i));
      if (here != std::string::npos && here > largest) {
        largest = here;
        dig2 = static_cast<int>(i);
      }
    }
    
    here = line.find_first_of("0123456789");
    if (here != std::string::npos && here <= smallest) {
      dig1 = std::stoi(line.substr(here, 1));
    }
    here = line.find_last_of("0123456789");
    if (here != std::string::npos && here >= largest) {
      dig2 = std::stoi(line.substr(here, 1));
    }
    
    total2 += dig1 * 10 + dig2;
  }

  std::cout << "P2: ";
  std::cout << total2 << std::endl;
  /*
  goddammit I was checking against 0123459789 (not the 9 in place of 6)
  */
}

} // anon namespace

void y2023d01(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d01);
}