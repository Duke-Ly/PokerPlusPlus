#ifndef POKER_TABLE_HPP
#define POKER_TABLE_HPP

#include <iostream>
#include "dealer.hpp"
#include "poker_player.hpp"

typedef class chat_message_queue
{
  std::deque <chat_message> cm;
}

class Poker_Table
{
  public:
      Poker_Table();
      ~Poker_Table();
      void join(player_ptr player);
      void leave(player_ptr player);
      void deliver(const chat_message& msg);
      bool all_players_have_UUID();

      Dealer dealer;

  private:
      set<player_ptr> players;
      enum max_recent_msgs = 100;
      chat_message_queue recent_msgs;
};

#endif
