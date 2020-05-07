#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE winning_hand_tests
#include <boost/test/unit_test.hpp>

#include "hand.hpp"
#include "card.hpp"

using namespace boost;
using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(winning_hand_tests)

BOOST_AUTO_TEST_CASE(one_pair)
{
  Hand H;
  H.addCard(Card(13, SPADES, "KS"));
  H.addCard(Card(13, DIAMONDS, "KD"));
  H.addCard(Card(6, HEARTS, "6H"));
  H.addCard(Card(3, CLUBS, "3C"));
  H.addCard(Card(2, DIAMONDS, "JS"));

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 1);
}

BOOST_AUTO_TEST_CASE(two_pair)
{
  Hand H;
  H.addCard(Card(12, SPADES, "QS"));
  H.addCard(Card(12, CLUBS, "QC"));
  H.addCard(Card(4, SPADES, "4S"));
  H.addCard(Card(4, HEARTS, "4H"));
  H.addCard(Card(1, SPADES, "AS"));

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 2);
}

BOOST_AUTO_TEST_CASE(three_of_a_kind)
{
  Hand H;
  H.addCard(Card(8, SPADES, "8S"));
  H.addCard(Card(8, CLUBS, "8C"));
  H.addCard(Card(8, HEARTS, "8H"));
  H.addCard(Card(11, SPADES, "JS"));
  H.addCard(Card(5, HEARTS, "5H"));

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 3);
}

BOOST_AUTO_TEST_CASE(straight)
{
  Hand H;
  H.addCard(Card(2, SPADES, "2S"));
  H.addCard(Card(3, CLUBS, "3C"));
  H.addCard(Card(4, HEARTS, "4H"));
  H.addCard(Card(5, DIAMONDS, "5D"));
  H.addCard(Card(6, CLUBS, "6C"));

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 4);
}

BOOST_AUTO_TEST_CASE(flush)
{
  Hand H;
  H.addCard(Card(13, CLUBS, "KC"));
  H.addCard(Card(10, CLUBS, "10C"));
  H.addCard(Card(9, CLUBS, "9C"));
  H.addCard(Card(7, CLUBS, "7C"));
  H.addCard(Card(6, CLUBS, "6C"));

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 5);
}

BOOST_AUTO_TEST_CASE(full_house)
{
  Hand H;
  H.addCard(Card(10, CLUBS, "10C"));
  H.addCard(Card(10, SPADES, "10S"));
  H.addCard(Card(10, DIAMONDS, "10D"));
  H.addCard(Card(2, SPADES, "2S"));
  H.addCard(Card(2, HEARTS, "2H"));

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 6);
}

BOOST_AUTO_TEST_CASE(four_of_a_kind)
{
  Hand H;
  H.addCard(Card(11, CLUBS, "11C"));
  H.addCard(Card(11, SPADES, "11S"));
  H.addCard(Card(11, DIAMONDS, "11D"));
  H.addCard(Card(11, HEARTS, "11H"));
  H.addCard(Card(1, CLUBS, "AC"));

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 7);
}

BOOST_AUTO_TEST_CASE(straight_flush)
{
  Hand H;
  H.addCard(Card(5, CLUBS, "5C"));
  H.addCard(Card(6, CLUBS, "6C"));
  H.addCard(Card(7, CLUBS, "7C"));
  H.addCard(Card(8, CLUBS, "8C"));
  H.addCard(Card(9, CLUBS, "9C"));

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 8);
}

BOOST_AUTO_TEST_CASE(royal_flush)
{
  Hand H;
  H.addCard(Card(10, CLUBS, "10C"));
  H.addCard(Card(11, CLUBS, "JC"));
  H.addCard(Card(12, CLUBS, "QC"));
  H.addCard(Card(13, CLUBS, "KC"));
  H.addCard(Card(1, CLUBS, "AC"));

  H.sortHand();
  H.calcHandValue();

  BOOST_CHECK(H.handValue == 9);
}

BOOST_AUTO_TEST_SUITE_END()
