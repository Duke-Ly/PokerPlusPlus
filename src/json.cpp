/*
 * CSE3310-003 Spring 2020
 * Project Prototyping
 * Duke Ly
 * 1001296968
 * 02/04/2020
*/

#include <iostream>
#include <ctime>
#include "json.hpp"

using json = nlohmann::json;

int main(int argc,char *argv[])
{
// SERVER
    json server = {
      {"cards_requested", 1},
      {"chat", "this is a string of text requesting a chat message"},
      {"current_bet", 1.0},
      {"event", "**some event**"},
      {"from", {
        {"name", "**name**"},
        {"uuid", "**uuid**"}}},
      {"total_bet", 1.0}
    };

// CLIENT
    json client = {
      {"chat",
               {"line1",
                "line2",
                "lineX"}},
      {"dealer_comment", "**dealer comment**"},
      {"hand", {
          {"bet", "1"},
          {"cards", {"card1",
                     "card2",
                     "card3",
                     "card4",
                     "card5"}},
      {"current_bet", "1"},
      {"name", "**name**"},
      {"uuid", "**uuid**"},

      {"bet", 1},
      {"cards", {"card1",
               "card2",
               "card3",
               "card4",
               "card5"}},
    {"current_bet", 1.0},
    {"name", "player name"},
    {"uuid", "player uuid"}
  }}};


    std::cout << server.dump(1) << "\n\n\n";
    std::cout << client.dump(1) << '\n';

/*
    json j;
    j["name"] = "duke";
    j["id"] = "1001296968";
    time_t tt;
    struct tm * ti;
    time (&tt);
    ti = localtime(&tt);
    std::string str = asctime(ti);
    str[str.length()-1] = ' ';
    j["chrono"] = str;
    std::string s = j.dump();
    std::cout << s << "\n";
*/

   return 0;
}
