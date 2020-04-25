#include <cstdlib>
#include <deque>
#include <iostream>
#include "json.hpp"
#include "asio.hpp"
#include "chat_message.hpp"
#include "game_client.hpp"

using asio::ip::tcp;
using json = nlohmann::json;
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
            cout<<"From the dealer"<<endl;
            cout.write(read_msg_.body(), read_msg_.body_length());
            cout << "\n";
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
