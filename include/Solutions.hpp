#pragma once

#include <map>
#include <string>

#include "ArgParser.hpp"
#include "Common.hpp"

// reference for singleton: https://stackoverflow.com/a/1008289

/// @brief Lets you choose what file is to be run by the solver
enum class PuzzleChoice {
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
  Solutions(Solutions const &) = delete;
  Solutions &operator=(const Solutions &) = delete;

  static void setupInstance(const System::ArgParser &args);

  std::map<std::pair<Year, Day>, VoidFunc> mFuncs;
  static std::string rootPath;

public:
  static Solutions &getInstance(const System::ArgParser &args);

  void addEntry(VoidFunc func, Year year, Day day);
  bool runEntry(Year year, Day day, PuzzleChoice pc = DEFAULT_CHOICE) const;
};
