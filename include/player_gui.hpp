#ifndef PLAYER_GUI_HPP
#define PLAYER_GUI_HPP

#include <iostream>
#include "card.hpp"
#include "chat_client.hpp"
#include "chat_message.hpp"
#include "Dealer.hpp"
#include "hand.hpp"
#include "player.hpp"
#include "poker_game.hpp"
#include "poker_player.hpp"
#include "poker_table.hpp"
#include "spectator.hpp"
#include "asio.hpp"
#include <gtkmm.h>

class Player_GUI
{
public:
  void player_bet_entry_activate();
  void check1_toggled();
  void check2_toggled();
  void check3_toggled();
  void check4_toggled();
  void check5_toggled();
  void on_check_button_clicked();
  void on_call_button_clicked();
  void on_fold_button_clicked();
  void on_bet_button_clicked();
  void on_raise_button_clicked();
  void on_all_in_button_clicked();
  void on_replace_button_clicked();

private:
  Gtk::Window* window = nullptr;
  Gtk::Button* button_check = nullptr;
  Gtk::Button* button_call = nullptr;
  Gtk::Button* button_fold = nullptr;
  Gtk::Button* button_bet = nullptr;
  Gtk::Button* button_raise = nullptr;
  Gtk::Button* button_all_in = nullptr;
  Gtk::Button* button_replace = nullptr;
  Gtk::CheckButton* check1 = nullptr;
  Gtk::CheckButton* check2 = nullptr;
  Gtk::CheckButton* check3 = nullptr;
  Gtk::CheckButton* check4 = nullptr;
  Gtk::CheckButton* check5 = nullptr;
  Gtk::Entry* player_bet_entry = nullptr;
};

#endif
