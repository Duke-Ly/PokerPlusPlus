#ifndef Player_GUI
#define PLAYER_GUI

#include <gtkmm.h>
#include "game_client.hpp"

using namespace Gtk;
using namespace Glib;

class Player_GUI : public Gtk::Window
{
public:
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::Dialog* player_name_dialog = nullptr;
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
    Gtk::Entry* entry_player_chat = nullptr;
    Gtk::Button* button_send = nullptr;
    Gtk::Button* button_clear = nullptr;
    Gtk::Image* image_card1 = nullptr;
    Gtk::Image* image_card2 = nullptr;
    Gtk::Image* image_card3 = nullptr;
    Gtk::Image* image_card4 = nullptr;
    Gtk::Image* image_card5 = nullptr;
    Gtk::MenuItem* help_menu = nullptr;
    Gtk::MenuItem* about_menu = nullptr;
    Gtk::Label* label_dealer_message = nullptr;
    Gtk::Label* label_total_balance = nullptr;
    Gtk::Label* label_chat_message = nullptr;
    Gtk::Label* label_recommended_play = nullptr;
    Gtk::Label* label_pot_value = nullptr;
    Gtk::Label* label_current_bet = nullptr;
    Gtk::Label* label_minimum_bet = nullptr;
    Gtk::Label* label_player1_name = nullptr;
    Gtk::Label* label_player2_name = nullptr;
    Gtk::Label* label_player3_name = nullptr;
    Gtk::Label* label_player4_name = nullptr;
    Gtk::Label* label_player1_balance = nullptr;
    Gtk::Label* label_player2_balance = nullptr;
    Gtk::Label* label_player3_balance = nullptr;
    Gtk::Label* label_player4_balance = nullptr;
    Gtk::Label* label_spectator1 = nullptr;
    Gtk::Label* label_spectator2 = nullptr;
    Gtk::Label* label_spectator3 = nullptr;
    Gtk::Label* label_spectator4 = nullptr;
    Gtk::Label* label_spectator5 = nullptr;
    Gtk::AboutDialog* about_dialog = nullptr;
    Gtk::MessageDialog* help_dialog = nullptr;
    Gtk::Button* help_close = nullptr;
    Gtk::Button* name_dialog_enter = nullptr;
    Gtk::Entry* entry_player_name = nullptr;

public:
    Player_GUI(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
    game_client* client;
    friend void update(Player_GUI*);

protected:
    void on_name_dialog_enter_clicked();
    void on_help_menu_activate();
    void on_help_close_clicked();
    void on_help_menu_clicked();
    void on_about_dialog_activate_link();
    //void entry_player_chat_activate();
    void on_send_button_clicked();
    void on_clear_button_clicked();
    /*  void player_bet_entry_activate();
      void check1_toggled();
      void check2_toggled();
      void check3_toggled();
      void check4_toggled();
      void check5_toggled();
      void on_check_button_clicked();
    */void on_call_button_clicked();
    void on_fold_button_clicked();
    void on_bet_button_clicked();
    void on_raise_button_clicked();
    void on_all_in_button_clicked();
    void on_replace_button_clicked();
};

#endif
