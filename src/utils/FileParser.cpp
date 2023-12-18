#include "FileParser.hpp"

#include <exception>
#include <filesystem>
#include <fstream>

FileParser::FileParser(std::string path) : mPath(path) {
  if (!std::filesystem::exists(mPath)) {
    throw std::invalid_argument("Path does not exist");
  }
  // the file can stop existing after this constructor asserts that it exists,
  // but the run time of this code is short enough that such possibility has
  // little importance
}

std::vector<std::string> FileParser::getLines() {
  std::ifstream infile(mPath);
  std::vector<std::string> output;

  for (std::string line; std::getline(infile, line); /* NA */) {
    output.push_back(line);
  }

  return output;
}
