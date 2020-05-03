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
    builder->get_widget("label_total_balance", label_total_balance);
    builder->get_widget("label_chat_message", label_chat_message);
    builder->get_widget("label_recommended_play", label_recommended_play);
    builder->get_widget("label_pot_value", label_pot_value);
    builder->get_widget("label_current_bet", label_current_bet);
    builder->get_widget("label_minimum_bet", label_minimum_bet);
    builder->get_widget("label_player1_name", label_player1_name);
    builder->get_widget("label_player2_name", label_player1_name);
    builder->get_widget("label_player3_name", label_player1_name);
    builder->get_widget("label_player4_name", label_player1_name);
    builder->get_widget("label_player1_balance", label_player1_balance);
    builder->get_widget("label_player2_balance", label_player1_balance);
    builder->get_widget("label_player3_balance", label_player1_balance);
    builder->get_widget("label_player4_balance", label_player1_balance);
    builder->get_widget("label_spectator1", label_spectator1);
    builder->get_widget("label_spectator2", label_spectator1);
    builder->get_widget("label_spectator3", label_spectator1);
    builder->get_widget("label_spectator4", label_spectator1);
    builder->get_widget("label_spectator5", label_spectator1);
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
    //player_bet_entry->signal_activate().connect(sigc::mem_fun(*this, &Player_GUI::player_bet_entry_activate));
    //entry_player_chat->signal_activate().connect(sigc::mem_fun(*this, &Player_GUI::entry_player_chat_activate));
    button_send->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_send_button_clicked));
    button_clear->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_clear_button_clicked));
    about_menu->signal_activate().connect(sigc::mem_fun(*this, &Player_GUI::on_about_dialog_activate_link));
    help_menu->signal_activate().connect(sigc::mem_fun(*this, &Player_GUI::on_help_menu_activate));
    help_close->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_help_close_clicked));

    window->set_title("Poker++");
    window->override_background_color(Gdk::RGBA{"green"});
    player_name_dialog->show();
}

Player_GUI::~Player_GUI()
{
    //delete clientPTR;
    //delete player_name_dialog;
    //delete window;
    delete button_check;
    delete button_call;
    delete button_fold;
    delete button_bet;
    delete button_raise;
    delete button_all_in;
    delete button_replace;
    delete check1;
    delete check2;
    delete check3;
    delete check4;
    delete check5;
    delete player_bet_entry;
    delete entry_player_chat;
    delete button_send;
    delete button_clear;
    delete image_card1;
    delete image_card2;
    delete image_card3;
    delete image_card4;
    delete image_card5;
    delete help_menu;
    delete about_menu;
    delete label_dealer_message;
    delete label_total_balance;
    delete label_chat_message;
    delete label_recommended_play;
    delete label_pot_value;
    delete label_current_bet;
    delete label_minimum_bet;
    delete label_player1_name;
    delete label_player2_name;
    delete label_player3_name;
    delete label_player4_name;
    delete label_player1_balance;
    delete label_player2_balance;
    delete label_player3_balance;
    delete label_player4_balance;
    delete label_spectator1;
    delete label_spectator2;
    delete label_spectator3;
    delete label_spectator4;
    delete label_spectator5;
    delete about_dialog;
    delete help_dialog;
    delete help_close;
    delete name_dialog_enter;
    delete entry_player_name;
}

