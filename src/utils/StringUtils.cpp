#include "StringUtils.hpp"

#include <sstream>

std::string stringBetween(const std::string & src, char start, char end) {
  size_t p0, p1;
  p0 = src.find(start) + 1;
  p1 = src.find(end) - 1;
  if (p0 == src.npos || p1 == src.npos || p0 >= p1) {
    return std::string{};
  } else {
    return src.substr(p0, p1 - p0);
  }
}

std::string stringBefore(const std::string & src, char end) {
  size_t p1;
  p1 = src.find(end) - 1;
  if (p1 == src.npos || p1 <= 0) {
    return std::string{};
  } else {
    return src.substr(0, p1);
  }
}

std::string stringAfter(const std::string & src, char start) {
  size_t p0;
  p0 = src.find(start) + 1;
  if (p0 == src.npos || p0 >= src.length()) {
    return std::string{};
  } else {
    return src.substr(p0);
  }
}

std::vector<int> parseInts(const std::string & src) {
  std::vector<int> result;
  std::stringstream buffer;
  bool enableBuffer = false;

  for (auto const & chr: src) {
    if (std::isdigit(chr)) {
      buffer << chr;
      enableBuffer = true;
    } else if (enableBuffer) {
      result.push_back(std::stoi(buffer.str()));
      buffer.clear();
      buffer.str("");
      enableBuffer = false;
    }
  }

  if (enableBuffer) {
    result.push_back(std::stoi(buffer.str()));
  }

  return result;
}