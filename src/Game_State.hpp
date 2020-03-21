//
// Game_State.hpp
//

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

class Game_State
{
public:
    int cards_requested;
    std::vector<std::string> chat;
    std::string dealer_comment;
    std::vector<std::string> hand;
    int current_bet;
    std::string event;
    std::vector<std::string> from;
    int total_bet;
    std::string turn;
};

#endif
