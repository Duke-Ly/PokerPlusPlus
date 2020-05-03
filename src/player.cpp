#include <gtkmm.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <string>
#include "json.hpp"
#include "asio.hpp"
#include "chat_message.hpp"
#include "game_client.hpp"
#include "player_gui.hpp"

using asio::ip::tcp;
using json = nlohmann::json;
using namespace Gtk;
using namespace std;

// Global Pointers
Player_GUI *player_gui = nullptr;
game_client* client = nullptr;

void update(Player_GUI *player_gui, game_client* client)
{
    if(client->dealer_comment.compare(""))
        player_gui->label_dealer_message->set_text(client->dealer_comment);
    player_gui->label_total_balance->set_text(to_string(client->total_balance));
    if(client->chat.compare(""))
        player_gui->label_chat_message->set_text("Chat "+client->chat);
    if(client->recommended_play.compare(""))
        player_gui->label_recommended_play->set_text("Recommended Play: " + client->recommended_play);
    player_gui->label_pot_value->set_text("$"+to_string(client->current_pot));
    player_gui->label_current_bet->set_text(to_string(client->current_bet));
    player_gui->label_minimum_bet->set_text(to_string(client->minimum_bet));

    cout<<"Updating gui"<<endl;
    cout<<client->playersName.size()<<endl;

    if(client->playersName.size()==1)
    {
        player_gui->label_player1_name->set_text(client->playersName[0]);
        player_gui->label_player1_balance->set_text("Balance: $" + to_string(client->playersBalance[0]));
    }
    else if(client->playersName.size()==2)
    {
        player_gui->label_player1_name->set_text(client->playersName[0]);
        player_gui->label_player2_name->set_text(client->playersName[1]);
        player_gui->label_player1_balance->set_text("Balance: $" + to_string(client->playersBalance[0]));
        player_gui->label_player2_balance->set_text("Balance: $" + to_string(client->playersBalance[1]));
    }
    else if(client->playersName.size()==3)
    {
        player_gui->label_player1_name->set_text(client->playersName[0]);
        player_gui->label_player2_name->set_text(client->playersName[1]);
        player_gui->label_player3_name->set_text(client->playersName[2]);
        player_gui->label_player1_balance->set_text("Balance: $" + to_string(client->playersBalance[0]));
        player_gui->label_player2_balance->set_text("Balance: $" + to_string(client->playersBalance[1]));
        player_gui->label_player3_balance->set_text("Balance: $" + to_string(client->playersBalance[2]));
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

int main(int argc, char* argv[])
{
    Main kit(argc,argv);
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui_mockup.glade");
    builder->get_widget_derived("window", player_gui);

    try
    {
        if (argc != 3)
        {
            cerr<<"Usage: player <host> <port>\n";
            return 1;
        }

        asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        client = new game_client(io_context, endpoints);
        player_gui->clientPTR = client;
        client->guiPTR = player_gui;

        thread t([&io_context]()
        {
            io_context.run();
        });

        kit.run(*player_gui);
        client->close();
        t.join();
    }

    catch (exception& e)
    {
        cerr<<"Exception: "<<e.what()<<"\n";
    }

    //delete player_gui;
    //delete client;
    player_gui = nullptr;
    client = nullptr;

    return 0;
}
