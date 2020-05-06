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

using namespace std;

void Poker_Table::join(player_ptr player)
{
    if(players_.size()<=5)
    {
        players_.insert(player);
    }
}

void Poker_Table::leave(player_ptr player)
{
    players_.erase(player);
}

void Poker_Table::deliver(const chat_message& msg)
{
    recent_msgs_.push_back(msg);
    while(recent_msgs_.size()> max_recent_msgs)
        recent_msgs_.pop_front();

    for(auto player: players_)
        player->deliver(msg);
}

bool Poker_Table::all_players_have_a_name()
{
    bool retval = true;
    for(auto player: players_)
        retval = retval && (player->name > "");

    return retval;
}

bool Poker_Table::all_players_have_UUID()
{
    bool retval = true;
    for(auto player : players_)
        retval = retval && (player->UUID > "");

    return retval;
}

bool Poker_Table::enough_player()
{
    if(players_.size()>=2)
        return true;
    else
        return false;
}

bool Poker_Table::all_bet_valid()
{
    for(auto player : players_)
    {
        if((player->current_bet<minimum_bet)&&(player->total_balance!=0))
            return false;
    }
    return true;
}

player_ptr Poker_Table::find_winner()
{
    player_ptr winner = *(players_.begin());

    for(auto player : players_)
    {
        if((player->hand.handValue)>(winner->hand.handValue))
        {
            winner = player;
        }
        else if((player->hand.handValue)==(winner->hand.handValue))
        {
            if((player->hand.highCardValue)>(winner->hand.highCardValue)
                    &&((winner->hand.highCardValue)!=1))
            {
                winner = player;
            }
        }
    }

    return winner;
}
