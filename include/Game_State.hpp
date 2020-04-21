//
// game_state.hpp
//

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <vector>
#include <string>

using namespace std;

class game_state
{
public:
   bool dealer_cards_valid;
   int dealer_cards[3];
   bool player_cards_valid;
   int player_cards[2][3]; // [players][cards]
/*
    int cards_requested;
    vector<string> chat;
    string dealer_comment;
    vector<string> hand;
    int current_bet;
    string event;
    vector<string> from;
    int total_bet;
    string turn;
*/
};

#endif
