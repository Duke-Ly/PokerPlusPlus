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
   return 0;
}
