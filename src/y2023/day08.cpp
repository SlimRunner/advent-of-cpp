#include "EntryHeaders.hpp"
#include "StringUtils.hpp"
#include <array>
#include <cstring> // memcpy
#include <exception>
#include <map>
#include <string_view>
#include <utility> // pair
#include <vector>

namespace {

enum class NodeChoice {
  LEFT,
  RIGHT
};

NodeChoice parseChoice(char chr) {
  switch (chr) {
  case 'l':
  case 'L':
    return NodeChoice::LEFT;

  case 'r':
  case 'R':
    return NodeChoice::RIGHT;
    break;

  default:
    throw std::invalid_argument(
        "Expected a character R or L (case insensitive)");
  }
}

template <typename T>
T chooseNode(std::pair<T, T> p, NodeChoice nc) {
  switch (nc) {
  case NodeChoice::LEFT:
    return p.first;

  case NodeChoice::RIGHT:
    return p.second;
    break;

  default:
    throw std::invalid_argument(
        "Encountered an unexpected case. Must be either LEFT or RIGHT");
  }
}

template <size_t N>
class FixedString {
private:
  std::array<char, N> data;

public:
  constexpr FixedString (const char (&cstr)[N + 1]) {
    std::memcpy(data.data(), cstr, N);
  }

  FixedString(const std::string & str) {
    if (str.size() != 3) {
      throw std::invalid_argument("String must have size 3");
    }
    std::copy(str.begin(), str.end(), data.begin());
  }

  constexpr std::string_view view() const noexcept {
    return static_cast<std::string_view>(*this);
  }

  constexpr explicit operator std::string_view() const noexcept {
    return {data.data(), N};
  }

  constexpr bool operator<(const FixedString &other) const {
    return data < other.data;
  }
};

template <typename T>
struct LinkedList {
  T data;
  LinkedList * next;
};

void solve(std::string path) {
  using FStr = FixedString<3>;

  FileParser fp(path);
  auto const lines = fp.getLines();
  auto line = lines.cbegin();

  std::map<FStr, std::pair<FStr, FStr>> graph;

  std::vector<char> strRules(line->cbegin(), line->cend());
  LinkedList<NodeChoice> leadRule = {parseChoice(strRules.at(0)), nullptr};
  LinkedList<NodeChoice> * nextRule = &leadRule;
  for (
    auto ruleIt = strRules.cbegin() + 1;
    ruleIt != strRules.cend(); ++ruleIt
  ) {
    nextRule->next = new LinkedList<NodeChoice>{parseChoice(*ruleIt), nullptr};
    nextRule = nextRule->next;
  }
  nextRule->next = &leadRule;
  line += 2;

  for (; line != lines.cend(); ++line) {
    auto tokens = parseWords(*line);
    graph.insert({tokens.at(0), {tokens.at(1), tokens.at(2)}});
  }

  nextRule = &leadRule;
  size_t steps = 0;
  constexpr std::string_view const END_MATCH = "ZZZ";

  if (graph.begin() != graph.end()) {
    for (
      auto here = graph.begin()->first;
      here.view().compare(END_MATCH);
      here = chooseNode(graph.at(here), nextRule->data),
      ++steps, nextRule = nextRule->next
    ) {

    }
  }

  std::cout << "P1: " << steps << std::endl;

  for (
    LinkedList<NodeChoice> * ruleIt = leadRule.next, * delIt = nullptr;
    ruleIt != nullptr && ruleIt != &leadRule;
    delIt = ruleIt, ruleIt = ruleIt->next, delete delIt
  ) { }
}

} // anon namespace

void y2023d08(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d08);
}
