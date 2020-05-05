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
    Player()
    {
        cout<<"Creating a new player"<<endl;
    }
    virtual ~Player() {};
    virtual void deliver(const chat_message& msg) = 0;

    Hand playerHand;
    string name;
    string playerUUID;
    int current_bet = 0;
    int total_balance = 100;
};

#endif
