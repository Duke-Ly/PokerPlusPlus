#include <cstdlib>
#include <exception>
#include <vector>
#include <iostream>
#include <algorithm>
#include <list>
#include <random>
#include <memory>
#include "dealer.hpp"
#include "card.hpp"
#include "player.hpp"
#include "asio.hpp"
#include "chat_message.hpp"
#include "poker_game.hpp"

using asio::ip::tcp;
using namespace std;

Dealer::Dealer()
{
    cout<<"Creating a dealer"<<endl;
    createDeck();
    shuffleDeck();
    current_player = NULL;
    card_idx=0;
}

Dealer::~Dealer() {};

void Dealer::createDeck()
{
    cardSuit Dsuit = DIAMONDS;
    cardSuit Hsuit = HEARTS;
    cardSuit Csuit = CLUBS;
    cardSuit Ssuit = SPADES;

    deck.push_back(Card(1, Dsuit, "AD"));
    deck.push_back(Card(2, Dsuit, "2D"));
    deck.push_back(Card(3, Dsuit, "3D"));
    deck.push_back(Card(4, Dsuit, "4D"));
    deck.push_back(Card(5, Dsuit, "5D"));
    deck.push_back(Card(6, Dsuit, "6D"));
    deck.push_back(Card(7, Dsuit, "7D"));
    deck.push_back(Card(8, Dsuit, "8D"));
    deck.push_back(Card(9, Dsuit, "9D"));
    deck.push_back(Card(10, Dsuit, "10D"));
    deck.push_back(Card(11, Dsuit, "JD"));
    deck.push_back(Card(12, Dsuit, "QD"));
    deck.push_back(Card(13, Dsuit, "KD"));

    deck.push_back(Card(1, Hsuit, "AH"));
    deck.push_back(Card(2, Hsuit, "2H"));
    deck.push_back(Card(3, Hsuit, "3H"));
    deck.push_back(Card(4, Hsuit, "4H"));
    deck.push_back(Card(5, Hsuit, "5H"));
    deck.push_back(Card(6, Hsuit, "6H"));
    deck.push_back(Card(7, Hsuit, "7H"));
    deck.push_back(Card(8, Hsuit, "8H"));
    deck.push_back(Card(9, Hsuit, "9H"));
    deck.push_back(Card(10, Hsuit, "10H"));
    deck.push_back(Card(11, Hsuit, "JH"));
    deck.push_back(Card(12, Hsuit, "QH"));
    deck.push_back(Card(13, Hsuit, "KH"));

    deck.push_back(Card(1, Csuit, "AC"));
    deck.push_back(Card(2, Csuit, "2C"));
    deck.push_back(Card(3, Csuit, "3C"));
    deck.push_back(Card(4, Csuit, "4C"));
    deck.push_back(Card(5, Csuit, "5C"));
    deck.push_back(Card(6, Csuit, "6C"));
    deck.push_back(Card(7, Csuit, "7C"));
    deck.push_back(Card(8, Csuit, "8C"));
    deck.push_back(Card(9, Csuit, "9C"));
    deck.push_back(Card(10, Csuit, "10C"));
    deck.push_back(Card(11, Csuit, "JC"));
    deck.push_back(Card(12, Csuit, "QC"));
    deck.push_back(Card(13, Csuit, "KC"));

    deck.push_back(Card(1, Ssuit, "AS"));
    deck.push_back(Card(2, Ssuit, "2S"));
    deck.push_back(Card(3, Ssuit, "3S"));
    deck.push_back(Card(4, Ssuit, "4S"));
    deck.push_back(Card(5, Ssuit, "5S"));
    deck.push_back(Card(6, Ssuit, "6S"));
    deck.push_back(Card(7, Ssuit, "7S"));
    deck.push_back(Card(8, Ssuit, "8S"));
    deck.push_back(Card(9, Ssuit, "9S"));
    deck.push_back(Card(10, Ssuit, "10S"));
    deck.push_back(Card(11, Ssuit, "JS"));
    deck.push_back(Card(12, Ssuit, "QS"));
    deck.push_back(Card(13, Ssuit, "KS"));

    cout<<"deck have been created"<<endl;
}

void Dealer::shuffleDeck()
{
    auto rng = default_random_engine {};
    shuffle(deck.begin(), deck.end(), rng);
    cout<<"deck have been shuffled"<<endl;
}

void Dealer::deal(player_ptr dealPlayer)
{
    for(int i=0; i<5; i++)
        dealPlayer->playerHand.addCard(this->deck[card_idx++]);
    dealPlayer->playerHand.sortHand();
}

void Dealer::next_player(player_ptr nextPlayer)
{
    current_player = nextPlayer;
    cout<<"current_player = "<<current_player->name<<endl;
}

int main(int argc, char* argv[])
{
    try
    {
        if(argc < 2)
        {
            cerr<<"Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        asio::io_context io_context;
        list<Poker_Game> servers;
        for (int i = 1; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), atoi(argv[i]));
            servers.emplace_back(io_context, endpoint);
        }

        io_context.run();
    }
    catch(exception& e)
    {
        cerr<<"Exception: "<<e.what()<<"\n";
    }

    return 0;
}
