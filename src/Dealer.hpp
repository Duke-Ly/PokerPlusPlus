//
// Dealer.hpp
//

#ifndef DEALER_HPP
#define DEALER_HPP

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "chat_message.hpp"
#include "Hand.hpp"
#include "Player.hpp"

typedef std::shared_ptr<player> player_ptr;

class Dealer
{
public:
    Dealer();
    ~Dealer();
    void createDeck();
    void shuffleDeck();
    void setCurrentDeck();
    void deal();
    void next_player(player_ptr nextPlayer);
};

#endif
