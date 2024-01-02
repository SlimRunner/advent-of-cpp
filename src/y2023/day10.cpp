#include "EntryHeaders.hpp"
#include <deque>
#include <exception>
#include <queue>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

class Direction {
public:
  enum Value {
    NORTH = 0x1,
    WEST = 0x2,
    SOUTH = 0x4,
    EAST = 0x8,
  };

  constexpr inline operator int() {
    return static_cast<int>(value);
  }
private:
  Value value;
};

// constexpr inline int operator int(const Direction & other) {
//   return <int>(other);
// }

// constexpr inline int operator|(L lhs, R rhs) {
//   return static_cast<int>(static_cast<int>(lhs) | static_cast<int>(rhs));
// }

enum class TileType {
  GROUND = 0x0,
  START = Direction::NORTH | Direction::WEST | Direction::SOUTH | Direction::EAST,
  PIPE_VERT = Direction::NORTH | Direction::SOUTH,
  PIPE_HORZ = Direction::WEST | Direction::EAST,
  PIPE_CORN_NE = Direction::NORTH | Direction::EAST,
  PIPE_CORN_NW = Direction::NORTH | Direction::WEST,
  PIPE_CORN_SW = Direction::WEST | Direction::SOUTH,
  PIPE_CORN_SE = Direction::SOUTH | Direction::EAST,
};

