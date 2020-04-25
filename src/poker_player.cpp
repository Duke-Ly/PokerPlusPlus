#include <cstdlib>
#include <memory>
#include <vector>
#include "player.hpp"
#include "chat_message.hpp"
#include "poker_table.hpp"
#include "asio.hpp"
#include "json.hpp"
#include "poker_player.hpp"

using asio::ip::tcp;
using namespace std;
using json = nlohmann::json;

Poker_Player::Poker_Player(tcp::socket socket, Poker_Table& table)
    : socket_(move(socket)),
      table_(table)
{
    cout<<"Creating a Poker_Player"<<endl;
}

Poker_Player::~Poker_Player() {};

void Poker_Player::start()
{
    table_.join(shared_from_this());
    do_read_header();
}

void Poker_Player::deliver(const chat_message& msg)
{
    bool write_in_progress = !write_msg_.empty();
    write_msg_.push_back(msg);
    if (!write_in_progress)
    {
        do_write();
    }
}

void Poker_Player::do_read_header()
{
    auto self(shared_from_this());
    asio::async_read(socket_,
                     asio::buffer(read_msg_.data(), chat_message::header_length),
                     [this, self](error_code ec, size_t /*length*/)
    {
        if (!ec && read_msg_.decode_header())
        {
            do_read_body();
        }
        else
        {
            table_.leave(shared_from_this());
        }
    });
}

void Poker_Player::do_read_body()
{
    auto self(shared_from_this());
    asio::async_read(socket_,
                     asio::buffer(read_msg_.body(), read_msg_.body_length()),
                     [this, self](error_code ec, size_t /*length*/)
    {
        if(!ec)
        {
            json to_dealer = json::parse(string(read_msg_.body()));
            json to_player;  // represents the entire game state.  sent to all players
            to_player["turn"] = nullptr;//table_.dealer.current_player->playerUUID;
            to_player["chat"] = "Table chat message";
            to_player["dealer_comment"] = "Waiting for dealer";
            to_player["recommended_play"] = "Waiting for dealer";
            to_player["current_pot"] = table_.current_pot;
            to_player["minimum_bet"] = table_.minimum_bet;
            to_player["hand"] = {};

            if(to_dealer["event"]=="join")
            {
                string n = string(to_dealer["from"].at("name")) + " has joined.";
                cout<<n<<endl;
                self->name = string(to_dealer["from"].at("name"));
                self->playerUUID = string(to_dealer["from"].at("uuid"));
                to_player["dealer_comment"] = n;
            }
            else if(to_dealer["event"]=="chat"&&table_.game_active&&(table_.game_state==WAITING))
            {
                if(to_dealer["chat"]=="start")
                {
                    table_.game_state = ANTE;
                    cout<<"game_state = ANTE"<<endl;
                    table_.dealer.current_player = *table_.players.begin();
                    to_player["turn"] = table_.dealer.current_player->playerUUID;
                    cout<<"current player turn: "<<table_.dealer.current_player->playerUUID<<endl;
                    string str = "Waiting for " + table_.dealer.current_player->name + " turn";
                    to_player["dealer_comment"] = str;
                    str = "";
                    str = "Bet the ant of $" + to_string(table_.minimum_bet);
                    to_player["recommended_play"] = str;
                }
            }
            else if((table_.game_state==ANTE)
                    &&(to_dealer["from"].at("uuid")==table_.dealer.current_player->playerUUID)
                    &&(to_dealer["event"]!="chat"))
            {
                cout<<"event is valid"<<endl;
                if((to_dealer["event"]=="bet")
                   &&(to_dealer["bet"]<=table_.dealer.current_player->total_balance)
                   &&(to_dealer["bet"]==table_.minimum_bet))
                {
                    cout<<"bet is valid"<<endl;
                    table_.dealer.current_player->total_balance -= (int) to_dealer["bet"];
                    table_.dealer.current_player->current_bet = (int) to_dealer["bet"];
                }
            }
            else if((to_dealer["event"]=="chat")&&table_.game_active)
            {
                string str = "Waiting for " + table_.dealer.current_player->name + " turn";
                to_player["dealer_comment"] = str;
                to_player["recommended_play"] = str;
                cout<<"from: "<<string(to_dealer["from"])<<endl;
                cout<<"chat: "<<string(to_dealer["chat"])<<endl;
                string msg = "from: " + string(to_dealer["from"]) + "\n" +
                             "chat: " + string(to_dealer["chat"]);
                to_player["chat"] = msg;
            }
            else
            {
                string str = "Waiting for " + table_.dealer.current_player->name + " turn";
                to_player["dealer_comment"] = str;
                to_player["recommended_play"] = str;
            }

            if(table_.players.size() >= 2 && !table_.game_active)
            {
                table_.game_active = true;
                cout<<"game is now active with "<<table_.players.size()<<" players"<<endl;
                to_player["recommended_play"] = "Send 'start' in the chat message to start the game";
            }
            else
                table_.game_active = false;

            set<player_ptr>::iterator it;
            for(it=table_.players.begin(); it!=table_.players.end(); ++it)
            {
                to_player["hand"].push_back(
                {
                    {"total_balance",(*it)->total_balance},
                    {"current_bet",(*it)->current_bet},
                    {"uuid",(*it)->playerUUID},
                    {"name",(*it)->name},
                    {"cards",(*it)->playerHand.cardsVector()}});
            }
            cout<<"to player:"<<endl;
            cout<<to_player.dump()<<endl;
            string t = to_player.dump();
            chat_message sending;
            memcpy(sending.body(), t.c_str(), t.size());
            sending.body_length(t.size());
            sending.encode_header();
            table_.deliver(sending);
            do_read_header();
        }
        else
        {
            table_.leave(shared_from_this());
        }
    });
}

void Poker_Player::do_write()
{
    auto self(shared_from_this());
    asio::async_write(socket_,
                      asio::buffer(write_msg_.front().data(), write_msg_.front().length()),
                      [this, self](std::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            write_msg_.pop_front();
            if (!write_msg_.empty())
            {
                do_write();
            }
        }
        else
        {
            table_.leave(shared_from_this());
        }
    });
}
