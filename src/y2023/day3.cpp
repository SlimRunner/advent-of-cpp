#include "EntryHeaders.hpp"
#include <array>
#include <sstream>
#include <vector>

struct Grid {
  int row;
  int col;

  Grid & operator+=(const Grid &);
  Grid & operator-=(const Grid &);

  bool boundCheck(Grid min, Grid max) {
    return row >= min.row && row <= max.row && col >= min.col && col <= max.col;
  }
};

Grid & Grid::operator+=(const Grid &rhs) {
  row += rhs.row;
  col += rhs.col;
  return *this;
}

Grid & Grid::operator-=(const Grid &rhs) {
  row -= rhs.row;
  col -= rhs.col;
  return *this;
}

Grid operator+(const Grid &lhs, const Grid &rhs) {
  Grid result = lhs;
  result += rhs;
  return result;
}

Grid operator-(const Grid &lhs, const Grid &rhs) {
  Grid result = lhs;
  result -= rhs;
  return result;
}

template <typename T, typename U>
T At(std::vector<std::vector<T>> &vec, U y, U x) {
  return vec.at(static_cast<size_t>(y)).at(static_cast<size_t>(x));
}

template <typename T>
T At(std::vector<std::vector<T>> &vec, Grid pt) {
  return vec.at(static_cast<size_t>(pt.row)).at(static_cast<size_t>(pt.col));
}

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();
  const size_t WIDTH = lines.at(0).size();
  const size_t HEIGHT = lines.size();
  const Grid LOWER_BOUND {0, 0};
  const Grid UPPER_BOUND{static_cast<int>(HEIGHT - 1),
                         static_cast<int>(WIDTH - 1)};
  const std::string SYM_MAP = "*@/+$=&-#%";

  std::vector<std::vector<char>> charMap;
  std::vector<std::vector<bool>> visitMap;
  std::vector<Grid> symStack;

  for (auto const & line: lines) {
    charMap.push_back({});
    visitMap.push_back({});
    for (auto const & chr: line) {
      charMap.back().push_back(chr);
      visitMap.back().push_back(false);
      if (SYM_MAP.find(chr) != SYM_MAP.npos) {
        symStack.push_back({
          static_cast<int>(charMap.size() - 1),
          static_cast<int>(charMap.back().size() - 1)
        });
      } else if (chr == '.') {
        visitMap.back().back() = true;
      }
    }
  }

  // run DFS on all 8 adjacent squares for all symbols
  // then all subsequent DFS are only left-right
  // collect all the branches in a deque in the proper order
  // join into a string and parse to int
  // add those numbers
  // for (auto const& a: symStack) {
  //   std::cout << At(charMap, a);
  // }
  // std::cout << std::endl;

  while (symStack.size()) {
    auto const here = symStack.back();
    symStack.pop_back();

    // I made At() to factor this horror out, but .at() returns rvalue
    // references so I cannot return an lvalue ref. Therefore, the jank must
    // remain here yikes.
    visitMap
      .at(static_cast<size_t>(here.row))
      .at(static_cast<size_t>(here.col)) = true;
    
    auto chr = At(charMap, here);
    if (!std::isdigit(chr)) {
      // add left and right to stack
      std::array<Grid, 8> adjacent = {{
          {-1, 1},
          {-1, 0},
          {-1, -1},
          {0, -1},
          {1, -1},
          {1, 0},
          {1, 1},
          {0, 1},
      }};
      for (auto const & move: adjacent) {
        Grid there = here + move;
        if (there.boundCheck(LOWER_BOUND, UPPER_BOUND) && !At(visitMap, there)) {
          symStack.push_back(there);
        }
      }
    } else {
      // add all adjacent to stack
      std::array<Grid, 8> nline = {{
          {0, -1},
          {0, 1},
      }};
      for (auto const & move: nline) {
        Grid there = here + move;
        if (there.boundCheck(LOWER_BOUND, UPPER_BOUND) && !At(visitMap, there)) {
          symStack.push_back(there);
        }
      }
    }
  }

  std::stringstream buffer;
  size_t bufferSize = 0;
  int total = 0;
  for (size_t y = 0; y < HEIGHT; ++y) {
    for (size_t x = 0; x < WIDTH; ++x) {
      if (At(visitMap, y, x) && std::isdigit(At(charMap, y, x))) {
        buffer << At(charMap, y, x);
        ++bufferSize;
      } else if (bufferSize > 0) {
        total += std::stoi(buffer.str());
        buffer.clear();
        buffer.str("");
        bufferSize = 0;
      }
    }
  }
  if (bufferSize > 0) {
    total += std::stoi(buffer.str());
    buffer.clear();
    buffer.str("");
  }

  std::cout << "P1: " << total;
}

void y2023d03(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d03);
}