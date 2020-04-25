#include <memory>
#include <vector>
#include "player.hpp"
#include "chat_message.hpp"
#include "poker_table.hpp"
#include "asio.hpp"
#include "json.hpp"
#include "poker_player.hpp"

using asio::ip::tcp;
using namespace std;
using json = nlohmann::json;

Poker_Player::Poker_Player(tcp::socket socket, Poker_Table& table)
    : socket_(std::move(socket)),
      table_(table)
{
    std::cout << "Creating a Poker_Player " << std::endl;
}

Poker_Player::~Poker_Player() {};

void Poker_Player::start()
{
    table_.join(shared_from_this());
    do_read_header();
}

void Poker_Player::deliver(const chat_message& msg)
{
    bool write_in_progress = !write_msg_.empty();
    write_msg_.push_back(msg);
    if (!write_in_progress)
    {
        do_write();
    }
}

void Poker_Player::do_read_header()
{
    auto self(shared_from_this());
    asio::async_read(socket_,
                     asio::buffer(read_msg_.data(), chat_message::header_length),
                     [this, self](std::error_code ec, std::size_t /*length*/)
    {
        if (!ec && read_msg_.decode_header())
        {
            do_read_body();
        }
        else
        {
            table_.leave(shared_from_this());
        }
    });
}

void Poker_Player::do_read_body()
{
    auto self(shared_from_this());
    asio::async_read(socket_,
                     asio::buffer(read_msg_.body(), read_msg_.body_length()),
                     [this, self](std::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            json to_dealer = json::parse(std::string(read_msg_.body()));
            json to_player;  // represents the entire game state.  sent to all players
            to_player["turn"] = "3f96b414-9ac9-40b5-8007-90d0e771f0d0";   // UUID of the current player.
            to_player["chat"] = {"this is one line","this is another","and so is this"};
            to_player["dealer_comment"] = "fred has raised and received 2 new cards";
            to_player["recommended_play"] = "you should fold";
            to_player["current_pot"] = 5.00;
            to_player["minimum_bet"] = 1.00;
            to_player["hand"] =
            {
                {{"total_balance",100}, {"current_bet",10}, {"uuid","3f96b414-9ac9-40b5-8007-90d0e771f0d0"}, {"name","Bud"},{"cards",{"acespades","10hearts","9clubs","2diamonds","kinghearts"}}},
                {{"total_balance",100}, {"current_bet",1}, {"uuid","3f96b414-9ac9-40b5-8007-20d0e771f0d0"}, {"name","Donald"},{"cards",{"acehearts","10spades","9clubs","2clubs","jackhearts"}}},
                {{"total_balance",100}, {"current_bet",5}, {"uuid","3f96b414-9ac9-40b5-8007-30d0e771f0d0"}, {"name","Ann"},{"cards",{"aceclubs","10diamonds","9clubs","2hearts","queenhearts"}}},
                {{"total_balance",100}, {"current_bet",0}, {"uuid","3f96b414-9ac9-40b5-8007-40d0e771f0d0"}, {"name","Melania"},{"cards",{"acediamonds","10clubs","9clubs","2spades","kinghearts"}}}
            };

            std::cout << "to player:" << std::endl;
            std::cout << to_player.dump(2) << std::endl;
            std::string t = to_player.dump();
            //read_msg_.body_length(0);
            //read_msg_.gs.dealer_cards_valid = false;
            //read_msg_.gs.player_cards_valid = false;
            // is it a join
            /*
            if (read_msg_.ca.join && read_msg_.ca.name_valid)
            {
                std::cout << "the name is " << read_msg_.ca.name << std::endl;
                std::string m = std::string(read_msg_.ca.name) + " has joined.";
                strcpy(read_msg_.body(),m.c_str());
                read_msg_.body_length(strlen(read_msg_.body()));
                self->name = std::string (read_msg_.ca.name);
            }
            if (self->name>"") // quick way to see if they have entered a name
            {
                if (read_msg_.ca.hit) // also need to check in order, since everyone has a turn
                {
                    // call the dealer class with some kind of method and
                    // argument
                    std::string m = self->name + " has asked for a hit.";
                    strcpy(read_msg_.body(),m.c_str());
                    read_msg_.body_length(strlen(read_msg_.body()));
                    // also set read_msg.gs.XXX to whatever needs to go to the clients
                }
            }

            // display the cards if everyone has joined
            if (table_.all_players_have_a_name())
            {
                table_.dealer.deal();
                read_msg_.gs.dealer_cards_valid = true;

            }
            */
            chat_message sending;
            if (t.size() < chat_message::max_body_length)
            {
                std::cout << "the size string being sent is " << t.size() << std::endl;
                memcpy( sending.body(), t.c_str(), t.size() );
                sending.body_length(t.size());
                sending.encode_header();
                table_.deliver(sending);
            }
            //read_msg_.encode_header(); // so the body text above gets sent
            //table_.deliver(read_msg_);
            do_read_header();
        }
        else
        {
            table_.leave(shared_from_this());
        }
    });
}

void Poker_Player::do_write()
{
    auto self(shared_from_this());
    asio::async_write(socket_,
                      asio::buffer(write_msg_.front().data(),
                                   write_msg_.front().length()),
                      [this, self](std::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            write_msg_.pop_front();
            if (!write_msg_.empty())
            {
                do_write();
            }
        }
        else
        {
            table_.leave(shared_from_this());
        }
    });
}
