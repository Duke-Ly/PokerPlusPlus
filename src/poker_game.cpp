#include "poker_game.hpp"
#include "asio.hpp"
#include <iostream>
#include <iostream>

using namespace std;
using asio::ip::tcp;

Poker_Game::Poker_Game(asio::io_context& io_context,
                       const tcp::endpoint& endpoint)
    : acceptor_(io_context, endpoint)
{
    cout<<"Creating a Poker_Game"<<endl;
    do_accept();

}

Poker_Game::~Poker_Game() { };

void do_accept()
{
    acceptor_.async_accept(
        [this](std::error_code ec, tcp::socket socket)
    {
        if (!ec)
        {
            std::make_shared<blackjack_player>(std::move(socket), table_)->start();
        }

        do_accept();
    });
}
