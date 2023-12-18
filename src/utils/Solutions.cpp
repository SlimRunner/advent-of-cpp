#include "Solutions.hpp"
#include "DailySetup.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>

std::string Solutions::rootPath = "";

Solutions::Solutions() : mFuncs{} {
  collectSetups(*this);
}

Solutions &Solutions::getInstance() {
  static Solutions instance;
  return instance;
}

Solutions &Solutions::getInstance(System::ArgParser args) {
  rootPath = "./puzzles/";
  if (args.empty()) {
    
  } else {
    // this should not happen here, but in main
    // if (
    //   System::vecstring param;
    //   (args.findParams("r", param) || args.findParams("-run", param)) &&
    //   param.size() == 2
    // ){
    //   auto year = parseYear(param.at(0));
    //   auto day = parseDay(param.at(1));
    // } else if (
    //   (args.findParams("b", param) || args.findParams("-batch", param)) &&
    //   param.size() == 2
    // ) {
    //   param;
    // }

    if (
      std::string customPath;
      args.findFirst("p", customPath) || args.findFirst("-path", customPath)
    ){
      rootPath = customPath;
    }
  }

  return getInstance();
}

void Solutions::addEntry(VoidFunc f, Year y, Day d) {
  mFuncs.insert({{y, d}, f});
}

bool Solutions::runEntry(Year year, Day day, PuzzleChoice pc) const {
  // pc has default of RUN_INPUT

  // (\w+),?
  // case PuzzleChoice::$1:\n    break;

  std::vector<std::pair<VoidFunc, std::string>> subset;
  std::stringstream fullpath;

  switch (pc) {
  case PuzzleChoice::RUN_ALL:
    break;
  case PuzzleChoice::RUN_INPUT:
    if (auto it = mFuncs.find({year, day}); it != mFuncs.end()) {
      fullpath.clear();
      fullpath << rootPath;
      fullpath << std::setw(0) << static_cast<int>(year) << "/";
      fullpath << std::setfill('0') << std::setw(2);
      fullpath << static_cast<int>(day) << "/data.in.txt";
      subset.push_back({it->second, fullpath.str()});
    }
    
    break;
  case PuzzleChoice::RUN_ALL_INPUTS:
    break;
  case PuzzleChoice::RUN_EXAMPLE:
    if (auto it = mFuncs.find({year, day}); it != mFuncs.end()) {
      fullpath.clear();
      fullpath << rootPath;
      fullpath << std::setw(0) << static_cast<int>(year) << "/";
      fullpath << std::setfill('0') << std::setw(2) << std::right;
      fullpath << static_cast<int>(day) << "/data.ex.txt";
      subset.push_back({it->second, fullpath.str()});
    }
    break;
  case PuzzleChoice::RUN_ALL_EXAMPLES:
    break;
  default:
    break;
  }

  std::stringstream mainMsg;
  
  mainMsg << "== Day " << static_cast<int>(day);
  mainMsg << " Year " << static_cast<int>(year) << " ";
  std::cout << std::setfill('=') << std::setw(50) << std::left << mainMsg.str();
  std::cout << std::endl;
  for (auto const& fset: subset) {
    std::cout << "File: " << fset.second << std::endl;
    fset.first(fset.second);
  }
  std::cout << std::endl;
  return !subset.empty();
}