constexpr inline int operator&(TileType lhs, Direction::Value rhs) {
  return static_cast<int>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

[[maybe_unused]]
std::ostream & operator<<(std::ostream & ostr, const TileType tile) {
  switch (tile) {
  case TileType::GROUND:
    ostr << "Ground";
    break;
  case TileType::START:
    ostr << "Start";
    break;
  case TileType::PIPE_VERT:
    ostr << "Vertical pipe";
    break;
  case TileType::PIPE_HORZ:
    ostr << "Horizontal pipe";
    break;
  case TileType::PIPE_CORN_NE:
    ostr << "North-East pipe";
    break;
  case TileType::PIPE_CORN_NW:
    ostr << "North-West pipe";
    break;
  case TileType::PIPE_CORN_SW:
    ostr << "South-West pipe";
    break;
  case TileType::PIPE_CORN_SE:
    ostr << "South-East pipe";
    break;

  default:
    ostr << "INVALID";
    break;
  }

  return ostr;
}

template <class T>
struct Grid {
  T x, y, hash;

  Grid & shift(int xd, int yd) {
    x = static_cast<T>(static_cast<int>(x) + xd);
    y = static_cast<T>(static_cast<int>(y) + yd);
    return *this;
  }

  Grid & updateHash(T wdt) {
    hash = y * wdt + x;
    return *this;
  }

  bool boundCheck(Grid min, Grid max) const {
    return y >= min.y && y <= max.y && x >= min.x && x <= max.x;
  }

  static Grid getFromHash(int hsh, int wdt) {
    const int x = hsh % wdt;
    const int y = hsh / wdt;
    return Grid{x, y, hsh};
  }
};

void insertFrom(std::string & out, const std::string_view & src) {
  out.insert(out.end(), src.cbegin(), src.cend());
}

void beautifyPipes(std::string & outLine, char targetChar) {
  // ║═╔╗╚╝

  constexpr const std::string_view VERT_PIPE = "║";
  constexpr const std::string_view HORZ_PIPE = "═";
  constexpr const std::string_view CORN_SE_PIPE = "╔";
  constexpr const std::string_view CORN_SW_PIPE = "╗";
  constexpr const std::string_view CORN_NE_PIPE = "╚";
  constexpr const std::string_view CORN_NW_PIPE = "╝";

  switch (targetChar) {
  case '|':
    insertFrom(outLine, VERT_PIPE);
    break;
  case '-':
    insertFrom(outLine, HORZ_PIPE);
    break;
  case 'L':
    insertFrom(outLine, CORN_NE_PIPE);
    break;
  case 'J':
    insertFrom(outLine, CORN_NW_PIPE);
    break;
  case '7':
    insertFrom(outLine, CORN_SW_PIPE);
    break;
  case 'F':
    insertFrom(outLine, CORN_SE_PIPE);
    break;
  case '.':
    outLine.push_back('*');
    break;

  default:
    outLine.push_back(targetChar);
    break;
  }
}

TileType parseTile(char targetChar) {
  switch (targetChar) {
  case '|':
    return TileType::PIPE_VERT;
  case '-':
    return TileType::PIPE_HORZ;
  case 'L':
    return TileType::PIPE_CORN_NE;
  case 'J':
    return TileType::PIPE_CORN_NW;
  case '7':
    return TileType::PIPE_CORN_SW;
  case 'F':
    return TileType::PIPE_CORN_SE;
  case '.':
    return TileType::GROUND;
  case 'S':
    return TileType::START;

  default:
    throw std::invalid_argument("Invalid tile character.");
  }
}

std::vector<Grid<size_t>> getCandidates(
  Grid<size_t> from, TileType tile, const std::vector<std::vector<TileType>> & tiles
) {
  const auto EAST = Grid(from).shift(1, 0);
  const auto NORTH = Grid(from).shift(0, -1);
  const auto WEST = Grid(from).shift(-1, 0);
  const auto SOUTH = Grid(from).shift(0, 1);

  std::vector<Grid<size_t>> result;

  const auto pushIf = [&tiles, &tile, &result](Grid<size_t> moveTo, Direction::Value sourceMatch, Direction::Value targetMatch) {
    const size_t FIELD_WIDTH = tiles.at(0).size();
    const size_t FIELD_HEIGHT = tiles.size();
    const Grid<size_t> MIN_BOUND = {0, 0, 0};
    const Grid<size_t> MAX_BOUND = {FIELD_WIDTH - 1, FIELD_HEIGHT - 1, 0}; // this hash is unnecessary

    if (
      moveTo.boundCheck(MIN_BOUND, MAX_BOUND) &&
      tiles.at(moveTo.y).at(moveTo.x) & targetMatch &&
      tile & sourceMatch
    ) {
      moveTo.updateHash(FIELD_WIDTH);
      result.push_back(moveTo);
    }
  };

  switch (tile) {
  case TileType::GROUND:
    // nothing to do
  case TileType::START:
    pushIf(EAST, Direction::EAST, Direction::WEST);
    pushIf(NORTH, Direction::NORTH, Direction::SOUTH);
    pushIf(WEST, Direction::WEST, Direction::EAST);
    pushIf(SOUTH, Direction::SOUTH, Direction::NORTH);
    break;
  case TileType::PIPE_VERT:
    pushIf(NORTH, Direction::NORTH, Direction::SOUTH);
    pushIf(SOUTH, Direction::SOUTH, Direction::NORTH);
    break;
  case TileType::PIPE_HORZ:
    pushIf(EAST, Direction::EAST, Direction::WEST);
    pushIf(WEST, Direction::WEST, Direction::EAST);
    break;
  case TileType::PIPE_CORN_NE:
    pushIf(EAST, Direction::EAST, Direction::WEST);
    pushIf(NORTH, Direction::NORTH, Direction::SOUTH);
    break;
  case TileType::PIPE_CORN_NW:
    pushIf(NORTH, Direction::NORTH, Direction::SOUTH);
    pushIf(WEST, Direction::WEST, Direction::EAST);
    break;
  case TileType::PIPE_CORN_SW:
    pushIf(WEST, Direction::WEST, Direction::EAST);
    pushIf(SOUTH, Direction::SOUTH, Direction::NORTH);
    break;
  case TileType::PIPE_CORN_SE:
    pushIf(EAST, Direction::EAST, Direction::WEST);
    pushIf(SOUTH, Direction::SOUTH, Direction::NORTH);
    break;

  default:
    throw std::invalid_argument("Invalid tile type");
    break;
  }
  
  return result;
}

// bool doTilesMatch(TileType tile1, TileType tile2) {

// }

template <class T>
T meetingStepsInLoop(const std::vector<std::vector<TileType>> & tiles, Grid<size_t> start) {
  std::queue<Grid<size_t>, std::deque<Grid<size_t>>> queue;
  std::set<size_t> visited;
  T steps = 0;

  queue.push(start);
  visited.insert(start.hash);

  for (Grid<size_t> here; !queue.empty(); ++steps) {
    here = queue.front();
    auto thisTile = tiles.at(here.y).at(here.x);
    queue.pop();
    
    for (auto && newTile: getCandidates(here, thisTile, tiles)){
      if (visited.find(newTile.hash) == visited.end()) {
        visited.insert(newTile.hash);
        queue.push(newTile);
      }
    }
  }

  return steps / 2;
}

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();

  const size_t FIELD_WIDTH = lines.at(0).size();

  std::vector<std::vector<TileType>> tiles;
  Grid<size_t> start;
  bool startAssigned = false;
  
  for (auto const & line : lines) {
    std::string wline;
    tiles.push_back({});
    for (auto const & chr : line) {
      tiles.back().push_back(parseTile(chr));
      if (tiles.back().back() == TileType::START) {
        if (startAssigned) {
          throw std::logic_error("Start can only be assigned once.");
        }
        const size_t x = static_cast<size_t>(tiles.back().size() - 1);
        const size_t y = static_cast<size_t>(tiles.size() - 1);

        startAssigned = true;
        start = {x, y, y * FIELD_WIDTH + x};
      }
      beautifyPipes(wline, chr);
    }
    std::cout << wline << std::endl;
  }

  auto farthestStepInLoop = meetingStepsInLoop<size_t>(tiles, start);

  std::cout << "P1: " << farthestStepInLoop << std::endl;
}

} // anon namespace

void y2023d10(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d10);
}
