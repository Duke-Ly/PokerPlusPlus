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
#include "hand.hpp"
#include "player.hpp"

class Dealer
{
public:
    Dealer();
    ~Dealer();
    void createDeck();
    void shuffleDeck();
    void setCurrentDeck();
    void deal();
    void next_player();
};

#endif
