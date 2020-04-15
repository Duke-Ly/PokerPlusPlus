#include "poker_table.hpp"

Poker_Table::Poker_Table()
{

}

Poker_Table::~Poker_Table() { }

void Poker_Table::join(player_ptr player)
{
	std::cout<<"new player joining the table"<<std::endl;
	players.insert(player);
}

void Poker_Table::leave(player_ptr player)
{
	players.erase(player);
}

void Poker_Table::deliver(const chat_message& msg)
{
	recent_msgs_.push_back(msg);
	while (recent_msgs_.size()> max_recent_msgs)
		recent_msgs_.pop_front();
	
	for (auto player_: players)
		player_ -> deliver (msg);
}

bool Poker_Table::all_players_have_UUID()
{
	bool retval = true;
	for (auto player : players)
		retval = retval && (player->name > "");
	
	return retval;
}
