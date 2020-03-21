//
// chat_client.hpp
//

#ifndef CHAT_CLIENT_HPP
#define CHAT_CLIENT_HPP

#include <iostream>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <assert.h>
#include "asio.hpp"
#include "chat_message.hpp"

class chat_client
{
public:
    chat_client();
    ~chat_client();
    void write();
    void close();

private:
    void do_connect();
    void do_read_header();
    void do_read_body();
    void do_write();

private:
    asio::io_context& io_context_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

#endif
