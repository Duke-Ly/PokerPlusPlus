/*
 * CSE3310-003 Spring 2020
 * Project Prototyping
 * Duke Ly
 * 1001296968
 * 02/04/2020
*/
//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <ctime>
#include "json.hpp"
#include "asio.hpp"
#include "chat_message.hpp"

using asio::ip::tcp;
using json = nlohmann::json;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
    chat_client(asio::io_context& io_context,
                const tcp::resolver::results_type& endpoints)
        : io_context_(io_context),
          socket_(io_context)
    {
        // CSE3310 This is where the program call the function where connection is established with the server
        do_connect(endpoints);
    }

    void write(const chat_message& msg)
    {
        asio::post(io_context_,
                   [this, msg]()
        {
            bool write_in_progress = !write_msgs_.empty();
            write_msgs_.push_back(msg);
            if (!write_in_progress)
            {
                do_write();
            }
        });
    }

    void close()
    {
        asio::post(io_context_, [this]()
        {
            socket_.close();
        });
    }

private:
    void do_connect(const tcp::resolver::results_type& endpoints)
    {
        // CSE3310 This is where the program connection is established with the server
        asio::async_connect(socket_, endpoints,
                            [this](std::error_code ec, tcp::endpoint)
        {
            if (!ec)
            {
                do_read_header();
            }
        });
    }

    void do_read_header()
    {
        // CSE3310 This is where message body+header is received from the server
        asio::async_read(socket_,
                         asio::buffer(read_msg_.data(), chat_message::header_length),
                         [this](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec && read_msg_.decode_header())
            {
                do_read_body();
            }
            else
            {
                socket_.close();
            }
        });
    }

    void do_read_body()
    {
        // CSE3310 This is where message body is received from the server
        asio::async_read(socket_,
                         asio::buffer(read_msg_.body(), read_msg_.body_length()),
                         [this](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                std::cout.write(read_msg_.body(), read_msg_.body_length());
                std::cout << "\n";
                do_read_header();
            }
            else
            {
                socket_.close();
            }
        });
    }

    void do_write()
    {
        // CSE3310 This is where message is sent to the chat server
        asio::async_write(socket_,
                          asio::buffer(write_msgs_.front().data(),
                                       write_msgs_.front().length()),
                          [this](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
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

private:
    asio::io_context& io_context_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: chat_client <host> <port>\n";
            return 1;
        }

        asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        chat_client c(io_context, endpoints);

        std::thread t([&io_context]()
        {
            io_context.run();
        });

        char line[chat_message::max_body_length + 1];
        while (std::cin.getline(line, chat_message::max_body_length + 1))
        {   //CSE3310 This is where program accepts input from the user via cin
            json j;
            j["name"] = "duke";
            j["id"] = "1001296968";
            time_t tt;
            struct tm * ti;
            time (&tt);
            ti = localtime(&tt);
            std::string str = asctime(ti);
            str[str.length()-1] = ' ';
            j["chrono"] = str;
            std::string s = j.dump();
            s.append(line);
            strcpy(line, s.c_str());
            chat_message msg;
            msg.body_length(std::strlen(line));
            std::memcpy(msg.body(), line, msg.body_length());
            msg.encode_header();
            c.write(msg); // CSE3310 This is where the program send the string msg to be processed and sent to server
        }

        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
