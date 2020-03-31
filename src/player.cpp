#include "player.hpp"

Player::Player(Hand playerHand, std::string name, std::string playerID)
    : playerHand{playerHand}, name{name}, playerID{playerID} { }

Player::~Player() { }

virtual void deliver(const chat_message msg)
{
  bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
      do_write();
    }
}
