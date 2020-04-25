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

    std::thread t([&io_context]()
    {
        io_context.run();
    });

    kit.run(*player_gui);

    //delete player_gui;

//*********************************************************************************************
// CLIENT CODE ADDED BELOW

char line[chat_message::max_body_length + 1];
while (std::cin.getline(line, chat_message::max_body_length + 1))
{
  chat_message msg;


  nlohmann::json to_dealer;
  to_dealer["from"] = { {"uuid","3f96b414-9ac9-40b5-8007-90d0e771f0d0"} , {"name","Bud"} };
  to_dealer["event"] = "stand";        // "check","call","fold","bet","raise","all-in","replaceVector","replace"
  to_dealer["cards_requested"] = 3;    // optional, number of cards requested, 1 to 5
  to_dealer["current_bet"] = 1.00;
  to_dealer["total_bet"] = 5.00;
  to_dealer["chat"] = std::string(line);
  //std::cout << "to dealer:" << std::endl;
  //std::cout << to_dealer.dump(2) << std::endl;

  std::string t = to_dealer.dump();

  msg.body_length(t.size());
  std::memcpy(msg.body(), t.c_str() , msg.body_length());
  msg.encode_header();
  gc.write(msg);
}

  //kit.run(*player_gui);
  gc.close();
  t.join();

  //delete player_gui;
}

catch (std::exception& e)
{
  std::cerr << "Exception: " << e.what() << "\n";
}

// END OF CLIENT CODE ADDED
//*******************************************************************************************************

    //kit.run(*player_gui);
    /*
    gc.close();
    t.join();
*/

    delete player_gui;

    return 0;
}
