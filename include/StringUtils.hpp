#pragma once

#include <string>
#include <vector>

std::string stringBetween(const std::string & src, char start, char end);
std::string stringBefore(const std::string & src, char end);
std::string stringAfter(const std::string & src, char start);

std::vector<int> parseInts(const std::string & src);
