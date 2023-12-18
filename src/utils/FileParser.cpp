#include "FileParser.hpp"

#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>

FileParser::FileParser(std::string path) : mPath(path) {
  std::stringstream msg;
  if (!std::filesystem::exists(mPath)) {
    msg << "Path does not exist: " << mPath;
    throw std::invalid_argument(msg.str());
  } else if (std::filesystem::is_directory(mPath)) {
    msg << "Path is not a file: " << mPath;
    throw std::invalid_argument(msg.str());
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
