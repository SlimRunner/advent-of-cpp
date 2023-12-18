#include "Common.hpp"

Year parseYear(std::string yearText) {
  auto year = static_cast<Year>(std::stoi(yearText));
  switch (year) {
  case Year::y2023:
    return Year::y2023;

  default:
    return Year::none;
  }
}

Day parseDay(std::string dayText) {
  auto day = std::stoi(dayText);
  if (day < 1 || day > 25) {
    return Day::none;
  }
  return static_cast<Day>(day);
}