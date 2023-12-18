#pragma once

#include <string>

using VoidFunc = void (*)(std::string path);

enum class Year {
  none = 0,
  y2023 = 2023
};

enum class Day {
  none = 0,
  d01,
  d02,
  d03,
  d04,
  d05,
  d06,
  d07,
  d08,
  d09,
  d10,
  d11,
  d12,
  d13,
  d14,
  d15,
  d16,
  d17,
  d18,
  d19,
  d20,
  d21,
  d22,
  d23,
  d24,
  d25,
};

Year parseYear(std::string yearText);

Day parseDay(std::string dayText);
