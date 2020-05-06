#include <cstdlib>
#include <ctime>
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
    current_player = nullptr;
    card_idx = 0;
}

Dealer::~Dealer()
{
    deck.clear();
}

void Dealer::createDeck()
{
    deck.emplace_back(1, DIAMONDS, "AD");
    deck.emplace_back(2, DIAMONDS, "2D");
    deck.emplace_back(3, DIAMONDS, "3D");
    deck.emplace_back(4, DIAMONDS, "4D");
    deck.emplace_back(5, DIAMONDS, "5D");
    deck.emplace_back(6, DIAMONDS, "6D");
    deck.emplace_back(7, DIAMONDS, "7D");
    deck.emplace_back(8, DIAMONDS, "8D");
    deck.emplace_back(9, DIAMONDS, "9D");
    deck.emplace_back(10, DIAMONDS, "10D");
    deck.emplace_back(11, DIAMONDS, "JD");
    deck.emplace_back(12, DIAMONDS, "QD");
    deck.emplace_back(13, DIAMONDS, "KD");

    deck.emplace_back(1, HEARTS, "AH");
    deck.emplace_back(2, HEARTS, "2H");
    deck.emplace_back(3, HEARTS, "3H");
    deck.emplace_back(4, HEARTS, "4H");
    deck.emplace_back(5, HEARTS, "5H");
    deck.emplace_back(6, HEARTS, "6H");
    deck.emplace_back(7, HEARTS, "7H");
    deck.emplace_back(8, HEARTS, "8H");
    deck.emplace_back(9, HEARTS, "9H");
    deck.emplace_back(10, HEARTS, "10H");
    deck.emplace_back(11, HEARTS, "JH");
    deck.emplace_back(12, HEARTS, "QH");
    deck.emplace_back(13, HEARTS, "KH");

    deck.emplace_back(1, CLUBS, "AC");
    deck.emplace_back(2, CLUBS, "2C");
    deck.emplace_back(3, CLUBS, "3C");
    deck.emplace_back(4, CLUBS, "4C");
    deck.emplace_back(5, CLUBS, "5C");
    deck.emplace_back(6, CLUBS, "6C");
    deck.emplace_back(7, CLUBS, "7C");
    deck.emplace_back(8, CLUBS, "8C");
    deck.emplace_back(9, CLUBS, "9C");
    deck.emplace_back(10, CLUBS, "10C");
    deck.emplace_back(11, CLUBS, "JC");
    deck.emplace_back(12, CLUBS, "QC");
    deck.emplace_back(13, CLUBS, "KC");

    deck.emplace_back(1, SPADES, "AS");
    deck.emplace_back(2, SPADES, "2S");
    deck.emplace_back(3, SPADES, "3S");
    deck.emplace_back(4, SPADES, "4S");
    deck.emplace_back(5, SPADES, "5S");
    deck.emplace_back(6, SPADES, "6S");
    deck.emplace_back(7, SPADES, "7S");
    deck.emplace_back(8, SPADES, "8S");
    deck.emplace_back(9, SPADES, "9S");
    deck.emplace_back(10, SPADES, "10S");
    deck.emplace_back(11, SPADES, "JS");
    deck.emplace_back(12, SPADES, "QS");
    deck.emplace_back(13, SPADES, "KS");

    cout<<"deck have been created"<<endl;
}

void Dealer::shuffleDeck()
{
    card_idx = 0;
    srand((unsigned) time(0));
    int randomNumber = rand()% 100 + 1;
    //auto rng = default_random_engine {};
    //shuffle(deck.begin(), deck.end(), rng);
    for(int i=0; i<randomNumber; i++)
        random_shuffle(deck.begin(), deck.end());
    cout<<"deck have been shuffled"<<endl;
}

void Dealer::deal(player_ptr dealPlayer)
{
    for(int i=0; i<5; i++)
        dealPlayer->hand.addCard(deck[card_idx++]);

    cout<<"Hand had been dealt to player"<<endl;
    dealPlayer->hand.sortHand();
    dealPlayer->hand.calcHandValue();
    dealPlayer->hand.findHighCard();
}

void Dealer::next_player(player_ptr nextPlayer)
{
    current_player = nextPlayer;
    cout<<"current_player = "<<current_player->name<<endl;
}

void Dealer::replace_cards(vector<int> replace_vector)
{
    for(int i=4; i>=0; i--)
    {
        if(replace_vector[i])
        {
            current_player->hand.removeCard(current_player->hand.cards[i]);
        }
    }
    while(!current_player->hand.isMaxSize())
    {
        current_player->hand.addCard(deck[card_idx++]);
    }

    current_player->hand.sortHand();
    current_player->hand.calcHandValue();
    current_player->hand.findHighCard();
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
        for(int i = 1; i < argc; ++i)
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
