#include <cstdlib>
#include <deque>
#include <iostream>
#include "json.hpp"
#include "asio.hpp"
#include "chat_message.hpp"
#include "game_client.hpp"
#include "player_gui.hpp"

using asio::ip::tcp;
using json = nlohmann::json;
using namespace Gtk;
using namespace std;

game_client::game_client(asio::io_context& io_context,
                         const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
{
    do_connect(endpoints);
}

game_client::~game_client() {};

void game_client::write(const chat_message& msg)
{
    asio::post(io_context_, [this, msg]()
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if(!write_in_progress)
        {
            do_write();
        }
    });
}

void game_client::close()
{
    asio::post(io_context_, [this]()
    {
        socket_.close();
    });
}

void game_client::send()
{
    chat_message msg;

    json to_dealer;
    to_dealer["from"] = { {"uuid",this->uuid}, {"name",this->name} };
    to_dealer["event"] = this->event; // "check","bet","call","raise","fold","all_in","replace","chat","join"
    to_dealer["replace_vector"] = this->replace_vector;
    to_dealer["bet"] = this->bet;
    to_dealer["raise"] = this->raise;
    to_dealer["chat"] = this->chat;

    cout<<"to dealer:"<<endl;
    cout<<to_dealer.dump(2)<<endl;

    string t = to_dealer.dump();

    msg.body_length(t.size());
    memcpy(msg.body(), t.c_str(), msg.body_length());
    msg.encode_header();
    this->write(msg);
}

void game_client::do_connect(const tcp::resolver::results_type& endpoints)
{
    asio::async_connect(socket_, endpoints, [this](error_code ec, tcp::endpoint)
    {
        if(!ec)
        {
            do_read_header();
        }
    });
}

void game_client::do_read_header()
{
    asio::async_read(socket_, asio::buffer(read_msg_.data(), chat_message::header_length),
                     [this](error_code ec, size_t /*length*/)
    {
        if(!ec && read_msg_.decode_header())
        {
            for(unsigned int i=0; i<chat_message::max_body_length; i++)
            {
                read_msg_.body() [i] = '\0';
            }
            do_read_body();
        }
        else
        {
            socket_.close();
        }
    });
}

void game_client::do_read_body()
{
    asio::async_read(socket_, asio::buffer(read_msg_.body(), read_msg_.body_length()),
                     [this](error_code ec, size_t /*length*/)
    {
        if(!ec)
        {
            cout<<"from dealer:"<<endl;
            json to_player = json::parse(string(read_msg_.body()));
            cout<<to_player.dump()<<endl;
            this->turn = to_string(to_player["turn"]);
            this->chat = to_string(to_player["chat"]);
            this->dealer_comment = to_string(to_player["dealer_comment"]);
            this->recommended_play = to_string(to_player["recommended_play"]);
            this->current_pot = (int) to_player["current_pot"];
            this->minimum_bet = (int) to_player["minimum_bet"];
            unsigned int index;
            for(index=0; index<to_player["hand"].size(); index++)
            {
                if(uuid.compare(to_string(to_player["hand"][index].at("uuid"))))
                    break;
            }
            this->total_balance = (int) to_player["hand"][index].at("total_balance");
            this->current_bet = (int) to_player["hand"][index].at("current_bet");
            for(unsigned int i=0; i<this->cards.size(); i++)
                this->cards[i] = (string) to_player["hand"][index].at("cards")[i];

            //cout.write(read_msg_.body(), read_msg_.body_length());
            //cout << endl;
            do_read_header();
        }
        else
        {
            socket_.close();
        }
    });
}

void game_client::do_write()
{
    asio::async_write(socket_, asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
                      [this](error_code ec, size_t /*length*/)
    {
        if(!ec)
        {
            write_msgs_.pop_front();
            if(!write_msgs_.empty())
            {
                do_write();
            }
        }
        else
        {
            socket_.close();
        }
    });
}
