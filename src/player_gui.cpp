#include <gtkmm.h>
#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include "player_gui.hpp"
#include "game_client.hpp"

using namespace Gtk;
using namespace std;
using namespace boost::uuids;

Player_GUI::Player_GUI(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject), builder(refGlade)
{
    builder->get_widget("player_name_dialog", player_name_dialog);
    builder->get_widget("name_dialog_enter", name_dialog_enter);
    builder->get_widget("entry_player_name", entry_player_name);
    builder->get_widget("window", window);
    builder->get_widget("button_check",  button_check);
    builder->get_widget("button_call", button_call);
    builder->get_widget("button_fold", button_fold);
    builder->get_widget("button_bet", button_bet);
    builder->get_widget("button_raise", button_raise);
    builder->get_widget("button_all_in", button_all_in);
    builder->get_widget("button_replace", button_replace);
    builder->get_widget("check1", check1);
    builder->get_widget("check2", check2);
    builder->get_widget("check3", check3);
    builder->get_widget("check4", check4);
    builder->get_widget("check5", check5);
    builder->get_widget("player_bet_entry", player_bet_entry);
    builder->get_widget("entry_player_chat", entry_player_chat);
    builder->get_widget("button_send", button_send);
    builder->get_widget("button_clear", button_clear);
    builder->get_widget("image_card1", image_card1);
    builder->get_widget("image_card2", image_card2);
    builder->get_widget("image_card3", image_card3);
    builder->get_widget("image_card4", image_card4);
    builder->get_widget("image_card5", image_card5);
    builder->get_widget("help_menu", help_menu);
    builder->get_widget("about_menu", about_menu);
    builder->get_widget("label_dealer_message", label_dealer_message);
    builder->get_widget("total_balance", total_balance);
    builder->get_widget("about_dialog", about_dialog);
    builder->get_widget("help_dialog", help_dialog);
    builder->get_widget("help_close", help_close);

    name_dialog_enter->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_name_dialog_enter_clicked));
    button_check->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_check_button_clicked));
    button_call->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_call_button_clicked));
    button_fold->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_fold_button_clicked));
    button_bet->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_bet_button_clicked));
    button_raise->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_raise_button_clicked));
    button_all_in->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_all_in_button_clicked));
    button_replace->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_replace_button_clicked));
    check1->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::check1_toggled));
    check2->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::check2_toggled));
    check3->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::check3_toggled));
    check4->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::check4_toggled));
    check5->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::check5_toggled));
    player_bet_entry->signal_activate().connect(sigc::mem_fun(*this, &Player_GUI::player_bet_entry_activate));
    entry_player_chat->signal_activate().connect(sigc::mem_fun(*this, &Player_GUI::entry_player_chat_activate));
    button_send->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_send_button_clicked));
    button_clear->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_clear_button_clicked));
    about_menu->signal_activate().connect(sigc::mem_fun(*this, &Player_GUI::on_about_dialog_activate_link));
    help_menu->signal_activate().connect(sigc::mem_fun(*this, &Player_GUI::on_help_menu_activate));
    help_close->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_help_close_clicked));

    window->set_title("Poker++");
    window->override_background_color(Gdk::RGBA{"green"});
    player_name_dialog->show();
}

void Player_GUI::on_name_dialog_enter_clicked()
{
    std::string playerName;

    if (entry_player_name->get_text() == "")
    {
        entry_player_name->set_text("**INVALID** Please enter name");
    }
    else if (entry_player_name->get_text() == "**INVALID** Please enter name")
    {
        entry_player_name->set_text("**INVALID** Please enter name");
    }
    else
    {
        random_generator gen;
        uuid id = gen();
        playerName = entry_player_name->get_text();
        client->name = playerName;
        cout<<"Welcome "<<playerName<<"!\n\n";
        string uuid = to_string(id);
        client->uuid = uuid;
        client->event = "join";
        client->send();
        player_name_dialog->hide();
    }
}

void Player_GUI::on_help_menu_activate()
{
    help_dialog->show();
}

void Player_GUI::on_help_close_clicked()
{
    help_dialog->hide();
}

void Player_GUI::on_about_dialog_activate_link()
{
    about_dialog->show();
}


void Player_GUI::entry_player_chat_activate()
{
    if (window)
    {
        if (entry_player_chat)
        {
            std::cout << entry_player_chat->get_text() + "\n\n";
            entry_player_chat->set_text("");
        }
    }
}

void Player_GUI::on_send_button_clicked()
{
    if(window)
    {
        if (button_send)
        {
            cout<<"Send Button was clicked"<<endl;
            string chat = entry_player_chat->get_text();
            client->chat = chat;
            string event = "chat";
            client->event = event;
            cout<<chat<<endl;
            entry_player_chat->set_text("");
            client->send();
        }

    }
}

void Player_GUI::on_clear_button_clicked()
{
    if(window)
    {
        if (button_clear)
        {
            std::cout << "Clear Button was clicked.\n\n";
            entry_player_chat->set_text("");
        }
    }
}

void Player_GUI::player_bet_entry_activate()
{
    if (window)
    {
        if (player_bet_entry)
        {
            int player_balance = 100;

            int entry = std::stoi(player_bet_entry->get_text(),nullptr,0);
            player_balance = player_balance - entry;

            std::cout << "$" + player_bet_entry->get_text() + " bet was set.\n\n";
            std::cout << "current balance is " << player_balance << "\n\n";

            player_bet_entry->set_text("");
        }

    }
}

void Player_GUI::check1_toggled()
{
    if (window)
    {
        if (check1->get_active())
            std::cout << "Check Box 1 checked.\n\n";
        else
            std::cout << "Check Box 1 UNCHECKED.\n\n";
    }
}

