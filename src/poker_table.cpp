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

Poker_Table::Poker_Table()
{
    cout<<"Creating Poker Table"<<endl;
}

Poker_Table::~Poker_Table()
{
    for(auto & element : players)
    {
        delete &element;
    }
    players.clear();

    for(auto & element : folded_players)
    {
        delete &element;
    }
    folded_players.clear();

    for(auto & element : recent_msgs_)
    {
        delete &element;
    }
    recent_msgs_.clear();
}

void Poker_Table::join(player_ptr player_)
{
    if(players.size()<=5)
    {
        cout<<"new player joining the table"<<endl;
        players.insert(player_);
    }
}

void Poker_Table::leave(player_ptr player_)
{
    players.erase(player_);
    cout<<"player leaving the table"<<endl;
}

void Poker_Table::deliver(const chat_message& msg)
{
//    recent_msgs_.push_back(msg);
//    while (recent_msgs_.size()> max_recent_msgs)
//        recent_msgs_.pop_front();

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
    if(players.size()>=2)
        return true;
    else
        return false;
}

bool Poker_Table::all_bet_valid()
{
    set<player_ptr>::iterator it;
    for(it=players.begin(); it!=players.end(); it++)
    {
        if(((*it)->current_bet<minimum_bet)&&((*it)->total_balance!=0))
            return false;
    }
    return true;
}

player_ptr Poker_Table::find_winner()
{
    player_ptr winner;

    set<player_ptr>::iterator it;
    for(it=players.begin(); it!=players.end(); ++it)
    {
        if(it==players.begin())
        {
            winner = *it;
        }
        else if(((*it)->playerHand.handValue)>(winner->playerHand.handValue))
        {
            winner = *it;
        }
        else if(((*it)->playerHand.handValue)==(winner->playerHand.handValue))
        {
            if(((*it)->playerHand.highCardValue)>(winner->playerHand.highCardValue)
                    &&((winner->playerHand.highCardValue)!=1))
            {
                winner = *it;
            }
        }
    }

    return winner;
}
