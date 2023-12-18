#pragma once

#include <map>
#include <string>
#include <vector>

namespace System {
using vecstring = std::vector<std::string>;
using argmap = std::map<std::string, vecstring>;

class ArgParser {
private:
  argmap m_args;
  bool m_matched;

public:
  ArgParser(/* args */) = delete;

  ArgParser(int argc, char const *argv[]);

  /// @brief get value of first argument of given flag
  /// @param flag argument flag to search for
  /// @return string of frist argument
  std::string findFirst(std::string flag);

  /// @brief get list of arguments of given flag
  /// @param flag argument flag to search for
  /// @return vector of arguments
  vecstring findParams(std::string flag);

  /// @brief determines if provided value exists as an argument flag
  /// @param flag argument flag to search for
  /// @return true if key was found, false otherwise
  bool hasFlag(std::string flag);
  
  /// @brief Determines the result of last search performed
  /// @return true if last search succeeded, false otherwise
  inline bool found() { return m_matched; };
};

} // namespace System