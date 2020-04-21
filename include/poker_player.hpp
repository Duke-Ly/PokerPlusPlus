#ifndef POKER_PLAYER_HPP
#define POKER_PLAYER_HPP

#include <memory>
#include <string>
#include "hand.hpp"
#include "player.hpp"
#include "chat_message.hpp"
#include "poker_table.hpp"
#include "asio.hpp"

using asio::ip::tcp;
using namespace std;

class Poker_Player
    : public Player,
    public std::enable_shared_from_this<Poker_Player>
{
public:
    Poker_Player(tcp::socket socket, Poker_Table& table)
        : socket_(std::move(socket)),
          table_(table){};
    ~Poker_Player();
    void start();
    void deliver(const chat_message& msg);

private:
    void do_read_header();
    void do_read_body();
void do_write();
public:
    tcp::socket socket_;
    Poker_Table& table_;
    chat_message read_msg_;
    chat_message_queue write_msg_;
};

#endif
