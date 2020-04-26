#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <set>
#include <utility>
#include "asio.hpp"
#include "dealer.hpp"
#include "player.hpp"
#include "poker_table.hpp"

Poker_Table::Poker_Table()
{
    cout<<"Creating Poker Table"<<endl;
}

Poker_Table::~Poker_Table() {};

void Poker_Table::join(player_ptr player_)
{
    std::cout<<"new player joining the table"<<std::endl;
    players.insert(player_);
}

void Poker_Table::leave(player_ptr player_)
{
    players.erase(player_);
}

void Poker_Table::deliver(const chat_message& msg)
{
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size()> max_recent_msgs)
        recent_msgs_.pop_front();

    for (auto player_: players)
        player_ -> deliver (msg);
}

bool Poker_Table::all_players_have_a_name()
{
    bool retval = true;
    for (auto player : players)
        retval = retval && (player->name > "");

    return retval;
}

bool Poker_Table::all_players_have_UUID()
{
    bool retval = true;
    for (auto player : players)
        retval = retval && (player->playerUUID > "");

    return retval;
}

bool Poker_Table::enough_player()
{
    if(players.size()== 2)
        return true;
    else
        return false;
}
