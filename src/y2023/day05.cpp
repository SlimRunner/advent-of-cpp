#include "EntryHeaders.hpp"
#include "StringUtils.hpp"
#include <algorithm>
#include <exception>
#include <limits>
#include <deque>

namespace {

enum class RangeJoinType {
  DISJOINT,
  ENCLOSED,
  OVERLAP_BACK,
  OVERLAP_FRONT,
  OVERLAP_THRU
};

template <typename T>
struct Range {
  T from;
  T to;

  bool probeRange(T value) const;
  RangeJoinType compareRanges(const Range<T> & rhs) const;
  inline bool operator < (const Range<T> & rhs);
  inline bool operator <= (const Range<T> & rhs);
};

template <typename T>
struct RangedOffset: public Range<T> {
  T offset;
};

template <typename T>
struct AlmanacEntry {
  T destStart;
  T sourceStart;
  T rangeSpan;

  AlmanacEntry(std::vector<T> values);
  RangedOffset<T> getRange() const;
};

template <typename T>
std::vector<RangedOffset<T>>
getRangeMap(const std::vector<AlmanacEntry<T>> &maps);
template <typename T>
std::vector<std::vector<RangedOffset<T>>>
getAlmanacMap(const std::vector<std::vector<AlmanacEntry<T>>> &almanac);
template <typename T>
void remapValues(const std::vector<RangedOffset<T>> &map, std::vector<T> &out);
template <typename T>
std::vector<Range<T>> remapRanges(const std::vector<RangedOffset<T>> &map,
                                  const std::vector<Range<T>> &src);

void solve(std::string path) {
  FileParser fp(path);
  auto const lines = fp.getLines();
  auto line = lines.cbegin();
  auto const IN_EOF = lines.cend();

  std::vector<std::vector<AlmanacEntry<long long>>> almanac;

  // populate almanac

  auto const parameters = parseLLs(stringAfter(*(line++), ':'));
  
  while (line != IN_EOF && line->size() == 0) {++line;}
  if (line != IN_EOF) ++line;
  
  while (line != IN_EOF) {
    almanac.push_back({});
    while (line != IN_EOF && line->size() > 0) {
      almanac.back().push_back(parseLLs(*line++));
    }
    while (line != IN_EOF && line->size() == 0) {++line;}
    if (line != IN_EOF) ++line;
  }

  // create range mappings

  auto mappings = getAlmanacMap(almanac);

  // remap all seed values to locations

  auto locations{parameters};

  for (auto maps: mappings) {
    remapValues(maps, locations);
  }

  // compute lowest location from list

  auto lowestLoc = std::numeric_limits<long long>().max();
  if (
    auto it = std::min_element(locations.begin(), locations.end());
    it != locations.end()
  ) {
    lowestLoc = *it;
  }

  std::cout << "P1: " << lowestLoc << std::endl;

  // compute seed ranges

  std::vector<Range<long long>> locRanges;
  
  for (auto it = parameters.begin(); it != parameters.end(); it += 2) {
    locRanges.push_back({*it, *it + *(it + 1) - 1});
  }
  
  // remap all seed ranges to location ranges
  
  for (auto maps: mappings) {
    locRanges = remapRanges(maps, locRanges);
  }

  // compute lowest location from location ranges

  lowestLoc = std::numeric_limits<long long>().max();

  if (
    auto it = std::min_element(locRanges.begin(), locRanges.end());
    it != locRanges.end()
  ) {
    lowestLoc = it->from;
  }
  
  std::cout << "P2: " << lowestLoc << std::endl;
}

template <typename T>
bool Range<T>::probeRange(T value) const {
  return from <= value && value <= to;
}

template <typename T>
RangeJoinType Range<T>::compareRanges(const Range<T> &rhs) const {
  // this: (), rhs <>
  // front () back

  if (from <= rhs.from && rhs.to <= to) {
    // ( < > )
    return RangeJoinType::ENCLOSED;
  } else if (rhs.from < from && to < rhs.to) {
    // < ( ) >
    return RangeJoinType::OVERLAP_THRU;
  } else if (from <= rhs.from && rhs.to <= to && to < rhs.to) {
    // ( < ) > 
    return RangeJoinType::OVERLAP_BACK;
  } else if (rhs.from < from && from <= rhs.to && rhs.to <= to) {
    // < ( > )
    return RangeJoinType::OVERLAP_FRONT;
  } else {
    // < > ( ) OR ( ) < >
    return RangeJoinType::DISJOINT;
  }
}

template <typename T>
inline bool Range<T>::operator<(const Range<T> &rhs) {
  return this->from < rhs.from;
}

template <typename T>
inline bool Range<T>::operator<=(const Range<T> &rhs) {
  return this->from <= rhs.from;
}

template<typename T>
AlmanacEntry<T>::AlmanacEntry(std::vector<T> values) {
  if (values.size() != 3) {
    throw std::invalid_argument("An almanac entry must be of size 3.");
  } else {
    destStart = values.at(0);
    sourceStart = values.at(1);
    rangeSpan = values.at(2);
  }
}

template <typename T>
RangedOffset<T> AlmanacEntry<T>::getRange() const {
  RangedOffset<T> result;
  result.from = sourceStart;
  result.to = sourceStart + rangeSpan - 1;
  result.offset = destStart - sourceStart;
  return result;
}

template <typename T>
std::vector<RangedOffset<T>> getRangeMap(const std::vector<AlmanacEntry<T>> &maps) {
  std::vector<RangedOffset<T>> ranges;

  // collect ranges

  for (const AlmanacEntry<T> & map: maps) {
    ranges.push_back(map.getRange());
  }

  std::sort(ranges.begin(), ranges.end());

  // compress like ranges

  T prevOffset;
  bool firstLoop = true;

  std::vector<RangedOffset<T>> outRanges;
  outRanges.reserve(ranges.size());

  for (const RangedOffset<T> & r: ranges) {
    if (firstLoop || r.offset != prevOffset) {
      outRanges.push_back(r);
      firstLoop = false;
    } else {
      outRanges.back().to = r.to;
    }
    prevOffset = r.offset;
  }

  return outRanges;
}

template <typename T>
std::vector<std::vector<RangedOffset<T>>>
getAlmanacMap(const std::vector<std::vector<AlmanacEntry<T>>> &almanac) {
  std::vector<std::vector<RangedOffset<T>>> result;
  for (const std::vector<AlmanacEntry<T>> & maps: almanac) {
    result.push_back(getRangeMap(maps));
  }

  return result;
}

template <typename T>
void remapValues(const std::vector<RangedOffset<T>> &map, std::vector<T> & out) {
  // if ranges are guranteed to be sorted
  // a binary search would be more effective
  
  for (auto seedIt = out.begin(); seedIt != out.end(); ++seedIt) {
    for (const RangedOffset<T> & range: map) {
      if (range.probeRange(*seedIt)) {
        *seedIt += range.offset;
        break;
      }
    }
  }
}

template <typename T>
std::vector<Range<T>> remapRanges(const std::vector<RangedOffset<T>> &map,
                                 const std::vector<Range<T>> &src) {
  //
  std::deque<Range<T>> deque(src.begin(), src.end());
  std::vector<Range<T>> out;

  for (const RangedOffset<T> & range : map) {
    // for (Range<T> seedRange; !stack.empty();) {
    const size_t STACK_SIZE = deque.size();
    for (size_t _ = 0; _ < STACK_SIZE; ++_) {
      Range<T> seedRange = deque.back();
      deque.pop_back();

      switch (range.compareRanges(seedRange)) {
      case RangeJoinType::DISJOINT:
        deque.push_front(seedRange);
        break;

      case RangeJoinType::ENCLOSED:
        out.push_back({
          seedRange.from + range.offset,
          seedRange.to + range.offset,
        });
        break;

      case RangeJoinType::OVERLAP_BACK:
        // (<)>
        out.push_back({
          seedRange.from + range.offset,
          range.to + range.offset,
        });
        deque.push_front({
          range.to + 1,
          seedRange.to,
        });
        break;

      case RangeJoinType::OVERLAP_FRONT:
        // <(>)
        out.push_back({
          range.from + range.offset,
          seedRange.to + range.offset,
        });
        deque.push_front({
          seedRange.from,
          range.from - 1,
        });
        break;

      case RangeJoinType::OVERLAP_THRU:
        // <()>
        out.push_back({
          range.from + range.offset,
          range.to + range.offset,
        });
        deque.push_front({
          seedRange.from,
          range.from - 1,
        });
        deque.push_front({
          range.to + 1,
          seedRange.to,
        });
        break;

      default:
        throw std::invalid_argument("Unexpected enum type for RangeJoinType");
        break;
      }
    }
  }

  if (!deque.empty()) {
    out.insert(out.end(), deque.begin(), deque.end());
  }

  return out;
}

} // namespace

void y2023d05(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d05);
}
