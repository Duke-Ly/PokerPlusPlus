#include "specator.hpp"

Spectator::Spectator(Hand playerHand, std::string name, std::string playerID)
    : playerHand{playerHand}, name{name}, playerID{playerID} { }

Spectator::~Spectator() { }
