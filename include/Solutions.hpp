#pragma once

#include <map>
#include <string>

#include "Common.hpp"
#include "ArgParser.hpp"

// reference for singleton: https://stackoverflow.com/a/1008289

/// @brief Lets you choose what file is to be run by the solver
enum class PuzzleChoice{
  RUN_ALL,
  RUN_INPUT,
  RUN_ALL_INPUTS,
  RUN_EXAMPLE,
  RUN_ALL_EXAMPLES
};

constexpr const PuzzleChoice DEFAULT_CHOICE = PuzzleChoice::RUN_INPUT;

class Solutions {
private:
  Solutions();
  std::map<std::pair<Year, Day>, VoidFunc> mFuncs;
  static std::string rootPath;

public:
  Solutions(Solutions const &) = delete;
  void operator=(Solutions const &) = delete;

  static Solutions &getInstance();
  static Solutions &getInstance(System::ArgParser args);

  void addEntry(VoidFunc func, Year year, Day day);
  bool runEntry(Year year, Day day, PuzzleChoice pc = DEFAULT_CHOICE) const;
};
