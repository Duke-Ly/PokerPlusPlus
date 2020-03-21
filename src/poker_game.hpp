#ifndef POKER_GAME_HPP
#define POKER_GAME_HPP

#include <iostream>
#include "asio.h"
#include "poker_table.hpp"

class Poker_Game
{
  public:
      Poker_Game();
      ~Poker_Game();
      void do_accept();

  private:
      tcp::acceptor acceptor;
      Poker_Table table_;
};

#endif
