#include "EntryHeaders.hpp"
#include <exception>
#include <map>

namespace {

template <class T>
struct Grid {
  const T x, y;
};

template <class T>
struct ShiftedGrid: public Grid<T> {
  T dx, dy;

  T manDist(const ShiftedGrid & other, T shiftFactor = 2) const {
    // default is two because the shift is expected to already be 2x
    shiftFactor = shiftFactor - 1;
    auto ax = this->x + dx * shiftFactor;
    auto ay = this->y + dy * shiftFactor;
    auto bx = other.x + other.dx * shiftFactor;
    auto by = other.y + other.dy * shiftFactor;

    T tx = 0, ty = 0;

    if (ax > bx) {
      // tx = static_cast<int>(ax) - static_cast<int>(bx);
      tx = ax - bx;
    } else {
      tx = bx - ax;
    }

    if (ay > by) {
      // ty = static_cast<int>(ay) - static_cast<int>(by);
      ty = ay - by;
    } else {
      ty = by - ay;
    }

    return tx + ty;
  }
};

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();

  const size_t WIDTH = lines.front().size();
  const size_t HEIGHT = lines.size();

  std::map<size_t, ShiftedGrid<size_t>> galaxies;

  for (size_t y = 0, dy = 0; y < HEIGHT; ++y) {
    bool emptyRow = true;

    for ( size_t x = 0; x < WIDTH; ++x) {
      switch (lines.at(y).at(x)) {
      case '#':
        emptyRow = false;
        galaxies.insert({y * WIDTH + x, ShiftedGrid<size_t>{x, y, 0UL, dy}});
        break;
      case '.':
        // nothing to do
        break;
      default:
        throw std::invalid_argument(
            "Input should only have '.' or '#'. Something else was found");
      }
    }

    if (emptyRow) {
      ++dy;
    }
  }

  for ( size_t x = 0, dx = 0; x < WIDTH; ++x) {
    bool emptyCol = true;

    for (size_t y = 0; y < HEIGHT; ++y) {
      switch (lines.at(y).at(x)) {
      case '#':
        emptyCol = false;
        galaxies.at(y * WIDTH + x).dx = dx;
        break;
      case '.':
        // nothing to do
        break;
      default:
        throw std::invalid_argument(
            "Input should only have '.' or '#'. Something else was found");
      }
    }

    if (emptyCol) {
      ++dx;
    }
  }

  size_t sumOfNaiveDist = 0UL;
  size_t sumOfHugeDist = 0UL;

  for (auto first = galaxies.cbegin(); first != galaxies.cend(); ++first) {
    for (auto second = first; second != galaxies.cend(); ++second) {
      if (first != second) {
        sumOfNaiveDist += first->second.manDist(second->second);
        sumOfHugeDist += first->second.manDist(second->second, 1000000UL);
      }
    }
  }

  std::cout << "P1: " << sumOfNaiveDist << std::endl;
  std::cout << "P2: " << sumOfHugeDist << std::endl;
}

} // anon namespace

void y2023d11(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d11);
}
