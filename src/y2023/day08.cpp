#include "EntryHeaders.hpp"
#include "StringUtils.hpp"
#include <array>
#include <cstring> // memcpy
#include <exception>
#include <map>
#include <string_view>
#include <utility> // pair
#include <vector>
#include <numeric> // lcm

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

template <class T>
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

template <class T>
struct LinkedList {
  T data;
  LinkedList * next;
};

struct BranchChecker {
  size_t cycleSize;
  bool isDone;
};

size_t countSteps(const System::vecstring & lines) {
  using FStr = FixedString<3>;
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
    ) { }
  }

  for (
    LinkedList<NodeChoice> * ruleIt = leadRule.next, * delIt = nullptr;
    ruleIt != nullptr && ruleIt != &leadRule;
    delIt = ruleIt, ruleIt = ruleIt->next, delete delIt
  ) { }

  return steps;
}

size_t countMultiSteps(const System::vecstring & lines) {
  using FStr = FixedString<3>;
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
    auto tokens = parseWordNums(*line);
    graph.insert({tokens.at(0), {tokens.at(1), tokens.at(2)}});
  }

  constexpr char const BEG_MATCH = 'A';
  constexpr char const END_MATCH = 'Z';

  std::vector<std::pair<FStr, BranchChecker>> branches;
  std::map<FStr, BranchChecker> multiSteps;

  for (auto const & node: graph) {
    if (node.first.view().back() == BEG_MATCH) {
      branches.push_back({node.first, {0, false}});
    }
  }

  const size_t MAX_GOALS = branches.size();
  size_t goalCount = 0;

  for (
    auto thisRule = &leadRule;
    goalCount < MAX_GOALS;
    thisRule = thisRule->next
  ) {
    for (auto && branch: branches) {
      if (branch.second.isDone) {
        continue;
      }

      branch.first = chooseNode(graph.at(branch.first), thisRule->data);
      ++(branch.second.cycleSize);
      branch.second.isDone = branch.first.view().back() == END_MATCH;
      if (branch.second.isDone) {
        ++goalCount;
      }
    }
  }

  size_t steps = 1;

  for (auto const & branch: branches) {
    steps = std::lcm(steps, branch.second.cycleSize);
  }

  for (
    LinkedList<NodeChoice> * ruleIt = leadRule.next, * delIt = nullptr;
    ruleIt != nullptr && ruleIt != &leadRule;
    delIt = ruleIt, ruleIt = ruleIt->next, delete delIt
  ) { }

  return steps;
}

void solve(std::string path) {
  FileParser fp(path);
  auto const lines = fp.getLines();

  if (path.find("-p2") == path.npos) {
    std::cout << "P1: " << countSteps(lines) << std::endl;
  } else {
    std::cout << "P1: incompatible input [SKIPPED]" << std::endl;
  }

  std::cout << "P2: " << countMultiSteps(lines) << std::endl;
}

} // anon namespace

void y2023d08(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d08);
}
