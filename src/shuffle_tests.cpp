#include <map>
#include <string>
#include "card.hpp"
#include "dealer.hpp"
#include "hand.hpp"

#define BOOST_TEST_MODULE shuffle_tests
#include <boost/test/included/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE (shuffle_tests)

BOOST_AUTO_TEST_CASE (number_of_cards_in_deck)
{
    Dealer dealer;    // makes a deck
    BOOST_CHECK ( dealer.deck.size() == 52 );
}

BOOST_AUTO_TEST_CASE (legal_deck)
{
    // want to make sure that there is only 1 card of
    // each type
    map<string, int> M;
    Dealer dealer;
    // count how many of each value
    for(auto & card: dealer.deck)
    {
        auto result = M.insert(std::pair<std::string, int>(card.cardID, 1));
        if (result.second == false)
            result.first->second++;
    }

    bool duplicate = false;

    // Iterate over the map
    for (auto & elem : M)
    {
        // If frequency count is greater than 1 then its a duplicate element
        if (elem.second > 1)
        {
            duplicate = true;
        }
    }
    BOOST_CHECK_EQUAL( duplicate, false );
}

BOOST_AUTO_TEST_CASE (true_shuffle)
{
    Dealer dealer;
    vector<Card> deck_copy = dealer.deck;
    dealer.shuffleDeck();
    int shuffled_match = 0;
    for(int i=0; i<52; i++)
    {
        if(dealer.deck[i].cardID.compare(deck_copy[i].cardID)==0)
            shuffled_match++;
    }
    // if shuffled_match == 52, deck has not been shuffled

    BOOST_CHECK ( shuffled_match < 52 );
}

BOOST_AUTO_TEST_SUITE_END( )
