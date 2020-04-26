#include <gtkmm.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "json.hpp"
#include "asio.hpp"
#include "chat_message.hpp"
#include "game_client.hpp"
#include "player_gui.hpp"

using asio::ip::tcp;
using json = nlohmann::json;
using namespace Gtk;
using namespace std;

Player_GUI *player_gui = nullptr;

void update(Player_GUI *player_gui)
{
    player_gui->label_dealer_message->set_text("Message from Dealer: "+player_gui->client->dealer_comment);
    player_gui->label_total_balance->set_text(to_string(player_gui->client->total_balance));
    player_gui->label_chat_message->set_text(player_gui->client->chat);
    player_gui->label_recommended_play->set_text(player_gui->client->recommended_play);
    player_gui->label_pot_value->set_text(to_string(player_gui->client->current_pot));
    player_gui->label_current_bet->set_text(to_string(player_gui->client->current_bet));
    player_gui->label_minimum_bet->set_text(to_string(player_gui->client->minimum_bet));

    cout<<"Updating gui"<<endl;
    cout<<player_gui->client->playersName.size()<<endl;

    if(player_gui->client->playersName.size()==1)
    {
        player_gui->label_player1_name->set_text(player_gui->client->playersName[0]);
        player_gui->label_player1_balance->set_text(to_string(player_gui->client->playersBalance[0]));
    }
    else if(player_gui->client->playersName.size()==2)
    {
        player_gui->label_player1_name->set_text(player_gui->client->playersName[0]);
        player_gui->label_player2_name->set_text(player_gui->client->playersName[1]);
        player_gui->label_player1_balance->set_text(to_string(player_gui->client->playersBalance[0]));
        player_gui->label_player2_balance->set_text(to_string(player_gui->client->playersBalance[1]));
    }
    else if(player_gui->client->playersName.size()==3)
    {
        player_gui->label_player1_name->set_text(player_gui->client->playersName[0]);
        player_gui->label_player2_name->set_text(player_gui->client->playersName[1]);
        player_gui->label_player3_name->set_text(player_gui->client->playersName[2]);
        player_gui->label_player1_balance->set_text(to_string(player_gui->client->playersBalance[0]));
        player_gui->label_player2_balance->set_text(to_string(player_gui->client->playersBalance[1]));
        player_gui->label_player3_balance->set_text(to_string(player_gui->client->playersBalance[2]));
    }
    else if(player_gui->client->playersName.size()==4)
    {
        player_gui->label_player1_name->set_text(player_gui->client->playersName[0]);
        player_gui->label_player2_name->set_text(player_gui->client->playersName[1]);
        player_gui->label_player3_name->set_text(player_gui->client->playersName[2]);
        player_gui->label_player4_name->set_text(player_gui->client->playersName[3]);
        player_gui->label_player1_balance->set_text(to_string(player_gui->client->playersBalance[0]));
        player_gui->label_player2_balance->set_text(to_string(player_gui->client->playersBalance[1]));
        player_gui->label_player3_balance->set_text(to_string(player_gui->client->playersBalance[2]));
        player_gui->label_player4_balance->set_text(to_string(player_gui->client->playersBalance[3]));
    }
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
            std::cerr << "Usage: player <host> <port>\n";
            return 1;
        }

        asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        game_client gc(io_context, endpoints);
        player_gui->client = &gc;
        gc.guiPTR = player_gui;

        std::thread t([&io_context]()
        {
            io_context.run();
        });

        kit.run(*player_gui);
        gc.close();
        t.join();
    }

    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    delete player_gui;

    return 0;
}
