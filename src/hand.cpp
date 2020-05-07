#include "hand.hpp"
#include "card.hpp"
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <map>

using namespace std;

Hand::Hand()
{
    Card defaultCard(0, DIAMONDS, "red_back");
    for(int i=0; i<5; i++)
    {
        cards.push_back(defaultCard);
        default_cards.push_back(defaultCard);
    }
    cout<<"A hand was created"<<endl;
}

Hand::~Hand()
{
    cards.clear();
    default_cards.clear();
}

void Hand::addCard(Card& card)
{
    cards.push_back(card);
    handSize++;
    cout<<card.cardID<<" had been added to hand."<<endl;
}

void Hand::removeCard(Card& rmCard)
{
    for(int i=0; i<5; ++i)
        if((rmCard.suit==cards[i].suit)&&(rmCard.value==cards[i].value))
        {
            cout<<cards[i].cardID<<" had been removed from hand."<<endl;
            cards.erase(cards.begin()+i);
            handSize--;
        }
}

void Hand::sortHand()
{
    sort(cards.begin(), cards.end(), [](const Card& lhs, const Card& rhs)
    {
        return lhs.value < rhs.value;
    });
    for(auto it = cards.begin(); it != cards.end(); it++)
        cout<<it->cardID<<endl;

    cout<<"Hand have been sorted"<<endl;
}

void Hand::calcHandValue()
{
    bool onePair = false, twoPair = false, threeOfaKind = false, straight = false, flush = false;
    bool fullHouse = false, fourOfaKind = false, straightFlush = false, royalFlush = false;
    int numPair = 0;

    // check for one pair or two pair
    map<int, int> M;
    for(auto & card : cards)
    {
        auto result = M.insert(std::pair<int, int>(card.value, 1));
        if (result.second == false)
            result.first->second++;
    }
    // Iterate over the map
    for (auto & elem : M)
    {
        // If frequency count is greater than 1 then its a duplicate element
        if (elem.second == 2)
        {
            numPair++;
        }
        if (elem.second == 3)
        {
            threeOfaKind = true;
        }
    }
/*
    for(int i=1; i<5; i++)
    {
        if((i<3)&&(cards[i].value==cards[i-1].value)&&(cards[i].value!=cards[i+1].value))
            numPair++;
        if((i>2)&&(cards[i].value==cards[i-1].value)&&(cards[i].value!=cards[i-2].value))
            numPair++;
    }
*/
    if(numPair==1)
        onePair = true;
    if(numPair==2)
        twoPair = true;
/*
    // check for three of a kind
    for(int i=1; i<4; i++)
    {
        if((cards[i].value==cards[i-1].value)&&(cards[i].value==cards[i+1].value))
            threeOfaKind = true;
    }
*/
    // check for straight
    if((cards[0].value==(cards[1].value-1))&&(cards[1].value==(cards[2].value-1))
       &&(cards[2].value==(cards[3].value-1))&&(cards[3].value==(cards[4].value-1)))
        straight = true;
    if((cards[0].value==1)&&(cards[1].value==10)&&(cards[2].value==11)&&(cards[3].value==12)&&(cards[4].value==13))
        straight = true;

    // check for flush
    if((cards[0].suit==cards[1].suit)&&(cards[1].suit==cards[2].suit)&&(cards[2].suit==cards[3].suit)&&(cards[3].suit==cards[4].suit))
        flush = true;

    // check full house
    if(onePair&&threeOfaKind)
        fullHouse = true;

    // check four of a kind
    if((cards[0].value==cards[1].value)&&(cards[1].value==cards[2].value)&&(cards[2].value==cards[3].value))
        fourOfaKind = true;
    if((cards[1].value==cards[2].value)&&(cards[2].value==cards[3].value)&&(cards[3].value==cards[4].value))
        fourOfaKind = true;

    // check for straight flush
    if(straight&&flush)
        straightFlush = true;

    // check for royal flush
    if(straight&&flush&&(cards[4].value==13))
        royalFlush = true;

    // assign hand value
    if(onePair)
        handValue = 1;
    if(twoPair)
        handValue = 2;
    if(threeOfaKind)
        handValue = 3;
    if(straight)
        handValue = 4;
    if(flush)
        handValue = 5;
    if(fullHouse)
        handValue = 6;
    if(fourOfaKind)
        handValue = 7;
    if(straightFlush)
        handValue = 8;
    if(royalFlush)
        handValue = 9;

    cout<<"Value of the hand is "<<handValue<<endl;
}

void Hand::findHighCard()
{
    for(int i=0; i<5; i++)
    {
        if(cards[i].value==1)
        {
            highCardValue = 1;
            return;
        }
        // generic case
        if((i>0)&&(cards[i].value>cards[i-1].value))
            highCardValue = cards[i].value;
        else // special case for first card in hand
            highCardValue = cards[i].value;
    }
    cout<<"Hand high card value is "<<highCardValue<<endl;
}

bool Hand::isMaxSize()
{
    if(cards.size()==maxHandSize)
        return true;
    else
        return false;
}

vector<string> Hand::cardsVector()
{
    vector<string> returnVector;
    if(5==0)
        return {"red_back","red_back","red_back","red_back","red_back"};
    for(int i=0; i<5; i++)
        returnVector.push_back(cards[i].cardID);

    return returnVector;
}
