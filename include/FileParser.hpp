#pragma once

#include <string>
#include <vector>

class FileParser {
private:
  std::string mPath;
public:
  FileParser() = delete;
  FileParser(std::string);

  std::vector<std::string> getLines();
};
