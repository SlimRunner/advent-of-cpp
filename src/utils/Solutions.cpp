#include "Solutions.hpp"
#include "DailySetup.hpp"

Solutions::Solutions() : mFuncs{} {
  y2023d01(*this);
}

Solutions &Solutions::getInstance() {
  static Solutions instance;
  return instance;
}

void Solutions::addEntry(VoidFunc f, Year y, Day d) {
  mFuncs.insert({{y, d}, f});
}

bool Solutions::runEntry(Year year, Day day) const {
  auto itFunc = mFuncs.find({year, day});
  if (itFunc != mFuncs.end()) {
    itFunc->second();
    return true;
  }
  return false;
}
