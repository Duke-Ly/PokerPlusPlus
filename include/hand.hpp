#ifndef HAND_HPP
#define HAND_HPP

#include <iostream>
#include "card.hpp"
#include "dealer.hpp"

class Hand
{
  public:
      Hand();
      ~Hand();
      void addCard(Card card);
      void removeCard(std::vector<Card> rmCards);
      void sortHand();
      void calcHandValue();

      bool isOverMaxSize;
      int handSize = 0;
      int handValue = -1;
      std::vector<Card> cards;

  private:
      int maxHandSize = 5;
};

#endif
