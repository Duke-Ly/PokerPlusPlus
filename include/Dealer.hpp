#ifndef DEALER_HPP
#define DEALER_HPP

#include "card.hpp"
#include "player.hpp"
#include <vector>

using namespace std;

typedef std::shared_ptr<player> player_ptr;

class Dealer
{
public:
    Dealer();
    ~Dealer();
    void createDeck();
    void shuffleDeck();
    void deal();
    void next_player();

    int card_idx;
    vector<Card> deck;
    player_ptr current_player;
};

#endif
