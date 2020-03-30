#include "card.hpp"
#include <string>
#include <iostream>

using namespace std;

Card::Card(int value, cardSuit suit, string cardID)
{
    this->value = value;
    this->suit = suit;
    this->cardID = cardID;
    cout<<cardID<<" has been created."<<endl;
}

Card::~Card() {};
