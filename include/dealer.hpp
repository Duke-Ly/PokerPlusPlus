#ifndef DEALER_HPP
#define DEALER_HPP

#include <vector>
#include <memory>
#include "card.hpp"
#include "player.hpp"

using namespace std;

class Dealer
{
public:
    Dealer();
    ~Dealer();
    void createDeck();
    void shuffleDeck();
    void deal(player_ptr dealPlayer);
    void next_player(player_ptr nextPlayer);
    void replace_cards(vector<int> replace_vector);

    int card_idx;
    vector<Card> deck;
    player_ptr current_player = nullptr;
};

#endif
