#include "EntryHeaders.hpp"

#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>

struct Cubes {
  int red = 0;
  int green = 0;
  int blue = 0;
};

bool operator<=(const Cubes & lhs, const Cubes & rhs) {
  return lhs.red <= rhs.red && lhs.green <= rhs.green && lhs.blue <= rhs.blue;
}

int powerOfCubes(const Cubes & cb) {
  return cb.red * cb.green * cb.blue;
}

Cubes max(const Cubes & lhs, const Cubes & rhs) {
  Cubes cb = lhs;
  if (rhs.red > lhs.red) cb.red = rhs.red;
  if (rhs.green > lhs.green) cb.green = rhs.green;
  if (rhs.blue > lhs.blue) cb.blue = rhs.blue;
  return cb;
}

static void parseInput(
  const std::vector<std::string> & lines,
  std::vector<std::vector<Cubes>> & games
) {
  const std::regex rxNums("(\\d+)\\s*(\\w+)", std::regex_constants::ECMAScript);
  std::stringstream drawline;

  for (auto const & line: lines) {
    auto draws = line.substr(line.find(":") + 1);
    games.push_back({});
    drawline.clear();
    drawline.str("");

    for (auto itChar = draws.begin(); true; ++itChar) {
      if (itChar == draws.end() || *itChar == ';') {
        std::smatch nums;
        auto text = drawline.str();
        drawline.clear();
        drawline.str("");
        games.back().push_back({});

        while (std::regex_search(text, nums, rxNums)) {
          auto const name = nums[2];
          if (name == "red") {
            games.back().back().red = std::stoi(nums[1]);
          } else if (name == "green") {
            games.back().back().green = std::stoi(nums[1]);
          } else if (name == "blue") {
            games.back().back().blue = std::stoi(nums[1]);
          }
          text = nums.suffix().str();
        }
      } else {
        drawline << *itChar;
      }

      if (itChar == draws.end()) {
        break;
      }
    }
  }
}

static void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();
  
  // assume the input always have indices in order and contigouous

  std::vector<std::vector<Cubes>> games;
  const Cubes MAX_P1 = {12, 13, 14};
  parseInput(lines, games);

  size_t index = 1, gameScore = 0, cubePow = 0;
  for (auto const& game :games) {
    bool count = true;
    Cubes least;
    for (auto const & draw: game) {
      least = max(least, draw);
      count = count && draw <= MAX_P1;
    }
    if (count) {
      gameScore += index;
    }
    cubePow += static_cast<size_t>(powerOfCubes(least));
    ++index;
  }

  std::cout << "P1: " << gameScore << std::endl;
  std::cout << "P2: " << cubePow << std::endl;
}

void y2023d02(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d02);
}