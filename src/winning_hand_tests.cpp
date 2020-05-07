#define BOOST_TEST_MODULE winning_hand_tests
#include <boost/test/included/unit_test.hpp>

#include "hand.hpp"
#include "card.hpp"

BOOST_AUTO_TEST_SUITE(winning_hand_tests)

BOOST_AUTO_TEST_CASE(one_pair)
{
  Hand H;
  
  Card c1(13, SPADES, "KS");
  Card c2(13, DIAMONDS, "KD");
  Card c3(6, HEARTS, "6H");
  Card c4(3, CLUBS, "3C");
  Card c5(2, DIAMONDS, "2D");
  
  H.cards.clear();
  
  H.addCard(c1);
  H.addCard(c2);
  H.addCard(c3);
  H.addCard(c4);
  H.addCard(c5);

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 1);
}

BOOST_AUTO_TEST_CASE(two_pair)
{
  Hand H;
  
  Card c1(12, SPADES, "QS");
  Card c2(12, CLUBS, "QC");
  Card c3(4, SPADES, "4S");
  Card c4(4, HEARTS, "4H");
  Card c5(1, SPADES, "AS");
  
  H.cards.clear();
  
  H.addCard(c1);
  H.addCard(c2);
  H.addCard(c3);
  H.addCard(c4);
  H.addCard(c5);

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 2);
}

BOOST_AUTO_TEST_CASE(three_of_a_kind)
{
  Hand H;
  
  Card c1(8, SPADES, "8S");
  Card c2(8, CLUBS, "8C");
  Card c3(8, HEARTS, "8H");
  Card c4(11, SPADES, "JS");
  Card c5(5, HEARTS, "5H");
    
  H.cards.clear();
 
  H.addCard(c1);
  H.addCard(c2);
  H.addCard(c3);
  H.addCard(c4);
  H.addCard(c5);

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 3);
}

BOOST_AUTO_TEST_CASE(straight)
{
  Hand H;
  
  Card c1(2, SPADES, "2S");
  Card c2(3, CLUBS, "3C");
  Card c3(4, HEARTS, "4H");
  Card c4(5, DIAMONDS, "5D");
  Card c5(6, CLUBS, "6C");
    
  H.cards.clear();
 
  H.addCard(c1);
  H.addCard(c2);
  H.addCard(c3);
  H.addCard(c4);
  H.addCard(c5);

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 4);
}

BOOST_AUTO_TEST_CASE(flush)
{
  Hand H;
  
  Card c1(13, CLUBS, "KC");
  Card c2(10, CLUBS, "10C");
  Card c3(9, CLUBS, "9C");
  Card c4(7, CLUBS, "7C");
  Card c5(6, CLUBS, "6C");
    
  H.cards.clear();
 
  H.addCard(c1);
  H.addCard(c2);
  H.addCard(c3);
  H.addCard(c4);
  H.addCard(c5);

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 5);
}

BOOST_AUTO_TEST_CASE(full_house)
{
  Hand H;
  
  Card c1(10, CLUBS, "10C");
  Card c2(10, SPADES, "10S");
  Card c3(10, DIAMONDS, "10D");
  Card c4(2, SPADES, "2S");
  Card c5(2, HEARTS, "2H");
    
  H.cards.clear();
 
  H.addCard(c1);
  H.addCard(c2);
  H.addCard(c3);
  H.addCard(c4);
  H.addCard(c5);

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 6);
}

BOOST_AUTO_TEST_CASE(four_of_a_kind)
{
  Hand H;
  
  Card c1(11, CLUBS, "11C");
  Card c2(11, SPADES, "11S");
  Card c3(11, DIAMONDS, "11D");
  Card c4(11, HEARTS, "11H");
  Card c5(1, CLUBS, "AC");
  
  H.cards.clear();
 
  H.addCard(c1);
  H.addCard(c2);
  H.addCard(c3);
  H.addCard(c4);
  H.addCard(c5);

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 7);
}

BOOST_AUTO_TEST_CASE(straight_flush)
{
  Hand H;
  
  Card c1(5, CLUBS, "5C");
  Card c2(6, CLUBS, "6C");
  Card c3(7, CLUBS, "7C");
  Card c4(8, CLUBS, "8C");
  Card c5(9, CLUBS, "9C");
    
  H.cards.clear();
 
  H.addCard(c1);
  H.addCard(c2);
  H.addCard(c3);
  H.addCard(c4);
  H.addCard(c5);

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 8);
}

BOOST_AUTO_TEST_CASE(royal_flush)
{
  Hand H;
  
  Card c1(10, CLUBS, "10C");
  Card c2(11, CLUBS, "JC");
  Card c3(12, CLUBS, "QC");
  Card c4(13, CLUBS, "KC");
  Card c5(1, CLUBS, "AC");
    
  H.cards.clear();
 
  H.addCard(c1);
  H.addCard(c2);
  H.addCard(c3);
  H.addCard(c4);
  H.addCard(c5);

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 9);
}

BOOST_AUTO_TEST_SUITE_END()
