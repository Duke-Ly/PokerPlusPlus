#ifndef POKER_PLAYER_HPP
#define POKER_PLAYER_HPP

#include <gtkmm.h>
#include "player.hpp"
#include "chat_message.hpp"
#include "poker_table.hpp"
#include "asio.hpp"

class Poker_Player : public Gtk::Window
{
  public:
      Poker_Player();
      ~Poker_Player();
      void start();
      void deliver(chat_message& msg);

  private:
    void do_read_header();
    void do_read_body();
    void do_write();
    tcp::socket socket;
    chat_message read_msg;
    chat_message_queue write_msg;
    poker_table& table;
};

#endif
