#include "EntryHeaders.hpp"
#include "StringUtils.hpp"
#include <algorithm> // sort
#include <array>
#include <exception>
#include <map>
#include <sstream> // stringstream
#include <vector>

namespace {

enum class RuleType {
  NORMAL,
  JOKER
};

enum class CardKind {
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

enum class HandKind {
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_KIND,
  FULL_HOUSE,
  FOUR_KIND,
  FIVE_KIND,
};

using handOfCards = std::array<CardKind, 5U>;

char cardToChar(CardKind cval);
CardKind charToCard(char chr);
int cardRank(CardKind cval);
int handRank(HandKind hand);
HandKind parseHand(size_t unique, int mode);
HandKind parseJokeHand(HandKind handKind, size_t jokers);
std::string getHandName(HandKind hand);

struct Hand {
  int bid;
  HandKind hand;
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

int getWinnings(const System::vecstring & lines, RuleType rt) {
  using cval = CardKind;

  std::vector<Hand> hands;

  for (auto const & line: lines) {
    std::map<cval, int> cardBins;

    handOfCards currentHand;
    auto handIt = currentHand.begin();

    int maxSize = 0;
    size_t jokerCount = 0;
    auto cards = stringBefore(line, ' ');

    for (auto const & chr: cards) {
      if (rt == RuleType::JOKER && chr == 'J') {
        *handIt = charToCard('*');
      } else {
        *handIt = charToCard(chr);
      }

      if (*handIt == CardKind::JOKER) {
        ++jokerCount;
      } else if (
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

    if (rt == RuleType::JOKER) {
      if (maxSize == 0 && jokerCount == 5) {
        maxSize = 1;
      }
      hands.push_back({
        std::stoi(stringAfter(line, ' ')),
        parseJokeHand(parseHand(cardBins.size() + jokerCount, maxSize), jokerCount),
        currentHand
      });
    } else {
      hands.push_back({
        std::stoi(stringAfter(line, ' ')),
        parseHand(cardBins.size(), maxSize),
        currentHand
      });
    }
  }
  
  int winnings = 0;
  std::sort(hands.begin(), hands.end());
  const size_t HAND_SIZE = hands.size();
  
  for (size_t i = 0; i < HAND_SIZE; ++i) {
    winnings += hands.at(i).bid * (static_cast<int>(i) + 1);
  }

  return winnings;
}

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();

  std::cout << "P1: " << getWinnings(lines, RuleType::NORMAL) << std::endl;
  std::cout << "P2: " << getWinnings(lines, RuleType::JOKER) << std::endl;
}

[[maybe_unused]]
char cardToChar(CardKind cval) {
  using CV = CardKind;
  switch (cval) {
  case CV::JOKER:
    return '*';
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

CardKind charToCard(char chr) {
  using CV = CardKind;
  switch (chr) {
  case '*':
    return CV::JOKER;
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

int cardRank(CardKind cval) {
  return static_cast<int>(cval);
}

int handRank(HandKind hand) {
  return static_cast<int>(hand);
}

HandKind parseHand(size_t unique, int mode) {
  if        (unique == 1 && mode == 5) {
    return HandKind::FIVE_KIND;
  } else if (unique == 2 && mode == 4) {
    return HandKind::FOUR_KIND;
  } else if (unique == 2 && mode == 3) {
    return HandKind::FULL_HOUSE;
  } else if (unique == 3 && mode == 3) {
    return HandKind::THREE_KIND;
  } else if (unique == 3 && mode == 2) {
    return HandKind::TWO_PAIR;
  } else if (unique == 4 && mode == 2) {
    return HandKind::ONE_PAIR;
  } else if (unique == 5 && mode == 1) {
    return HandKind::HIGH_CARD;
  } else {
    std::stringstream msg;
    msg << "This configuration is not possible.";
    msg << "received: " << unique << "unique";
    msg << " and " << mode << " as mode.";
    throw std::logic_error(msg.str());
  }
}

HandKind parseJokeHand(HandKind handKind, size_t jokers) {
  // TODO: do the number of jokers uniquely transform a kind into another one?
  // Or do I have to know the cards necessarily?
  using HV = HandKind;
  switch (handKind) {
  case HV::HIGH_CARD:
    switch (jokers) {
    case 0U:
      return HV::HIGH_CARD;
    case 1U:
      return HV::ONE_PAIR;
    case 2U:
      return HV::THREE_KIND;
    case 3U:
      return HV::FOUR_KIND;
    case 4U:
      return HV::FIVE_KIND;
    case 5U:
      return HV::FIVE_KIND;
    default:
      throw std::invalid_argument("Unexpected number of jokers");
    }
  case HV::ONE_PAIR:
    switch (jokers) {
    case 0U:
      return HV::ONE_PAIR;
    case 1U:
      return HV::THREE_KIND;
    case 2U:
      return HV::FOUR_KIND;
    case 3U:
      return HV::FIVE_KIND;
    default:
      throw std::invalid_argument("Unexpected number of jokers");
    }
  case HV::TWO_PAIR:
    switch (jokers) {
    case 0U:
      return HV::TWO_PAIR;
    case 1U:
      return HV::FULL_HOUSE;
    default:
      throw std::invalid_argument("Unexpected number of jokers");
    }
  case HV::THREE_KIND:
    switch (jokers) {
    case 0U:
      return HV::THREE_KIND;
    case 1U:
      return HV::FOUR_KIND;
    case 2U:
      return HV::FIVE_KIND;
    default:
      throw std::invalid_argument("Unexpected number of jokers");
    }
  case HV::FULL_HOUSE:
    switch (jokers) {
    case 0U:
      return HV::FULL_HOUSE;
    default:
      throw std::invalid_argument("Unexpected number of jokers");
    }
  case HV::FOUR_KIND:
    switch (jokers) {
    case 0U:
      return HV::FOUR_KIND;
    case 1U:
      return HV::FIVE_KIND;
    default:
      throw std::invalid_argument("Unexpected number of jokers");
    }
  case HV::FIVE_KIND:
    switch (jokers) {
    case 0U:
      return HV::FIVE_KIND;
    default:
      throw std::invalid_argument("Unexpected number of jokers");
    }
  default:
    throw std::invalid_argument("Received invalid kind of hand");
  }
}

[[maybe_unused]]
std::string getHandName(HandKind hand) {
  using HV = HandKind;
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
