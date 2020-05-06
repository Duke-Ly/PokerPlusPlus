#include <iostream>
#include "asio.hpp"
#include "poker_player.hpp"
#include "poker_game.hpp"
#include "poker_table.hpp"

using namespace std;
using asio::ip::tcp;

Poker_Game::Poker_Game(asio::io_context& io_context,
                       const tcp::endpoint& endpoint)
    : acceptor_(io_context, endpoint)
{
    do_accept();
}

void Poker_Game::do_accept()
{
    acceptor_.async_accept(
        [this](error_code ec, tcp::socket socket)
    {
        if(!ec)
        {
            make_shared<Poker_Player>(move(socket), table_)->start();
        }
        do_accept();
    });
}
