#ifndef POKER_TABLE_HPP
#define POKER_TABLE_HPP

#include <iostream>
#include "dealer.hpp"
#include "player.hpp"
#include "poker_player.hpp"

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
      set<player_ptr> players;
      enum max_recent_msgs = 100;
};

#endif
