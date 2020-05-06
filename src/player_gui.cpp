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
    builder->get_widget("label_player2_name", label_player2_name);
    builder->get_widget("label_player3_name", label_player3_name);
    builder->get_widget("label_player4_name", label_player4_name);
    builder->get_widget("label_player1_balance", label_player1_balance);
    builder->get_widget("label_player2_balance", label_player2_balance);
    builder->get_widget("label_player3_balance", label_player3_balance);
    builder->get_widget("label_player4_balance", label_player4_balance);
    builder->get_widget("label_spectator1", label_spectator1);
    builder->get_widget("label_spectator2", label_spectator2);
    builder->get_widget("label_spectator3", label_spectator3);
    builder->get_widget("label_spectator4", label_spectator4);
    builder->get_widget("label_spectator5", label_spectator5);
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
    button_send->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_send_button_clicked));
    button_clear->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_clear_button_clicked));
    about_menu->signal_activate().connect(sigc::mem_fun(*this, &Player_GUI::on_about_dialog_activate_link));
    help_menu->signal_activate().connect(sigc::mem_fun(*this, &Player_GUI::on_help_menu_activate));
    help_close->signal_clicked().connect(sigc::mem_fun(*this, &Player_GUI::on_help_close_clicked));

    window->set_title("Poker++");
    window->override_background_color(Gdk::RGBA{"green"});
    player_name_dialog->show();
    //player_name_dialog->run();
}

