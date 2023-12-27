#include "Solutions.hpp"
#include "DailySetup.hpp"
#include <exception>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

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
  if (mFuncs.find({y, d}) != mFuncs.end()) {
    throw std::logic_error("You are trying to enter the same entry twice");
  }
  mFuncs.insert({{y, d}, f});
}

static void singleDayInserter(
    const std::map<std::pair<Year, Day>, VoidFunc>::const_iterator &payload,
    const std::string &rootPath, const std::string &filename,
    std::vector<std::pair<VoidFunc, std::string>> &subset) {
  std::stringstream fullpath;

  auto year = payload->first.first;
  auto day = payload->first.second;
  auto functor = payload->second;

  fullpath << rootPath;
  fullpath << std::setw(0) << static_cast<int>(year) << "/";
  fullpath << std::setfill('0') << std::setw(2) << std::right;
  fullpath << static_cast<int>(day) << "/" << filename;
  subset.push_back({functor, fullpath.str()});
}

static void multiDayInserter(
    const std::map<std::pair<Year, Day>, VoidFunc>::const_iterator &payload,
    const std::string &rootPath, const std::regex & pattern,
    std::vector<std::pair<VoidFunc, std::string>> &subset) {
  std::stringstream dirpath;

  auto year = payload->first.first;
  auto day = payload->first.second;
  auto functor = payload->second;
  (void)functor;
  (void)subset;
  (void)pattern;

  dirpath << rootPath;
  dirpath << std::setw(0) << static_cast<int>(year) << "/";
  dirpath << std::setfill('0') << std::setw(2) << std::right;
  dirpath << static_cast<int>(day);

  // ref: https://stackoverflow.com/a/612176
  for (const auto &entry :
       std::filesystem::directory_iterator(dirpath.str())) {
    if (auto fullpath = entry.path().string(); std::regex_match(fullpath, pattern)) {
      subset.push_back({functor, fullpath});
    }
  }
}

bool Solutions::runEntry(Year year, Day day, PuzzleChoice pc) const {
  // pc has default of RUN_INPUT

  // (\w+),?
  // case PuzzleChoice::$1:\n    break;

  std::vector<std::pair<VoidFunc, std::string>> subset;

  switch (pc) {
  case PuzzleChoice::RUN_ALL:
    if (auto it = mFuncs.find({year, day}); it != mFuncs.end()) {
      const std::regex rxPuzzles(".+(?:in|ex).txt$");
      multiDayInserter(it, rootPath, rxPuzzles, subset);
    }
    break;

  case PuzzleChoice::RUN_INPUT:
    if (auto it = mFuncs.find({year, day}); it != mFuncs.end()) {
      singleDayInserter(it, rootPath, "data.in.txt", subset);
    }
    break;

  case PuzzleChoice::RUN_ALL_INPUTS:
    if (auto it = mFuncs.find({year, day}); it != mFuncs.end()) {
      const std::regex rxPuzzles(".+in.txt$");
      multiDayInserter(it, rootPath, rxPuzzles, subset);
    }
    break;

  case PuzzleChoice::RUN_EXAMPLE:
    if (auto it = mFuncs.find({year, day}); it != mFuncs.end()) {
      singleDayInserter(it, rootPath, "data.ex.txt", subset);
    }
    break;

  case PuzzleChoice::RUN_ALL_EXAMPLES:
    if (auto it = mFuncs.find({year, day}); it != mFuncs.end()) {
      const std::regex rxPuzzles(".+ex.txt$");
      multiDayInserter(it, rootPath, rxPuzzles, subset);
    }
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
    std::cout << std::endl;
  }
  return !subset.empty();
}
