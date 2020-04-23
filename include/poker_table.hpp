#ifndef POKER_TABLE_HPP
#define POKER_TABLE_HPP

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <set>
#include <utility>
#include "asio.hpp"
#include "Dealer.hpp"
#include "player.hpp"

using asio::ip::tcp;
using namespace std;

typedef std::deque<chat_message> chat_message_queue;

class Poker_Table
{
public:
    Poker_Table();
    ~Poker_Table();
    void join(player_ptr player_);
    void leave(player_ptr player_);
    void deliver(const chat_message& msg);
    bool all_players_have_a_name();
    bool all_players_have_UUID();

    Dealer dealer;

private:
    enum { max_recent_msgs = 100 };
    std::set<player_ptr> players;
    chat_message_queue recent_msgs_;
};

#endif
