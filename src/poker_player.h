#ifndef POKER_PLAYER_H
#define POKER_PLAYER_H

#include <gtkmm.h>
//#include "player.h"

class Poker_Player : public Gtk::Window
{
  public:
      Poker_Player();
      ~Poker_Player();
  protected:
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
//      Player *player;

};

#endif
