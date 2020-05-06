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
Player_GUI *player_gui = 0;
game_client *client = nullptr;

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            cerr<<"Usage: player <host> <port>\n";
            return 1;
        }

        Gtk::Main kit(argc,argv);
        Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui_mockup.glade");
        builder->get_widget_derived("window", player_gui);

        asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        game_client c(io_context, endpoints);
        client = &c;
        player_gui->clientPTR = client;
        c.guiPTR = player_gui;

        thread t([&io_context]()
        {
            io_context.run();
        });

        kit.run(*player_gui);
        c.close();
        t.join();
    }

    catch (exception& e)
    {
        cerr<<"Exception: "<<e.what()<<"\n";
    }

    delete player_gui;
    //delete client;
    player_gui = nullptr;
    client = nullptr;

    return 0;
}
