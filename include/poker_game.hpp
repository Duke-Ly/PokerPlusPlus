#ifndef POKER_GAME_HPP
#define POKER_GAME_HPP

#include <iostream>
#include "asio.hpp"
#include "poker_player.hpp"
#include "poker_table.hpp"

using namespace std;
using asio::ip::tcp;

class Poker_Game
{
public:
    Poker_Game(asio::io_context& io_context,
               const tcp::endpoint& endpoint)
        : acceptor_(io_context, endpoint)
    {
        cout<<"Creating a Poker_Game"<<endl;
        do_accept();
    }
    ~Poker_Game();

private:
    void do_accept();
    tcp::acceptor acceptor_;
    Poker_Table table_;
};

#endif
