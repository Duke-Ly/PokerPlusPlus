#ifndef POKER_PLAYER_H
#define POKER_PLAYER_H

#include <gtkmm.h>
//#include "player.h"
//#include "chat_message.h"

class Poker_Player : public Gtk::Window
{
  public:
      Poker_Player();
      ~Poker_Player();
      void start();
      void deliver(chat_message& msg);
      void on_new_game_click();
      void on_help_click();
      void on_about_click();
      void on_quit_click();
      void on_bet_click();
      void on_raise_click();
      void on_check_click();
      void on_call_click();
      void on_fold_click();
      void on_replace_click();
  private:
    void do_read_header();
    void do_read_body();
    void do_write();
    tcp::socket socket;
    Poker_Table& table_ptr;
    chat_message read_msg;
    chat_message_queue write_msg;
};

#endif
