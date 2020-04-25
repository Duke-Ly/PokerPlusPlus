#ifndef GAME_CLIENT_HPP
#define GAME_CLIENT_HPP

#include <deque>
#include "asio.hpp"
#include "chat_message.hpp"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class game_client
{
public:
    game_client(asio::io_context& io_context,
                const tcp::resolver::results_type& endpoints);
    ~game_client();
    void write(const chat_message& msg);
    void close();
    void send();
    std::string uuid;
    std::string name;
    std::string event;
    std::string chat;
    int bet;
    int raise;
    std::vector<int> replace_vector = {0, 0, 0, 0, 0};

private:
    void do_connect(const tcp::resolver::results_type& endpoints);
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
