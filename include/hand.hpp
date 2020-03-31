#ifndef HAND_HPP
#define HAND_HPP

#include "card.hpp"
#include <vector>
#include <string>

using namespace std;

class Hand
{
public:
    Hand();
    ~Hand();
    void addCard(Card card);
    void removeCard(Card rmCard);
    void sortHand();
    void calcHandValue();
    void findHighCard();
    bool isOverMaxSize();

    int highCardValue = 0;
    int handSize = 0;
    int handValue = 0;
    vector<Card> cards;

private:
    int maxHandSize = 5;
};

#endif
