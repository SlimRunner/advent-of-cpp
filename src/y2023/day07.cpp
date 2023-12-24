#include "EntryHeaders.hpp"
#include "StringUtils.hpp"
#include <algorithm> // sort
#include <array>
#include <exception>
#include <map>
#include <sstream> // stringstream
#include <vector>

namespace {

enum class CardValues {
  JOKER,
  N2,
  N3,
  N4,
  N5,
  N6,
  N7,
  N8,
  N9,
  T,
  J,
  Q,
  K,
  A,
};

enum class HandValues {
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_KIND,
  FULL_HOUSE,
  FOUR_KIND,
  FIVE_KIND,
};

using handOfCards = std::array<CardValues, 5U>;

char cardToChar(CardValues cval);
CardValues charToCard(char chr);
int cardRank(CardValues cval);
int handRank(HandValues hand);
HandValues parseHand(size_t unique, int mode);
std::string getHandName(HandValues hand);

struct Hand {
  int bid;
  HandValues hand;
  handOfCards cards;

  bool operator<(const Hand & rhs) const {
    auto h1 = handRank(hand);
    auto h2 = handRank(rhs.hand);
    if (h1 < h2) {
      return true;
    } else if (h1 > h2) {
      return false;
    } else {
      for (
        auto c1 = cards.cbegin(), c2 = rhs.cards.cbegin();
        c1 != cards.cend() && c2 != rhs.cards.cend();
        ++c1, ++c2
      ) {
        if (*c1 != *c2) {
          return cardRank(*c1) < cardRank(*c2);
        }
      }
      throw std::logic_error("Hand kinds should not be ambiguous");
    }
  }
};

void normalRules(const System::vecstring & lines) {
  using cval = CardValues;

  std::vector<Hand> hands;

  for (auto const & line: lines) {
    std::map<cval, int> cardBins;

    handOfCards currentHand;
    auto handIt = currentHand.begin();

    int maxSize = 0;
    auto cards = stringBefore(line, ' ');

    for (auto const & chr: stringBefore(line, ' ')) {
      *handIt = charToCard(chr);
      if (
        auto nthCard = *handIt;
        cardBins.find(nthCard) == cardBins.end()
      ) {
        cardBins.emplace(nthCard, 1);
        maxSize = maxSize < 1 ? 1 : maxSize;
      } else {
        auto & here = cardBins.at(nthCard);
        ++here;
        maxSize = maxSize < here ? here : maxSize;
      }
      if (handIt == currentHand.end()) {
        throw std::logic_error("Input was asserted to have only five cards per hand");
      }
      ++handIt;
    }

    hands.push_back({
      std::stoi(stringAfter(line, ' ')),
      parseHand(cardBins.size(), maxSize),
      currentHand
    });
  }
  
  int winnings = 0;
  std::sort(hands.begin(), hands.end());
  const size_t HAND_SIZE = hands.size();
  
  for (size_t i = 0; i < HAND_SIZE; ++i) {
    winnings += hands.at(i).bid * (static_cast<int>(i) + 1);
  }

  std::cout << "P1: " << winnings << std::endl;
}

// void jokerRules(const System::vecstring & lines) {
  
// }

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();

  normalRules(lines);
  // jokerRules(lines);
}

[[maybe_unused]]
char cardToChar(CardValues cval) {
  using CV = CardValues;
  switch (cval) {
  case CV::N2:
    return '2';
  case CV::N3:
    return '3';
  case CV::N4:
    return '4';
  case CV::N5:
    return '5';
  case CV::N6:
    return '6';
  case CV::N7:
    return '7';
  case CV::N8:
    return '8';
  case CV::N9:
    return '9';
  case CV::T:
    return 'T';
  case CV::J:
    return 'J';
  case CV::Q:
    return 'Q';
  case CV::K:
    return 'K';
  case CV::A:
    return 'A';
  default:
    throw std::invalid_argument("Received invalid card value");
  }
}

CardValues charToCard(char chr) {
  using CV = CardValues;
  switch (chr) {
  case '2':
    return CV::N2;
  case '3':
    return CV::N3;
  case '4':
    return CV::N4;
  case '5':
    return CV::N5;
  case '6':
    return CV::N6;
  case '7':
    return CV::N7;
  case '8':
    return CV::N8;
  case '9':
    return CV::N9;
  case 'T':
    return CV::T;
  case 'J':
    return CV::J;
  case 'Q':
    return CV::Q;
  case 'K':
    return CV::K;
  case 'A':
    return CV::A;
  default:
    throw std::invalid_argument("Received invalid character for card value");
  }
}

int cardRank(CardValues cval) {
  return static_cast<int>(cval);
}

int handRank(HandValues hand) {
  return static_cast<int>(hand);
}

HandValues parseHand(size_t unique, int mode) {
  // UC: unique cards
  // LB: largest bin
  // if UC = 1 and LB = 5 -> Five of a kind
  // if UC = 2 and LB = 4 -> Four of a kind
  // if UC = 2 and LB = 3 -> Full house
  // if UC = 3 and LB = 3 -> Three of a kind
  // if UC = 3 and LB = 2 -> Two pair
  // if UC = 4 and LB = 2 -> One pair
  // if UC = 5 and LB = 1 -> High card
  if (unique == 1 && mode == 5) {
    return HandValues::FIVE_KIND;
  } else if (unique == 2 && mode == 4) {
    return HandValues::FOUR_KIND;
  } else if (unique == 2 && mode == 3) {
    return HandValues::FULL_HOUSE;
  } else if (unique == 3 && mode == 3) {
    return HandValues::THREE_KIND;
  } else if (unique == 3 && mode == 2) {
    return HandValues::TWO_PAIR;
  } else if (unique == 4 && mode == 2) {
    return HandValues::ONE_PAIR;
  } else if (unique == 5 && mode == 1) {
    return HandValues::HIGH_CARD;
  } else {
    std::stringstream msg;
    msg << "This configuration is not possible.";
    msg << "received: " << unique << "unique";
    msg << " and " << mode << " as mode.";
    throw std::logic_error(msg.str());
  }
}

[[maybe_unused]]
std::string getHandName(HandValues hand) {
  using HV = HandValues;
  switch (hand) {
  case HV::HIGH_CARD:
    return "high card";
  case HV::ONE_PAIR:
    return "one pair";
  case HV::TWO_PAIR:
    return "two pair";
  case HV::THREE_KIND:
    return "three of a kind";
  case HV::FULL_HOUSE:
    return "full house";
  case HV::FOUR_KIND:
    return "four of a kind";
  case HV::FIVE_KIND:
    return "five of a kind";
  default:
    throw std::invalid_argument("Received invalid kind of hand");
  }
}

} // anon namespace

void y2023d07(Solutions & sol) {
  sol.addEntry(solve, Year::y2023, Day::d07);
}