void Player_GUI::check2_toggled()
{
    if (window)
    {
        if (check2->get_active())
            std::cout << "Check Box 2 checked.\n\n";
        else
            std::cout << "Check Box 2 UNCHECKED.\n\n";
    }
}

void Player_GUI::check3_toggled()
{
    if (window)
    {
        if (check3->get_active())
            std::cout << "Check Box 3 checked.\n\n";
        else
            std::cout << "Check Box 3 UNCHECKED.\n\n";
    }
}

void Player_GUI::check4_toggled()
{
    if (window)
    {
        if (check4->get_active())
            std::cout << "Check Box 4 checked.\n\n";
        else
            std::cout << "Check Box 4 UNCHECKED.\n\n";
    }
}

void Player_GUI::check5_toggled()
{
    if (window)
    {
        if (check5->get_active())
            std::cout << "Check Box 5 checked.\n\n";
        else
            std::cout << "Check Box 5 UNCHECKED.\n\n";
    }
}

void Player_GUI::on_check_button_clicked()
{
    if(window)
    {
        if (button_check)
        {
          std::cout << "Check Button was clicked.\n\n";
          std::string event = "check";
          client->event = event;
        }
    }
}

void Player_GUI::on_call_button_clicked()
{
    if (window)
    {
        if (button_call)
        {
          std::cout << "Call Button was clicked.\n\n";
          std::string event = "call";
          client->event = event;
        }
    }
}

void Player_GUI::on_fold_button_clicked()
{
    if (window)
    {
        if (button_fold)
        {
          std::cout << "Fold Button was clicked.\n\n";
          std::string event = "fold";
          client->event = event;
        }
    }
}

void Player_GUI::on_bet_button_clicked()
{
    if (window)
    {
        if (button_bet)
        {
            int player_balance = 100;

            std::cout << "Bet Button was clicked.\n\n";

            if (player_bet_entry->get_text() == "")
                std::cout << "Error. Player bet not set.\n\n";
            else if (std::stoi(player_bet_entry->get_text(),nullptr,0) > player_balance)
                std::cout << "Error. Bet amount is greater than chip balance.\n\n";
            else
            {
                std::cout << "$" + player_bet_entry->get_text() + " bet was set.\n\n";

                int entry = std::stoi(player_bet_entry->get_text(),nullptr,0);
                client->bet = entry;
                std::string event = "bet";
                client->event = event;
            }

            player_bet_entry->set_text("");
        }
    }
}

void Player_GUI::on_raise_button_clicked()
{
    if (window)
    {
        if (button_raise)
        {
          std::cout << "Raise Button was clicked.\n\n";
          std::string event = "raise";
          client->event = event;
        }
    }
}

void Player_GUI::on_all_in_button_clicked()
{
    if (window)
    {
        if (button_all_in)
        {
          std::cout << "All-in Button was clicked.\n\n";
          std::string event = "all-in";
          client->event = event;
        }
    }
}

void Player_GUI::on_replace_button_clicked()
{
    std::string assets[52] = {"../assets/10C.png", "../assets/10D.png", "../assets/10H.png", "../assets/10S.png",
                              "../assets/9C.png", "../assets/9D.png", "../assets/9H.png", "../assets/9S.png",
                              "../assets/8C.png", "../assets/8D.png", "../assets/8H.png", "../assets/8S.png",
                              "../assets/7C.png", "../assets/7D.png", "../assets/7H.png", "../assets/7S.png",
                              "../assets/6C.png", "../assets/6D.png", "../assets/6H.png", "../assets/6S.png",
                              "../assets/5C.png", "../assets/5D.png", "../assets/5H.png", "../assets/5S.png",
                              "../assets/4C.png", "../assets/4D.png", "../assets/4H.png", "../assets/4S.png",
                              "../assets/3C.png", "../assets/3D.png", "../assets/3H.png", "../assets/3S.png",
                              "../assets/2C.png", "../assets/2D.png", "../assets/2H.png", "../assets/2S.png",
                              "../assets/AC.png", "../assets/AD.png", "../assets/AH.png", "../assets/AS.png",
                              "../assets/KC.png", "../assets/KD.png", "../assets/KH.png", "../assets/KS.png",
                              "../assets/QC.png", "../assets/QD.png", "../assets/QH.png", "../assets/QS.png",
                              "../assets/JC.png", "../assets/JD.png", "../assets/JH.png", "../assets/JS.png"
                             };

    if (window)
    {
        if (button_replace)
        {
            std::cout << "Replace Button was clicked.\n\n";
            std::string event = "replace";

            if (check1->get_active())
            {
                int randomIndex = rand() % 53;
                image_card1->set(assets[randomIndex]);
                int position = 0;
                client->replace_vector[position] = 1;
                client->event = event;

                check1->set_active(false);
            }
            if (check2->get_active())
            {
                int randomIndex = rand() % 53;
                image_card2->set(assets[randomIndex]);
                int position = 1;
                client->replace_vector[position] = 1;
                client->event = event;

                check2->set_active(false);
            }
            if (check3->get_active())
            {
                int randomIndex = rand() % 53;
                image_card3->set(assets[randomIndex]);
                int position = 2;
                client->replace_vector[position] = 1;
                client->event = event;

                check3->set_active(false);
            }
            if (check4->get_active())
            {
                int randomIndex = rand() % 53;
                image_card4->set(assets[randomIndex]);
                int position = 3;
                client->replace_vector[position] = 1;
                client->event = event;

                check4->set_active(false);
            }
            if (check5->get_active())
            {
                int randomIndex = rand() % 53;
                image_card5->set(assets[randomIndex]);
                int position = 4;
                client->replace_vector[position] = 1;
                client->event = event;

                check5->set_active(false);
            }
        }
    }
}
