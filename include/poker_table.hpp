#ifndef POKER_TABLE_HPP
#define POKER_TABLE_HPP

#include <iostream>
#include <vector>
#include <set>
#include "Dealer.hpp"
#include "player.hpp"
#include "poker_player.hpp"

typedef std::deque<chat_message> chat_message_queue;

class Poker_Table
{
  public:
      Poker_Table();
      ~Poker_Table();
      void join();
      void leave();
      void deliver();
      bool all_players_have_UUID();

      Dealer dealer;

  private:
      int max_recent_msgs = 100;
	std::set<player_ptr> players;
	chat_message_queue recent_msgs_;
};

#endif
