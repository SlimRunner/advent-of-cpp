#include "EntryHeaders.hpp"
#include <array>
#include <map>
#include <sstream>
#include <vector>

namespace {

struct Grid {
  int row;
  int col;
  int hash;

  Grid & operator+=(const Grid &);
  Grid & operator-=(const Grid &);

  bool boundCheck(Grid min, Grid max) {
    return row >= min.row && row <= max.row && col >= min.col && col <= max.col;
  }

  static Grid getFromHash(int hsh, int wdt) {
    const int x = hsh % wdt;
    const int y = hsh / wdt;
    return Grid{y, x, hsh};
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
  const int INT_W = static_cast<int>(WIDTH);
  const int INT_H = static_cast<int>(HEIGHT);
  const Grid LOWER_BOUND {0, 0, 0};
  const Grid UPPER_BOUND{INT_W - 1, INT_H - 1, INT_W * INT_H - 1};
  const std::string SYM_MAP = "*@/+$=&-#%";

  std::vector<std::vector<char>> charMap;
  std::vector<std::vector<bool>> visitMap;
  std::vector<std::vector<int>> ownerMap;
  std::vector<Grid> symStack;

  for (auto const & line: lines) {
    charMap.push_back({});
    visitMap.push_back({});
    ownerMap.push_back({});
    for (auto const & chr: line) {
      charMap.back().push_back(chr);
      visitMap.back().push_back(false);
      ownerMap.back().push_back(-1);
      
      int y = static_cast<int>(charMap.size() - 1);
      int x = static_cast<int>(charMap.back().size() - 1);

      if (SYM_MAP.find(chr) != SYM_MAP.npos) {
        symStack.push_back({
          y,
          x,
          y * INT_W + x
        });
      } else if (chr == '.') {
        visitMap.back().back() = true;
      }
    }
  }

  while (symStack.size()) {
    auto const here = symStack.back();
    symStack.pop_back();

    // I made At() to factor this horror out, but .at() returns rvalue
    // references so I cannot return an lvalue ref. Therefore, the jank must
    // remain here yikes.
    visitMap
      .at(static_cast<size_t>(here.row))
      .at(static_cast<size_t>(here.col)) = true;
    ownerMap
      .at(static_cast<size_t>(here.row))
      .at(static_cast<size_t>(here.col)) = here.hash;
    
    auto chr = At(charMap, here);
    if (!std::isdigit(chr)) {
      // add left and right to stack
      std::array<Grid, 8> adjacent = {{
          {-1, 1, here.hash},
          {-1, 0, here.hash},
          {-1, -1, here.hash},
          {0, -1, here.hash},
          {1, -1, here.hash},
          {1, 0, here.hash},
          {1, 1, here.hash},
          {0, 1, here.hash},
      }};
      for (auto const & move: adjacent) {
        Grid there = here + move;
        if (there.boundCheck(LOWER_BOUND, UPPER_BOUND) && !At(visitMap, there)) {
          symStack.push_back(there);
        }
      }
    } else {
      // add all adjacent to stack
      std::array<Grid, 2> nline = {{
          {0, -1, here.hash},
          {0, 1, here.hash},
      }};
      for (auto const & move: nline) {
        Grid there = here + move;
        if (there.boundCheck(LOWER_BOUND, UPPER_BOUND) && !At(visitMap, there)) {
          symStack.push_back(there);
        }
      }
    }
  }

  std::map<int, int> partNums;
  std::map<int, int> partCount;
  int ownerID = -1;

  std::stringstream buffer;
  size_t bufferSize = 0;
  int total1 = 0;
  for (size_t y = 0; y < HEIGHT; ++y) {
    for (size_t x = 0; x < WIDTH; ++x) {
      if (At(visitMap, y, x) && std::isdigit(At(charMap, y, x))) {
        ownerID = At(ownerMap, y, x);
        buffer << At(charMap, y, x);
        ++bufferSize;
      } else if (bufferSize > 0) {
        const int partNo = std::stoi(buffer.str());
        if (partNums.find(ownerID) == partNums.end()) {
          partNums.emplace(ownerID, partNo);
          partCount.emplace(ownerID, 1);
        } else if (
          At(charMap, Grid::getFromHash(
            ownerID, INT_W
          )) == '*'
        ) {
          partNums.at(ownerID) *= partNo;
          ++partCount.at(ownerID);
        } else {
          partNums.at(ownerID) += partNo;
          ++partCount.at(ownerID);
        }
        total1 += partNo;
        buffer.clear();
        buffer.str("");
        bufferSize = 0;
      }
    }
  }
  if (bufferSize > 0) {
    const int partNo = std::stoi(buffer.str());
    if (partNums.find(ownerID) == partNums.end()) {
      partNums.emplace(ownerID, partNo);
    }
    total1 += partNo;
    buffer.clear();
    buffer.str("");
  }
  
  std::cout << "P1: " << total1 << std::endl;

  int total2 = 0;
  for (auto const & kv: partNums) {
    const char chr = At(charMap, Grid::getFromHash(kv.first, INT_W));
    if (chr == '*' && partCount.at(kv.first) == 2) {
      total2 += kv.second;
    }
  }

  std::cout << "P2: " << total2 << std::endl;
}

} // anon namespace

void y2023d03(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d03);
}