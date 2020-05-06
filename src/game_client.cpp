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
            // clear out the old buffer from the last read
            // a '\0' is a good value to make sure a string
            // is terminated
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
            json to_player = json::parse(string(read_msg_.body()));
            cout<<"from dealer:"<<endl;
            cout<<to_player.dump(2)<<endl;

            turn = to_player["turn"];
            chat = to_player["chat"];
            dealer_comment = to_player["dealer_comment"];
            recommended_play = to_player["recommended_play"];
            current_pot = to_player["current_pot"];
            minimum_bet = to_player["minimum_bet"];

            unsigned int index;
            playersName.clear();
            playersBalance.clear();
            cards.clear();
            cout<<"There are "<<to_player["hand"].size()<<" players"<<endl;

            for(index=0; index<to_player["hand"].size(); index++)
            {
                if(!(uuid.compare(to_player["hand"][index].at("uuid"))))
                {
                    total_balance = to_player["hand"][index].at("total_balance");
                    current_bet = to_player["hand"][index].at("current_bet");

                    for(auto card: to_player["hand"][index].at("cards"))
                        cards.push_back(card);
                }
                else
                {
                    playersName.push_back(to_player["hand"][index].at("name"));
                    playersBalance.push_back(to_player["hand"][index].at("total_balance"));
                }
            }

            update(guiPTR, this);
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

void game_client::send()
{
    chat_message msg;

    json to_dealer;
    to_dealer["from"] = { {"uuid",uuid}, {"name",name} };
    to_dealer["event"] = event; // "check","bet","call","raise","fold","all_in","replace","chat","join"
    to_dealer["replace_vector"] = replace_vector;
    to_dealer["bet"] = bet;
    to_dealer["raise"] = raise;
    to_dealer["chat"] = chat;

    cout<<"to dealer:"<<endl;
    cout<<to_dealer.dump(2)<<endl;

    string t = to_dealer.dump();

    msg.body_length(t.size());
    memcpy(msg.body(), t.c_str(), msg.body_length());
    msg.encode_header();
    write(msg);
}
