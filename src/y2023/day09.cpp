#include "EntryHeaders.hpp"
#include "StringUtils.hpp"
#include <exception>
#include <utility>
#include <numeric>

namespace {

template <class T>
T newtNCR(std::vector<T> x,T n) {
  T num = 1, denom = 1;
  T accumulator = x.front();
  T r = static_cast<T>(x.size()) - 1;

  for (T i = 0; i < r; ++i) {
    num *= n - i;
    denom *= (i + 1);

    // reduce the chance of overflow
    if (num % denom == 0) {
      num /= denom;
      denom = 1;
    }

    accumulator += x.at(static_cast<size_t>(i + 1)) * num / denom;
  }
  
  return accumulator;
}

template <class T>
std::vector<T> findDiffTerms(const std::vector<T> & seq) {
  if (seq.size() < 2) {
    throw std::invalid_argument("Expected a vector of size 2 or larger.");
  }
  std::vector<std::vector<T>> deltas;
  auto seqnum = seq.cbegin();

  // based on this
  // https://youtu.be/4AuV93LOPcE?si=Z45ygqksAQ_Yz4EQ&t=996

  do {
    deltas.push_back({});
    const size_t DEGREE = deltas.size();

    for (size_t i = 0; i < DEGREE; ++i) {
      if (!i) {
        deltas.at(i).push_back(*(seqnum++));
      } else {
        auto lastNum = deltas.at(i - 1).cend() - 1;
        deltas.at(i).push_back(*lastNum - *(lastNum - 1));
      }
    }
  } while (
    deltas.size() < 2 ||
    seqnum != seq.cend()
  );

  std::vector<T> leadingTerms;

  for (auto const & level: deltas) {
    leadingTerms.push_back(level.front());
  }
  
  return leadingTerms;
}

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();

  long long guessOfNext = 0;
  long long guessOfPast = 0;

  for (auto const & line: lines) {
    auto nums = parseLLs(line);
    auto terms = findDiffTerms(nums);
    const size_t NTH_TERM = nums.size();
    long long nextInSeq = newtNCR(terms, static_cast<long long>(NTH_TERM));
    long long prevInSeq = newtNCR(terms, -1LL);
    guessOfNext += nextInSeq;
    guessOfPast += prevInSeq;
  }
  
  std::cout << "P1: " << guessOfNext << std::endl;
  std::cout << "P2: " << guessOfPast << std::endl;
}

} // anon namespace

void y2023d09(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d09);
}
