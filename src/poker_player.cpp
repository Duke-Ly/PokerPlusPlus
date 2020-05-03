#include <cstdlib>
#include <memory>
#include <vector>
#include <numeric>
#include "player.hpp"
#include "chat_message.hpp"
#include "poker_table.hpp"
#include "asio.hpp"
#include "json.hpp"
#include "poker_player.hpp"

using asio::ip::tcp;
using namespace std;
using json = nlohmann::json;

Poker_Player::Poker_Player(tcp::socket socket, Poker_Table& table_)
    : socket_(move(socket)),
      table_(table_)
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
            // clear out the old buffer from the last read
            // a '\0' is a good value to make sure a string
            // is terminated
            for (unsigned int i=0; i<chat_message::max_body_length; i++)
            {
                read_msg_.body() [i] = '\0';
            }

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
            cout<<"from player:"<<endl;
            cout<<to_dealer.dump(2)<<endl;

            json to_player;  // represents the entire game state.  sent to all players
            if(table_.game_state!=WAITING)
                to_player["turn"] = table_.dealer.current_player->playerUUID;
            else
                to_player["turn"] = "";
            to_player["chat"] = "";
            to_player["dealer_comment"] = "";
            to_player["recommended_play"] = "";
            to_player["current_pot"] = table_.current_pot;
            to_player["minimum_bet"] = table_.minimum_bet;
            to_player["hand"] = {};

            // Game Logic
            if(to_dealer["event"]=="join")
            {
                string n = string(to_dealer["from"].at("name")) + " has joined the table.";
                cout<<n<<endl;
                self->name = string(to_dealer["from"].at("name"));
                self->playerUUID = string(to_dealer["from"].at("uuid"));
                to_player["dealer_comment"] = n;
                if(table_.game_state==WAITING&&table_.enough_player())
                {
                    string str = "Send 'start' in chat to start the game!";
                    to_player["recommended_play"] = str;
                }
            }
            else if(to_dealer["event"]=="fold")
            {
                string player_action = table_.dealer.current_player->name + " folded! ";
                set<player_ptr>::iterator it;
                it = table_.players.find(table_.dealer.current_player);
                table_.folded_players.insert(table_.dealer.current_player);

                if(it==(--table_.players.end()))
                {
                    table_.players.erase(it);
                    table_.dealer.current_player = *(--table_.players.end());
                }
                else
                {
                    table_.dealer.next_player(*(it));
                    table_.players.erase(it);
                }

                if(table_.players.size()==1)
                {
                    table_.game_state = WIN;
                    cout<<"game_state = WIN"<<endl;
                    table_.dealer.current_player = *(table_.players.begin());
                }

                to_player["turn"] = table_.dealer.current_player->playerUUID;
                string str = player_action + table_.dealer.current_player->name + " turn";
                to_player["dealer_comment"] = str;
            }
            else if(to_dealer["event"]=="chat"&&table_.enough_player())
            {
                string temp = (string) to_dealer["chat"];

                if((temp.find("start")>=0)&&(table_.game_state==WAITING))
                {
                    table_.game_state = ANTE;
                    cout<<"game_state = ANTE"<<endl;
                    table_.dealer.current_player = *(table_.players.begin());
                    to_player["turn"] = table_.dealer.current_player->playerUUID;
                    cout<<"current player turn: "<<table_.dealer.current_player->playerUUID<<endl;
                    string str = table_.dealer.current_player->name + " turn.";
                    to_player["dealer_comment"] = str;
                    str = "\0";
                    str = "Bet the ante of $" + to_string(table_.minimum_bet) + ".";
                    to_player["recommended_play"] = str;
                }
                else
                {
                    string str = "";
                    to_player["dealer_comment"] = str;
                    to_player["recommended_play"] = str;
                    to_player["chat"] = string(to_dealer["chat"]);
                }
            }
            else if(table_.game_state==ANTE&&to_dealer["event"]=="bet")
            {
                set<player_ptr>::iterator it;
                it = table_.players.find(table_.dealer.current_player);
                string player_action = table_.dealer.current_player->name + " betted $1. ";
                table_.dealer.current_player->total_balance -= 1;
                table_.dealer.current_player->current_bet += 1;
                table_.current_pot += 1;
                to_player["current_pot"] = table_.current_pot;
                to_player["minimum_bet"] = table_.minimum_bet;

                if(++it==table_.players.end())
                {
                    table_.game_state = BETTING1;
                    cout<<"game_state = BETTING1"<<endl;
                    table_.dealer.current_player = *(table_.players.begin());
                    for(it=table_.players.begin(); it!=table_.players.end(); it++)
                    {
                        (*it)->playerHand.cards.clear();
                        table_.dealer.deal((*it));
                    }
                    to_player["turn"] = table_.dealer.current_player->playerUUID;
                    string str = player_action + table_.dealer.current_player->name + " turn now!";
                    to_player["dealer_comment"] = str;
                    to_player["recommended_play"] = "Check or make a bet.";
                }
                else
                {
                    table_.dealer.next_player(*(it));
                    to_player["turn"] = table_.dealer.current_player->playerUUID;
                    string str = player_action + table_.dealer.current_player->name + " turn now!";
                    to_player["dealer_comment"] = str;
                    str = "\0";
                    str = "Bet the ante of $" + to_string(table_.minimum_bet);
                    to_player["recommended_play"] = str;
                }
            }
            else if(table_.game_state==BETTING1)
            {
                set<player_ptr>::iterator it;
                it = table_.players.find(table_.dealer.current_player);
                string player_action;

                if(to_dealer["event"]=="bet")
                {
                    player_action = table_.dealer.current_player->name + " betted $" + to_string((int) to_dealer["bet"]) + " ";
                    table_.dealer.current_player->total_balance -= (int) to_dealer["bet"];
                    table_.dealer.current_player->current_bet += (int) to_dealer["bet"];
                    table_.current_pot += (int) to_dealer["bet"];
                    table_.minimum_bet += (int) to_dealer["bet"];
                    to_player["current_pot"] = table_.current_pot;
                    to_player["minimum_bet"] = table_.minimum_bet;
                }
                else if(to_dealer["event"]=="check")
                {
                    player_action = table_.dealer.current_player->name + " checked. ";
                }
                else if(to_dealer["event"]=="call")
                {
                    player_action = table_.dealer.current_player->name + " called. ";
                    table_.dealer.current_player->total_balance -= (table_.minimum_bet-table_.dealer.current_player->current_bet);
                    table_.dealer.current_player->current_bet += (table_.minimum_bet-table_.dealer.current_player->current_bet);
                    table_.current_pot += (table_.minimum_bet-table_.dealer.current_player->current_bet);
                    to_player["current_pot"] = table_.current_pot;
                    to_player["minimum_bet"] = table_.minimum_bet;
                }
                else if(to_dealer["event"]=="raise")
                {
                    player_action = table_.dealer.current_player->name + " raised $" + to_string((int) to_dealer["raise"]) + " ";
                    table_.dealer.current_player->total_balance -= (int) to_dealer["raise"] + table_.minimum_bet;
                    table_.dealer.current_player->current_bet += (int) to_dealer["raise"] + table_.minimum_bet;
                    table_.current_pot += (int) to_dealer["raise"] + table_.minimum_bet;
                    table_.minimum_bet += (int) to_dealer["raise"];
                    to_player["current_pot"] = table_.current_pot;
                    to_player["minimum_bet"] = table_.minimum_bet;
                }
                else if(to_dealer["event"]=="all-in")
                {
                    player_action = table_.dealer.current_player->name + " all-in. ";
                    int total = table_.dealer.current_player->total_balance;
                    table_.dealer.current_player->total_balance -= total;
                    table_.dealer.current_player->current_bet += total;
                    table_.current_pot += total;
                    table_.minimum_bet += total-table_.minimum_bet;
                    to_player["current_pot"] = table_.current_pot;
                    to_player["minimum_bet"] = table_.minimum_bet;
                }

                if(++it==table_.players.end()&&table_.all_bet_valid())
                {
                    table_.game_state = SWAPPING;
                    cout<<"game_state = SWAPPING"<<endl;
                    table_.dealer.current_player = *(table_.players.begin());

                    to_player["turn"] = table_.dealer.current_player->playerUUID;
                    string str = player_action + table_.dealer.current_player->name + " turn";
                    to_player["dealer_comment"] = str;
                    to_player["recommended_play"] = "Replace unwanted cards";
                }
                else if(it==table_.players.end()&&!table_.all_bet_valid())
                {
                    table_.dealer.current_player = *(table_.players.begin());
                    to_player["turn"] = table_.dealer.current_player->playerUUID;
                    string str = player_action + table_.dealer.current_player->name + " turn";
                    to_player["dealer_comment"] = str;
                    to_player["recommended_play"] = "Call, Check, Raise, All-in or Fold";
                }
                else
                {
                    table_.dealer.next_player(*(it));
                    to_player["turn"] = table_.dealer.current_player->playerUUID;
                    string str = player_action + table_.dealer.current_player->name + " turn";
                    to_player["dealer_comment"] = str;
                    to_player["recommended_play"] = "Call, Check, Raise, All-in or Fold";
                }
            }
            else if(table_.game_state==SWAPPING&&to_dealer["event"]=="replace")
            {
                set<player_ptr>::iterator it;
                it = table_.players.find(table_.dealer.current_player);
                vector<int> replace_vector = to_dealer["replace_vector"];
                int numSwapped = accumulate(replace_vector.begin(),replace_vector.end(),
                                            decltype(replace_vector)::value_type(0));;
                string player_action = table_.dealer.current_player->name + " swaped " + to_string(numSwapped) + " cards. ";
                table_.dealer.replace_cards(replace_vector);

                if(++it==table_.players.end())
                {
                    table_.game_state = BETTING2;
                    cout<<"game_state = BETTING2"<<endl;
                    table_.dealer.current_player = *(table_.players.begin());
                    to_player["turn"] = table_.dealer.current_player->playerUUID;
                    string str = player_action + table_.dealer.current_player->name + " turn";
                    to_player["dealer_comment"] = str;
                    to_player["recommended_play"] = "Check or make a bet.";
                }
                else
                {
                    table_.dealer.next_player(*(it));
                    to_player["turn"] = table_.dealer.current_player->playerUUID;
                    string str = player_action + table_.dealer.current_player->name + " turn";
                    to_player["dealer_comment"] = str;
                    to_player["recommended_play"] = "Replace unwanted cards";
                }
            }
            else if(table_.game_state==BETTING2)
            {
                set<player_ptr>::iterator it;
                it = table_.players.find(table_.dealer.current_player);
                string player_action;

                if(to_dealer["event"]=="bet")
                {
                    player_action = table_.dealer.current_player->name + " betted $" + to_string((int) to_dealer["bet"]) + " ";
                    table_.dealer.current_player->total_balance -= (int) to_dealer["bet"];
                    table_.dealer.current_player->current_bet += (int) to_dealer["bet"];
                    table_.current_pot += (int) to_dealer["bet"];
                    table_.minimum_bet += (int) to_dealer["bet"];
                    to_player["current_pot"] = table_.current_pot;
                    to_player["minimum_bet"] = table_.minimum_bet;
                }
                else if(to_dealer["event"]=="check")
                {
                    player_action = table_.dealer.current_player->name + " checked. ";
                }
                else if(to_dealer["event"]=="call")
                {
                    player_action = table_.dealer.current_player->name + " called. ";
                    table_.dealer.current_player->total_balance -= (table_.minimum_bet-table_.dealer.current_player->current_bet);
                    table_.dealer.current_player->current_bet += (table_.minimum_bet-table_.dealer.current_player->current_bet);
                    table_.current_pot += (table_.minimum_bet-table_.dealer.current_player->current_bet);
                    to_player["current_pot"] = table_.current_pot;
                    to_player["minimum_bet"] = table_.minimum_bet;
                }
                else if(to_dealer["event"]=="raise")
                {
                    player_action = table_.dealer.current_player->name + " raised $" + to_string((int) to_dealer["raise"]) + " ";
                    table_.dealer.current_player->total_balance -= (int) to_dealer["raise"];
                    table_.dealer.current_player->current_bet += (int) to_dealer["raise"];
                    table_.current_pot += (int) to_dealer["raise"];
                    table_.minimum_bet += ((int) to_dealer["raise"])-table_.minimum_bet;
                    to_player["current_pot"] = table_.current_pot;
                    to_player["minimum_bet"] = table_.minimum_bet;
                }
                else if(to_dealer["event"]=="all-in")
                {
                    player_action = table_.dealer.current_player->name + " all-in. ";
                    int total = table_.dealer.current_player->total_balance;
                    table_.dealer.current_player->total_balance -= total;
                    table_.dealer.current_player->current_bet += total;
                    table_.current_pot += total;
                    table_.minimum_bet += total-table_.minimum_bet;
                    to_player["current_pot"] = table_.current_pot;
                    to_player["minimum_bet"] = table_.minimum_bet;
                }

                if(++it==table_.players.end()&&table_.all_bet_valid())
                {
                    table_.game_state = WIN;
                    cout<<"game_state = WIN"<<endl;
                    table_.dealer.current_player = *(table_.players.begin());
                }
                else if(it==table_.players.end()&&!table_.all_bet_valid())
                {
                    table_.dealer.current_player = *(table_.players.begin());
                    to_player["turn"] = table_.dealer.current_player->playerUUID;
                    string str = player_action + table_.dealer.current_player->name + " turn";
                    to_player["dealer_comment"] = str;
                    to_player["recommended_play"] = "Call, Check, Raise, All-in or Fold";
                }
                else
                {
                    table_.dealer.next_player(*(it));
                    to_player["turn"] = table_.dealer.current_player->playerUUID;
                    string str = player_action + table_.dealer.current_player->name + " turn";
                    to_player["dealer_comment"] = str;
                    to_player["recommended_play"] = "Call, Check, Raise, All-in or Fold";
                }
            }

            if(table_.game_state==WIN)
            {
                table_.game_state = WAITING;
                cout<<"game_state = WAITING"<<endl;

                player_ptr winner;
                winner = table_.find_winner();
                winner->total_balance += table_.current_pot;
                string player_action = winner->name + " Won the round! ";
                table_.current_pot = 0;
                table_.minimum_bet = 1;

                set<player_ptr>::iterator it;
                for(it=table_.folded_players.begin(); it!=table_.folded_players.end(); it++)
                {
                    table_.players.insert(*it);
                }
                table_.folded_players.clear();

                for(it=table_.players.begin(); it!=table_.players.end(); it++)
                {
                    (*it)->current_bet = 0;
                    (*it)->playerHand.cards.clear();
                    (*it)->playerHand.cards = (*it)->playerHand.default_cards;
                }

                to_player["dealer_comment"] = player_action;
                to_player["recommended_play"] = "Send 'start' in chat to start the a new round!";
                to_player["turn"] = "";
                to_player["current_pot"] = table_.current_pot;
                to_player["minimum_bet"] = table_.minimum_bet;
            }

            set<player_ptr>::iterator it;
            for(it=(table_.players.begin()); it!=(table_.players.end()); ++it)
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
            cout<<to_player.dump(2)<<endl;
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
                      [this, self](error_code ec, size_t /*length*/)
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
