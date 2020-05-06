#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>
#include "chat_message.hpp"
#include "hand.hpp"

using namespace std;

class Player
{
public:
    virtual ~Player() {}
    virtual void deliver(const chat_message& msg) = 0;

    Hand hand;
    string name;
    string UUID;
    int current_bet = 0;
    int total_balance = 100;
};

typedef shared_ptr<Player> player_ptr;

#endif
