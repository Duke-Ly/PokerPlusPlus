#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include "hand.hpp"

class Player
{
  public:
      Player();
      ~Player();
      virtual void deliver() = 0;

      Hand playerHand;
      std::string name;
      std::string playerID;
};

#endif