void update(Player_GUI *player_gui, game_client *client)
{
    cout<<"Updating gui"<<endl;

    if(client->dealer_comment.compare(""))
        player_gui->label_dealer_message->set_text(client->dealer_comment);
    if(client->chat.compare(""))
        player_gui->label_chat_message->set_text("Chat "+client->chat);
    if(client->recommended_play.compare(""))
        player_gui->label_recommended_play->set_text("Recommended Play: " + client->recommended_play);

    player_gui->label_total_balance->set_text(to_string(client->total_balance));
    player_gui->label_pot_value->set_text("$"+to_string(client->current_pot));
    player_gui->label_current_bet->set_text(to_string(client->current_bet));
    player_gui->label_minimum_bet->set_text(to_string(client->minimum_bet));

    if(client->playersName.size()==1)
    {
        player_gui->label_player1_name->set_text(client->playersName[0]);
        player_gui->label_player2_name->set_text("Player Name");
        player_gui->label_player3_name->set_text("Player Name");
        player_gui->label_player4_name->set_text("Player Name");
        player_gui->label_player1_balance->set_text("Balance: $" + to_string(client->playersBalance[0]));
        player_gui->label_player2_balance->set_text("Chip Balance");
        player_gui->label_player3_balance->set_text("Chip Balance");
        player_gui->label_player4_balance->set_text("Chip Balance");
    }
    else if(client->playersName.size()==2)
    {
        player_gui->label_player1_name->set_text(client->playersName[0]);
        player_gui->label_player2_name->set_text(client->playersName[1]);
        player_gui->label_player3_name->set_text("Player Name");
        player_gui->label_player4_name->set_text("Player Name");
        player_gui->label_player1_balance->set_text("Balance: $" + to_string(client->playersBalance[0]));
        player_gui->label_player2_balance->set_text("Balance: $" + to_string(client->playersBalance[1]));
        player_gui->label_player3_balance->set_text("Chip Balance");
        player_gui->label_player4_balance->set_text("Chip Balance");
    }
    else if(client->playersName.size()==3)
    {
        player_gui->label_player1_name->set_text(client->playersName[0]);
        player_gui->label_player2_name->set_text(client->playersName[1]);
        player_gui->label_player3_name->set_text(client->playersName[2]);
        player_gui->label_player4_name->set_text("Player Name");
        player_gui->label_player1_balance->set_text("Balance: $" + to_string(client->playersBalance[0]));
        player_gui->label_player2_balance->set_text("Balance: $" + to_string(client->playersBalance[1]));
        player_gui->label_player3_balance->set_text("Balance: $" + to_string(client->playersBalance[2]));
        player_gui->label_player4_balance->set_text("Chip Balance");
    }
    else if(client->playersName.size()==4)
    {
        player_gui->label_player1_name->set_text(client->playersName[0]);
        player_gui->label_player2_name->set_text(client->playersName[1]);
        player_gui->label_player3_name->set_text(client->playersName[2]);
        player_gui->label_player4_name->set_text(client->playersName[3]);
        player_gui->label_player1_balance->set_text("Balance: $" + to_string(client->playersBalance[0]));
        player_gui->label_player2_balance->set_text("Balance: $" + to_string(client->playersBalance[1]));
        player_gui->label_player3_balance->set_text("Balance: $" + to_string(client->playersBalance[2]));
        player_gui->label_player4_balance->set_text("Balance: $" + to_string(client->playersBalance[3]));
    }

    player_gui->image_card1->set("../assets/"+client->cards[0]+".png");
    player_gui->image_card2->set("../assets/"+client->cards[1]+".png");
    player_gui->image_card3->set("../assets/"+client->cards[2]+".png");
    player_gui->image_card4->set("../assets/"+client->cards[3]+".png");
    player_gui->image_card5->set("../assets/"+client->cards[4]+".png");

    // To Do: add support for spectators
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
                update(this, clientPTR);
                return;
            }

            if((clientPTR->minimum_bet==clientPTR->current_bet)||(clientPTR->total_balance==0))
            {
                clientPTR->event = "check";
                clientPTR->send();
            }
            else
            {
                cout<<"Check was not valid"<<endl;
                clientPTR->dealer_comment = "You cannot check!";
                update(this, clientPTR);
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
                update(this, clientPTR);
                return;
            }

            if((clientPTR->minimum_bet>clientPTR->current_bet)
                    &&((clientPTR->minimum_bet-clientPTR->current_bet)<=clientPTR->total_balance))
            {
                clientPTR->event = "call";
                clientPTR->send();
            }
            else
            {
                cout<<"Call was not valid"<<endl;
                clientPTR->dealer_comment = "You cannot call!";
                update(this, clientPTR);
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
                update(this, clientPTR);
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
                update(this, clientPTR);
                return;
            }

            if(player_bet_entry->get_text() == "")
            {
                cout<<"Error. Player bet not set"<<endl;
                clientPTR->dealer_comment = "Bet not set!";
                update(this, clientPTR);
            }
            else if(stoi(player_bet_entry->get_text(),nullptr,0) > clientPTR->total_balance)
            {
                cout<<"Error. Bet amount is greater than chip balance"<<endl;
                clientPTR->dealer_comment = "Bet amount is greater than chip balance!";
                update(this, clientPTR);
            }
            else if((clientPTR->minimum_bet==1)&&(clientPTR->current_bet==0)
                    &&(stoi(player_bet_entry->get_text(),nullptr,0)==1))
            {
                cout<<"$" + player_bet_entry->get_text() + " bet was set"<<endl;

                clientPTR->bet = stoi(player_bet_entry->get_text(),nullptr,0);
                clientPTR->event = "bet";
                clientPTR->send();
            }
            else if((clientPTR->minimum_bet==clientPTR->current_bet)
                    &&(stoi(player_bet_entry->get_text(),nullptr,0)<=clientPTR->total_balance))
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
                update(this, clientPTR);
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
                update(this, clientPTR);
                return;
            }

            if((clientPTR->minimum_bet>clientPTR->current_bet)
                    &&((clientPTR->minimum_bet-clientPTR->current_bet)<(clientPTR->total_balance))
                    &&(stoi(player_bet_entry->get_text(),nullptr,0)<=(clientPTR->total_balance)))
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
                update(this, clientPTR);
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
                update(this, clientPTR);
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
                update(this, clientPTR);
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
                update(this, clientPTR);
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
            for(unsigned int i=0; i<5; i++)
            {
                clientPTR->replace_vector[i] = 0;
            }
        }
    }
}
