#include "poker_player.hpp"

Poker_Player::Poker_Player(tcp::socket socket, poker_table& table)
      : socket(std::move(socket)), table(table) { }

Poker_Player::~Poker_Player() { }

void Poker_Player::start()
{
  table.join(shared_from_this());
  do_read_header();
}
void Poker_Player::deliver(chat_message& msg)
{
  bool write_in_progress = !write_msgs.empty();
    write_msgs.push_back(msg);
    if (!write_in_progress)
    {
      do_write();
    }
}

void Poker_Player::do_read_header()
{
  auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg.data(), chat_message::header_length),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg.decode_header())
          {
            do_read_body();
          }
          else
          {
            table.leave(shared_from_this());
          }
        });
}
void Poker_Player::do_read_body()
{
  auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            // Here is where messages arrive from the client
            // this is where the design makes it easy or hard

            // ignore anything in the message body
            read_msg_.body_length(0);
            read_msg_.gs.dealer_cards_valid = false;
            read_msg_.gs.player_cards_valid = false;
            // is it a join
            if (read_msg_.ca.join && read_msg_.ca.name_valid)
            {
               std::cout << "the name is " << read_msg_.ca.name << std::endl;
               std::string m = std::string(read_msg_.ca.name) + " has joined.";
               strcpy(read_msg_.body(),m.c_str());
               read_msg_.body_length(strlen(read_msg_.body()));
               self->name = std::string (read_msg_.ca.name);
            }

            // display the cards if everyone has joined
            if (table.all_players_have_a_name ())
            {
               table.Dealer.deal();
               read_msg_.gs.dealer_cards_valid = true;

            }
            read_msg_.encode_header(); // so the body text above gets sent
            table.deliver(read_msg_);
            do_read_header();
          }
          else
          {
            table.leave(shared_from_this());
          }
        });
}
void Poker_Player::do_write()
{
  auto self(shared_from_this());
    asio::async_write(socket_,
        asio::buffer(write_msgs.front().data(),
          write_msgs.front().length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs.pop_front();
            if (!write_msgs.empty())
            {
              do_write();
            }
          }
          else
          {
            table.leave(shared_from_this());
          }
        });
}
