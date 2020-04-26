#ifndef CARD_HPP
#define CARD_HPP

#include <string>

using namespace std;

enum cardSuit {DIAMONDS, HEARTS, CLUBS, SPADES};

class Card
{
public:
    Card(int value, cardSuit suit, string cardID)
        : value(value), suit(suit), cardID(cardID) {};
    ~Card() {};

    int value;
    cardSuit suit;
    string cardID;
};

#endif
