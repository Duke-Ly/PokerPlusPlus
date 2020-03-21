#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>

enum cardSuit {DIAMONDS, HEARTS, CLUBS, SPADES};

class Card
{
  public:
      Card(int value, cardSuit suit);
      ~Card();

      int value;
      cardSuit suit;
};

#endif
