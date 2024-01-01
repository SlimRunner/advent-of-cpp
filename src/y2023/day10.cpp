#include "EntryHeaders.hpp"
#include <string>
#include <string_view>

namespace {

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();
  // ║═╔╗╚╝
  constexpr const std::string_view VERT_PIPE = "║";
  constexpr const std::string_view HORZ_PIPE = "═";
  constexpr const std::string_view CORN_SE_PIPE = "╔";
  constexpr const std::string_view CORN_SW_PIPE = "╗";
  constexpr const std::string_view CORN_NE_PIPE = "╚";
  constexpr const std::string_view CORN_NW_PIPE = "╝";
  for (auto const & line : lines) {
    std::string wline;
    for (auto const & chr : line) {
      switch (chr) {
      case '|':
        wline.insert(wline.end(), VERT_PIPE.cbegin(), VERT_PIPE.cend());
        break;
      case '-':
        wline.insert(wline.end(), HORZ_PIPE.cbegin(), HORZ_PIPE.cend());
        break;
      case 'L':
        wline.insert(wline.end(), CORN_NE_PIPE.cbegin(), CORN_NE_PIPE.cend());
        break;
      case 'J':
        wline.insert(wline.end(), CORN_NW_PIPE.cbegin(), CORN_NW_PIPE.cend());
        break;
      case '7':
        wline.insert(wline.end(), CORN_SW_PIPE.cbegin(), CORN_SW_PIPE.cend());
        break;
      case 'F':
        wline.insert(wline.end(), CORN_SE_PIPE.cbegin(), CORN_SE_PIPE.cend());
        break;
      case '.':
        wline.push_back(' ');
        break;

      default:
        wline.push_back(chr);
        break;
      }
    }

    std::cout << wline << std::endl;
  }

  std::cout << "P1: " << std::endl;
}

} // anon namespace

void y2023d10(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d10);
}
