// This file describes the format of the JSON strings to be used
// for communicating between the dealer and the player.

#include <iostream>
#include <string>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

int main(int argc, char *argv[])
{
    nlohmann::json to_dealer;
    to_dealer["from"] = { {"uuid","3f96b414-9ac9-40b5-8007-90d0e771f0d0"}, {"name","Bud"} };
    to_dealer["event"] = "check";        // "check","bet","call","raise","fold","all_in","replace","chat","join"
    to_dealer["replace_vector"] = {0, 0, 0, 0, 0};
    to_dealer["bet"] = 1.00;
    to_dealer["raise"] = 1.00;
    to_dealer["chat"] = "this is a string of text representing a chat message";

    //cout<<"to dealer:"<<endl;
    //cout<<to_dealer.dump(2)<<endl;

    if(to_dealer["event"]=="check")
    {
        cout<<to_dealer["from"].at("name")<<endl;
        string m = string(to_dealer["from"].at("name")) + " has joined.";
        cout<<m<<endl;
    }

    nlohmann::json to_player;  // represents the entire game state.  sent to all players
    to_player["turn"] = "3f96b414-9ac9-40b5-8007-90d0e771f0d0";   // UUID of the current player.
    to_player["chat"] = {"this is one line","this is another","and so is this"};
    to_player["dealer_comment"] = "fred has raised and received 2 new cards";
    to_player["recommended_play"] = "you should fold";
    to_player["current_pot"] = 5.00;
    to_player["minimum_bet"] = 1.00;
    to_player["hand"] =
    {
        {{"total_balance",100}, {"current_bet",10}, {"uuid","3f96b414-9ac9-40b5-8007-90d0e771f0d0"}, {"name","Bud"},{"cards",{"acespades","10hearts","9clubs","2diamonds","kinghearts"}}},
        {{"total_balance",100}, {"current_bet",1}, {"uuid","3f96b414-9ac9-40b5-8007-20d0e771f0d0"}, {"name","Donald"},{"cards",{"acehearts","10spades","9clubs","2clubs","jackhearts"}}},
        {{"total_balance",100}, {"current_bet",5}, {"uuid","3f96b414-9ac9-40b5-8007-30d0e771f0d0"}, {"name","Ann"},{"cards",{"aceclubs","10diamonds","9clubs","2hearts","queenhearts"}}},
        {{"total_balance",100}, {"current_bet",0}, {"uuid","3f96b414-9ac9-40b5-8007-40d0e771f0d0"}, {"name","Melania"},{"cards",{"acediamonds","10clubs","9clubs","2spades","kinghearts"}}}
    };

    to_player["hand"].push_back({{"total_balance",0}, {"current_bet",0}, {"uuid","3f96b414-9ac9-40b5-8007-000000000000"}, {"name","0"},{"cards",{"0","0","0","0","0"}}});
    //cout<<"to player:"<<endl;
    //cout<<to_player.dump(2) <<endl;
    cout<<to_player["hand"][1].at("uuid")<<endl;

    return 0;
}
