// CSE3310 Spring 2020
//
// Much code copied from:
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <assert.h>
#include "asio.hpp"
#include "chat_message3.hpp"
#include "chat_client.hpp"

chat_client::chat_client(asio::io_context& io_context,
                         const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
{
    do_connect(endpoints);
}

chat_client::~chat_client() {};

void chat_client::write(const chat_message& msg)
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

void chat_client::close()
{
    asio::post(io_context_, [this]()
    {
        socket_.close();
    });
}

void chat_client::do_connect(const tcp::resolver::results_type& endpoints)
{
    asio::async_connect(socket_, endpoints,
                        [this](std::error_code ec, tcp::endpoint)
    {
        if (!ec)
        {
            do_read_header();
        }
    });
}

void chat_client::do_read_header()
{
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

void chat_client::do_read_body()
{
    asio::async_read(socket_,
                     asio::buffer(read_msg_.body(), read_msg_.body_length()),
                     [this](std::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            gdk_threads_enter();
            {
                char outline[read_msg_.body_length() + 2];
                // '\n' + '\0' is 2 more chars
                outline[0] = '\n';
                outline[read_msg_.body_length() + 1] = '\0';
                std::memcpy ( &outline[1], read_msg_.body(), read_msg_.body_length() );
                gtk_label_set_text (GTK_LABEL(fromView), outline);

                std::string p = "player "+ std::to_string (read_msg_.gs.player_cards[0][0]) + " " +
                                std::to_string (read_msg_.gs.player_cards[0][1]) + " " +
                                std::to_string (read_msg_.gs.player_cards[0][2]) + '\n' +
                                std::to_string (read_msg_.gs.player_cards[1][0]) + " " +
                                std::to_string (read_msg_.gs.player_cards[1][1]) + " " +
                                std::to_string (read_msg_.gs.player_cards[1][2]);

                std::string d = "dealer " + std::to_string (read_msg_.gs.dealer_cards[0]) + " " +
                                std::to_string (read_msg_.gs.dealer_cards[1]) + " " +
                                std::to_string (read_msg_.gs.dealer_cards[2]);

                if (read_msg_.gs.player_cards_valid)
                    gtk_label_set_text (GTK_LABEL(playerCards),p.c_str() );
                else
                    gtk_label_set_text (GTK_LABEL(playerCards),"waiting");

                if (read_msg_.gs.dealer_cards_valid)
                    gtk_label_set_text (GTK_LABEL(dealerCards),d.c_str() );
                else
                    gtk_label_set_text (GTK_LABEL(dealerCards),"waiting");
            }
            gdk_threads_leave();

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

void chat_client::do_write()
{
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

