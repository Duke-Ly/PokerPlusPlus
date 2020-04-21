#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "json.hpp"
#include "asio.hpp"
#include "chat_message.hpp"
#include "card.hpp"
#include "hand.hpp"
#include "player.hpp"
#include "dealer.hpp"
#include "poker_table.hpp"
#include "poker_player.hpp"
#include "poker_game.hpp"

using asio::ip::tcp;
using json = nlohmann::json;
using namespace std;

typedef std::deque<chat_message> chat_message_queue;

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: Poker_Game <port> [<port> ...]\n";
            return 1;
        }

        asio::io_context io_context;

        std::list<Poker_Game> servers;
        for (int i = 1; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_context, endpoint);
        }

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
