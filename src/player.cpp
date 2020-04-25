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

int main(int argc, char* argv[])
{
    Main kit(argc,argv);
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui_mockup.glade");
    Player_GUI *player_gui = nullptr;
    builder->get_widget_derived("window", player_gui);

    if (argc != 3)
    {
        std::cerr << "Usage: player <host> <port>\n";
        return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    game_client gc(io_context, endpoints);

    std::thread t([&io_context]()
    {
        io_context.run();
    });
    kit.run(*player_gui);
    gc.close();
    t.join();

    delete player_gui;
    return 0;
}
