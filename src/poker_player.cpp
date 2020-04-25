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
    : socket_(move(socket)),
      table_(table)
{
    cout<<"Creating a Poker_Player"<<endl;
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
                     [this, self](error_code ec, size_t /*length*/)
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
                     [this, self](error_code ec, size_t /*length*/)
    {
        if (!ec)
        {
            json to_dealer = json::parse(string(read_msg_.body()));
            json to_player;  // represents the entire game state.  sent to all players
            to_player["turn"] = table_.dealer.current_player->playerUUID;   // UUID of the current player.
            to_player["chat"] = "chat msg";
            to_player["dealer_comment"] = "Awaiting for two player";
            to_player["recommended_play"] = "You should check";
            to_player["current_pot"] = table_.current_pot;
            to_player["minimum_bet"] = table_.minimum_bet;
            to_player["hand"] = {};
            set<player_ptr>::iterator it;
            for(it=table_.players.begin(); it!=table_.players.end(); ++it)
            {

                to_player["hand"].push_back(
                {
                    {"total_balance",(*it)->total_balance},
                    {"current_bet",(*it)->current_bet},
                    {"uuid",(*it)->playerUUID},
                    {"name",(*it)->name},
                    {"cards",(*it)->playerHand.cardsVector()}});
            }
            /*
                        to_player["hand"] =
                        {
                            {{"total_balance",100}, {"current_bet",10}, {"uuid","3f96b414-9ac9-40b5-8007-90d0e771f0d0"}, {"name","Bud"},{"cards",{"acespades","10hearts","9clubs","2diamonds","kinghearts"}}},
                            {{"total_balance",100}, {"current_bet",1}, {"uuid","3f96b414-9ac9-40b5-8007-20d0e771f0d0"}, {"name","Donald"},{"cards",{"acehearts","10spades","9clubs","2clubs","jackhearts"}}},
                            {{"total_balance",100}, {"current_bet",5}, {"uuid","3f96b414-9ac9-40b5-8007-30d0e771f0d0"}, {"name","Ann"},{"cards",{"aceclubs","10diamonds","9clubs","2hearts","queenhearts"}}},
                            {{"total_balance",100}, {"current_bet",0}, {"uuid","3f96b414-9ac9-40b5-8007-40d0e771f0d0"}, {"name","Melania"},{"cards",{"acediamonds","10clubs","9clubs","2spades","kinghearts"}}}
                        };
            */
            cout<<"to player:"<<endl;
            cout<<to_player.dump(2)<<endl;
            string t = to_player.dump();

            if(to_dealer["event"]=="join")
            {
                string m = string(to_dealer["from"].at("name")) + " has joined.";
                cout<<m<<endl;
                self->name = string(to_dealer["from"].at("name"));
                self->playerUUID = string(to_dealer["from"].at("uuid"));
            }

            if(table_.players.size() >= 2)
            {
                table_.game_active = true;
            }

            chat_message sending;
            if (t.size() < chat_message::max_body_length)
            {
                cout<<"the size string being sent is "<<t.size()<<endl;
                memcpy(sending.body(), t.c_str(), t.size());
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
                      asio::buffer(write_msg_.front().data(), write_msg_.front().length()),
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
