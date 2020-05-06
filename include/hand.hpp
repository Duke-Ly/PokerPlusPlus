#ifndef HAND_HPP
#define HAND_HPP

#include <vector>
#include <string>
#include "card.hpp"

using namespace std;

class Hand
{
public:
    Hand();
    ~Hand();
    void addCard(Card& card);
    void removeCard(Card& rmCard);
    void sortHand();
    void calcHandValue();
    void findHighCard();
    bool isMaxSize();
    vector<string> cardsVector();

    int highCardValue = 0;
    int handSize = 0;
    int handValue = 0;
    vector<Card> cards;
    vector<Card> default_cards;

private:
    unsigned int maxHandSize = 5;
};

#endif
