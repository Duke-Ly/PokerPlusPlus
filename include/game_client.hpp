#ifndef GAME_CLIENT_HPP
#define GAME_CLIENT_HPP

#include <deque>
#include "asio.hpp"
#include "chat_message.hpp"

using asio::ip::tcp;
using namespace std;

typedef deque<chat_message> chat_message_queue;

class Player_GUI;

class game_client
{
public:
    game_client(asio::io_context& io_context,
                const tcp::resolver::results_type& endpoints);
    void write(const chat_message& msg);
    void close();

    void send();
    friend void update(Player_GUI*, game_client*);

    Player_GUI* guiPTR;

    // to_dealer
    string uuid;
    string name;
    string event;
    int bet = 0;
    int raise = 0;
    vector<int> replace_vector = {0, 0, 0, 0, 0};

    // to_player
    string turn;
    string chat;
    string dealer_comment;
    string recommended_play;
    int current_pot = 0;
    int minimum_bet = 0;
    int total_balance = 100;
    int current_bet = 0;
    vector<string> cards = {"red_back",
                            "red_back",
                            "red_back",
                            "red_back",
                            "red_back"
                           };
    vector<string> playersName;
    vector<int> playersBalance;

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
