#ifndef POKER_TABLE_HPP
#define POKER_TABLE_HPP

#include <deque>
#include <memory>
#include <set>
#include "asio.hpp"
#include "dealer.hpp"
#include "player.hpp"

using asio::ip::tcp;
using namespace std;

typedef deque<chat_message> chat_message_queue;
enum gameState {WAITING, ANTE, BETTING1, SWAP, BETTING2, WIN};

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
    int current_pot = 0;
    int minimum_bet = 1;
    set<player_ptr> players;
    bool game_active = false;
    gameState game_state = WAITING;
    chat_message_queue recent_msgs_;

private:
    enum {max_recent_msgs = 100};

};

#endif
