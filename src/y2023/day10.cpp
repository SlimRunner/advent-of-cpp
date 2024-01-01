#include "EntryHeaders.hpp"
#include <string>
#include <string_view>

namespace {

void insertFrom(std::string & out, const std::string_view & src) {
  out.insert(out.end(), src.cbegin(), src.cend());
}

void beautifyPipes(std::string & outLine, char targetChar) {
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
    outLine.push_back(' ');
    break;

  default:
    outLine.push_back(targetChar);
    break;
  }
}

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();
  // ║═╔╗╚╝
  for (auto const & line : lines) {
    std::string wline;
    for (auto const & chr : line) {
      beautifyPipes(wline, chr);
    }

    std::cout << wline << std::endl;
  }

  std::cout << "P1: " << std::endl;
}

} // anon namespace

void y2023d10(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d10);
}
