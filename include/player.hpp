#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "chat_message.hpp"
#include "hand.hpp"

using namespace std;

class Player
{
  public:
      Player();
      virtual ~Player();
      virtual void deliver(const chat_message& msg) = 0;

      Hand playerHand;
      string name;
      string playerID;
};

#endif
