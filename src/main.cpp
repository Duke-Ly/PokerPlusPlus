#include <iostream>
#include "card.hpp"
#include "chat_client.hpp"
#include "chat_message.cpp"
#include "Dealer.hpp"
#include "hand.hpp"
#include "player.hpp"
#include "poker_game.hpp"
#include "poker_player.hpp"
#include "poker_table.hpp"
#include "spectator.hpp"
#include "asio.hpp"
#include <gtkmm.h>

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

void player_bet_entry_activate()
{
  if (window)
  {
    if (player_bet_entry)
    {
      std::cout << "$" + player_bet_entry->get_text() + " bet was set.\n\n";
    }
  }
}

void check1_toggled()
{
  if (window)
  {
    if (check1)
        std::cout << "Check Box 1 checked.\n\n";
  }
}

void check2_toggled()
{
  if (window)
  {
    if (check2)
        std::cout << "Check Box 2 checked.\n\n";
  }
}

void check3_toggled()
{
  if (window)
  {
    if (check3)
        std::cout << "Check Box 3 checked.\n\n";
  }
}

void check4_toggled()
{
  if (window)
  {
    if (check4)
        std::cout << "Check Box 4 checked.\n\n";
  }
}

void check5_toggled()
{
  if (window)
  {
    if (check5)
        std::cout << "Check Box 5 checked.\n\n";
  }
}

void on_check_button_clicked()
{
  if(window)
  {
    if (button_check)
        std::cout << "Check Button was clicked.\n\n";
  }
}

void on_call_button_clicked()
{
  if (window)
  {
    if (button_call)
        std::cout << "Call Button was clicked.\n\n";
  }
}

void on_fold_button_clicked()
{
  if (window)
  {
    if (button_fold)
        std::cout << "Fold Button was clicked.\n\n";
  }
}

void on_bet_button_clicked()
{
  if (window)
  {
    if (button_bet)
        std::cout << "Bet Button was clicked.\n\n";
  }
}

void on_raise_button_clicked()
{
  if (window)
  {
    if (button_raise)
        std::cout << "Raise Button was clicked.\n\n";
  }
}

void on_all_in_button_clicked()
{
  if (window)
  {
    if (button_all_in)
        std::cout << "All-in Button was clicked.\n\n";
  }
}

void on_replace_button_clicked()
{
  if (window)
  {
    if (button_replace)
        std::cout << "Replace Button was clicked.\n\n";
  }
}

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create(argc, argv, "Poker++");

  auto refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("../assets/gui_mockup.glade");
  }
  catch (const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch (const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch (const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  refBuilder->get_widget("window", window);

  if (window)
  {
    refBuilder->get_widget("button_check", button_check);
    refBuilder->get_widget("button_call", button_call);
    refBuilder->get_widget("button_fold", button_fold);
    refBuilder->get_widget("button_bet", button_bet);
    refBuilder->get_widget("button_raise", button_raise);
    refBuilder->get_widget("button_all_in", button_all_in);
    refBuilder->get_widget("button_replace", button_replace);
    refBuilder->get_widget("check1", check1);
    refBuilder->get_widget("check2", check2);
    refBuilder->get_widget("check3", check3);
    refBuilder->get_widget("check4", check4);
    refBuilder->get_widget("check5", check5);
    refBuilder->get_widget("player_bet_entry", player_bet_entry);

    if (button_check)
    {
      button_check->signal_clicked().connect(sigc::ptr_fun(on_check_button_clicked));
    }
    if (button_call)
    {
      button_call->signal_clicked().connect(sigc::ptr_fun(on_call_button_clicked));
    }
    if (button_fold)
    {
      button_fold->signal_clicked().connect(sigc::ptr_fun(on_fold_button_clicked));
    }
    if (button_bet)
    {
      button_bet->signal_clicked().connect(sigc::ptr_fun(on_bet_button_clicked));
    }
    if (button_raise)
    {
      button_raise->signal_clicked().connect(sigc::ptr_fun(on_raise_button_clicked));
    }
    if (button_all_in)
    {
      button_all_in->signal_clicked().connect(sigc::ptr_fun(on_all_in_button_clicked));
    }
    if (button_replace)
    {
      button_replace->signal_clicked().connect(sigc::ptr_fun(on_replace_button_clicked));
    }
    if (check1)
    {
      check1->signal_clicked().connect(sigc::ptr_fun(check1_toggled));
    }
    if (check2)
    {
      check2->signal_clicked().connect(sigc::ptr_fun(check2_toggled));
    }
    if (check3)
    {
      check3->signal_clicked().connect(sigc::ptr_fun(check3_toggled));
    }
    if (check4)
    {
      check4->signal_clicked().connect(sigc::ptr_fun(check4_toggled));
    }
    if (check5)
    {
      check5->signal_clicked().connect(sigc::ptr_fun(check5_toggled));
    }
    if (player_bet_entry)
    {
      player_bet_entry->signal_activate().connect(sigc::ptr_fun(player_bet_entry_activate));
    }

    app->run(*window);
  }

  delete window;


  return 0;
}
