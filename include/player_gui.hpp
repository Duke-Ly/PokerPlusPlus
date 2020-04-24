#ifndef Player_GUI
#define PLAYER_GUI

#include <gtkmm.h>

class Player_GUI : public Gtk::Window
{
protected:
  Glib::RefPtr<Gtk::Builder> builder;
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
  Gtk::Label* total_balance = nullptr;
  Gtk::AboutDialog* about_dialog = nullptr;
  Gtk::MessageDialog* help_dialog = nullptr;
  Gtk::Button* help_close = nullptr;

public:
  Player_GUI(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);

protected:
  void on_help_menu_activate();
  void on_help_close_clicked();
  void on_help_menu_clicked();
  void on_about_dialog_activate_link();
  void entry_player_chat_activate();
  void on_send_button_clicked();
  void on_clear_button_clicked();
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
};

#endif
