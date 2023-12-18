#pragma once

#include <map>

#include "Common.hpp"

// reference for singleton: https://stackoverflow.com/a/1008289

class Solutions {
private:
  Solutions();
  std::map<std::pair<Year, Day>, VoidFunc> mFuncs;

public:
  Solutions(Solutions const &) = delete;
  void operator=(Solutions const &) = delete;

  static Solutions &getInstance();

  void addEntry(VoidFunc func, Year year, Day day);
  bool runEntry(Year year, Day day) const;

};