void Player_GUI::on_name_dialog_enter_clicked()
{
    string playerName;

    if(entry_player_name->get_text() == "")
    {
        entry_player_name->set_text("**INVALID** Please enter name");
    }
    else if(entry_player_name->get_text() == "**INVALID** Please enter name")
    {
        entry_player_name->set_text("**INVALID** Please enter name");
    }
    else
    {
        random_generator gen;
        uuid id = gen();
        playerName = entry_player_name->get_text();
        clientPTR->name = playerName;
        cout<<"Welcome "<<playerName<<"!\n\n";
        string uuid = to_string(id);
        clientPTR->uuid = uuid;
        clientPTR->event = "join";
        clientPTR->send();
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

void Player_GUI::on_send_button_clicked()
{
    if(window)
    {
        if(button_send)
        {
            string temp = entry_player_chat->get_text();
            cout<<"Send Button was clicked"<<endl;
            clientPTR->chat = "From " + clientPTR->name + ": " + temp;
            clientPTR->event = "chat";
            entry_player_chat->set_text("");
            clientPTR->send();
        }
    }
}

void Player_GUI::on_clear_button_clicked()
{
    if(window)
    {
        if(button_clear)
        {
            cout<<"Clear Button was clicked"<<endl;
            entry_player_chat->set_text("");
        }
    }
}

/*
void Player_GUI::entry_player_chat_activate()
{
    if(window)
    {
        if(entry_player_chat)
        {
            cout<<entry_player_chat->get_text() + "\n\n";
            entry_player_chat->set_text("");
        }
    }
}

void Player_GUI::player_bet_entry_activate()
{
    if(window)
    {
        if(player_bet_entry)
        {
            int player_balance = 100;

            int entry = stoi(player_bet_entry->get_text(),nullptr,0);
            player_balance = player_balance - entry;

            cout<<"$" + player_bet_entry->get_text() + " bet was set"<<endl;
            cout<<"current balance is "<<player_balance<<"\n\n";

            player_bet_entry->set_text("");
        }

    }
}
*/

void Player_GUI::check1_toggled()
{
    if(window)
    {
        if(check1->get_active())
            cout<<"Check Box 1 checked"<<endl;
        else
            cout<<"Check Box 1 UNCHECKED"<<endl;
    }
}

void Player_GUI::check2_toggled()
{
    if(window)
    {
        if(check2->get_active())
            cout<<"Check Box 2 checked"<<endl;
        else
            cout<<"Check Box 2 UNCHECKED"<<endl;
    }
}

void Player_GUI::check3_toggled()
{
    if(window)
    {
        if(check3->get_active())
            cout<<"Check Box 3 checked"<<endl;
        else
            cout<<"Check Box 3 UNCHECKED"<<endl;
    }
}

void Player_GUI::check4_toggled()
{
    if(window)
    {
        if(check4->get_active())
            cout<<"Check Box 4 checked"<<endl;
        else
            cout<<"Check Box 4 UNCHECKED"<<endl;
    }
}

void Player_GUI::check5_toggled()
{
    if(window)
    {
        if(check5->get_active())
            cout<<"Check Box 5 checked"<<endl;
        else
            cout<<"Check Box 5 UNCHECKED"<<endl;
    }
}

void Player_GUI::on_check_button_clicked()
{
    if(window)
    {
        if(button_check)
        {
            cout<<"Check Button was clicked"<<endl;
            if(clientPTR->turn.compare(clientPTR->uuid))
            {
                cout<<"Not valid turn"<<endl;
                clientPTR->dealer_comment = "It is not your turn, please wait";
                update(this, this->clientPTR);
                return;
            }

            if(clientPTR->minimum_bet==clientPTR->current_bet)
            {
                clientPTR->event = "check";
                clientPTR->send();
            }
            else
            {
                cout<<"Check was not valid"<<endl;
                clientPTR->dealer_comment = "You cannot check!";
                update(this, this->clientPTR);
            }
        }
    }
}

void Player_GUI::on_call_button_clicked()
{
    if(window)
    {
        if(button_call)
        {
            cout<<"Call Button was clicked"<<endl;
            if(clientPTR->turn.compare(clientPTR->uuid))
            {
                cout<<"Not valid turn"<<endl;
                clientPTR->dealer_comment = "It is not your turn, please wait";
                update(this, this->clientPTR);
                return;
            }

            if((clientPTR->minimum_bet>clientPTR->current_bet)
                    &&(clientPTR->minimum_bet-clientPTR->current_bet)<=clientPTR->total_balance)
            {
                clientPTR->event = "call";
                clientPTR->send();
            }
            else
            {
                cout<<"Call was not valid"<<endl;
                clientPTR->dealer_comment = "You cannot call!";
                update(this, this->clientPTR);
            }
        }
    }
}

void Player_GUI::on_fold_button_clicked()
{
    if(window)
    {
        if(button_fold)
        {
            cout<<"Fold Button was clicked"<<endl;
            if(clientPTR->turn.compare(clientPTR->uuid))
            {
                cout<<"Not valid turn"<<endl;
                clientPTR->dealer_comment = "It is not your turn, please wait";
                update(this, this->clientPTR);
                return;
            }

            clientPTR->event = "fold";
            clientPTR->send();
        }
    }
}

void Player_GUI::on_bet_button_clicked()
{
    if(window)
    {
        if(button_bet)
        {
            cout<<"Bet Button was clicked"<<endl;
            if(clientPTR->turn.compare(clientPTR->uuid))
            {
                cout<<"Not valid turn"<<endl;
                clientPTR->dealer_comment = "It is not your turn, please wait";
                update(this, this->clientPTR);
                return;
            }

            if(player_bet_entry->get_text() == "")
            {
                cout<<"Error. Player bet not set"<<endl;
                clientPTR->dealer_comment = "Bet not set!";
                update(this, this->clientPTR);
            }
            else if(stoi(player_bet_entry->get_text(),nullptr,0) > clientPTR->total_balance)
            {
                cout<<"Error. Bet amount is greater than chip balance"<<endl;
                clientPTR->dealer_comment = "Bet amount is greater than chip balance!";
                update(this, this->clientPTR);
            }
            else if(clientPTR->minimum_bet==1&&clientPTR->current_bet==0
                    &&stoi(player_bet_entry->get_text(),nullptr,0)==1)
            {
                cout<<"$" + player_bet_entry->get_text() + " bet was set"<<endl;

                clientPTR->bet = stoi(player_bet_entry->get_text(),nullptr,0);
                clientPTR->event = "bet";
                clientPTR->send();
            }
            else if(clientPTR->minimum_bet==clientPTR->current_bet
                    &&stoi(player_bet_entry->get_text(),nullptr,0)<=clientPTR->total_balance)
            {
                cout<<"$" + player_bet_entry->get_text() + " bet was set"<<endl;

                clientPTR->bet = stoi(player_bet_entry->get_text(),nullptr,0);
                clientPTR->event = "bet";
                clientPTR->send();
            }
            else
            {
                cout<<"Bet was not valid"<<endl;
                clientPTR->dealer_comment = "You cannot Bet!";
                update(this, this->clientPTR);
            }

            player_bet_entry->set_text("");
        }
    }
}

void Player_GUI::on_raise_button_clicked()
{
    if(window)
    {
        if(button_raise)
        {
            cout<<"Raise Button was clicked"<<endl;
            if(clientPTR->turn.compare(clientPTR->uuid))
            {
                cout<<"Not valid turn"<<endl;
                clientPTR->dealer_comment = "It is not your turn, please wait";
                update(this, this->clientPTR);
                return;
            }

            if((clientPTR->minimum_bet>clientPTR->current_bet)
                    &&(clientPTR->minimum_bet-clientPTR->current_bet)<clientPTR->total_balance
                    &&stoi(player_bet_entry->get_text(),nullptr,0)<=clientPTR->total_balance)
            {
                cout<<"Raise is valid"<<endl;

                clientPTR->raise = stoi(player_bet_entry->get_text(),nullptr,0);
                clientPTR->event = "raise";
                clientPTR->send();
            }
            else
            {
                cout<<"Raise was not valid"<<endl;
                clientPTR->dealer_comment = "You cannot Raise!";
                update(this, this->clientPTR);
            }
        }
    }
}

void Player_GUI::on_all_in_button_clicked()
{
    if(window)
    {
        if(button_all_in)
        {
            cout<<"All-in Button was clicked"<<endl;
            if(clientPTR->turn.compare(clientPTR->uuid))
            {
                cout<<"Not valid turn"<<endl;
                clientPTR->dealer_comment = "It is not your turn, please wait";
                update(this, this->clientPTR);
                return;
            }

            if(0<clientPTR->total_balance)
            {
                cout<<"All-in is valid"<<endl;

                clientPTR->event = "all-in";
                clientPTR->send();
            }
            else
            {
                cout<<"All-in was not valid"<<endl;
                clientPTR->dealer_comment = "You cannot All-in!";
                update(this, this->clientPTR);
            }
        }
    }
}

void Player_GUI::on_replace_button_clicked()
{
    if(window)
    {
        if(button_replace)
        {
            cout<<"Replace Button was clicked"<<endl;

            if(clientPTR->turn.compare(clientPTR->uuid))
            {
                cout<<"Not valid turn"<<endl;
                clientPTR->dealer_comment = "It is not your turn, please wait";
                update(this, this->clientPTR);
                return;
            }

            clientPTR->event = "replace";

            if(check1->get_active())
            {
                clientPTR->replace_vector[0] = 1;
                check1->set_active(false);
            }
            if(check2->get_active())
            {
                clientPTR->replace_vector[1] = 1;
                check2->set_active(false);
            }
            if(check3->get_active())
            {
                clientPTR->replace_vector[2] = 1;
                check3->set_active(false);
            }
            if(check4->get_active())
            {
                clientPTR->replace_vector[3] = 1;
                check4->set_active(false);
            }
            if(check5->get_active())
            {
                clientPTR->replace_vector[4] = 1;
                check5->set_active(false);
            }

            clientPTR->send();
        }
    }
}
