//
// client_action.hpp
//

#ifndef CLIENT_ACTION_HPP
#define CLIENT_ACTION_HPP

class client_action
{
public:
    bool join;
    bool chat;
    bool check;
    bool call;
    bool fold;
    bool bet;
    bool raise;
    bool replaceCards;
    //
    bool hit;
    bool stand;
    bool name_valid;
    char name[25];
};

#endif
