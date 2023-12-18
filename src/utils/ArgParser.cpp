#include "ArgParser.hpp"

namespace System {
ArgParser::ArgParser(int argc, char const *argv[]) :m_args() {
  if (argc == 1) {
    return;
  }
  std::string keys = "";
  vecstring params;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      m_args.emplace(keys, params);
      params.clear();
      keys = std::string(argv[i]);
    } else {
      params.push_back(std::string(argv[i]));
    }
  }
  m_args.emplace(keys, params);
}

std::string ArgParser::findFirst(std::string flag) {
  if (hasFlag(flag)) {
    return m_args.at(flag).front();
  }
  return std::string();
}

vecstring ArgParser::findParams(std::string flag) {
  if (hasFlag(flag)) {
    return m_args.at(flag);
  }
  return vecstring();
}

bool ArgParser::hasFlag(std::string flag) {
  m_matched = m_args.find(flag) != m_args.end();
  return m_matched;
}

} // namespace System