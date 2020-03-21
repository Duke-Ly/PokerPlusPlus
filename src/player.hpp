#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include "hand.hpp"

class Player
{
  public:
      Player();
      ~Player();
      virtual void deliver(const chat_message& msg) = 0;

      Hand playerHand;
      std::string name;
      std::string playerID;
};

#endif
