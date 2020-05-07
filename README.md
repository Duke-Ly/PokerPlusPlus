# PokerPlusPlus
A Five-Card Draw Poker Game created using C++11, gtkmm 3.0, Boost ASIO and Boost UUID library.

PokerPlusPlus is based on Five-Card Draw Poker. 
The aim of the game is to make the best five-card poker hand. 
At the start of the game each player recieves five cards face down and a total chip balance of $100. 
Each player then has the opportunity to replace select cards with new cards from the dealer's stack. 
Then players will place a bet. The strongest hand wins the pot! Players play until their chip balance is $0.  

Authors: Armando Castillo and Duke Ly

Dependencies: 
Must have the boost development libs installed
Must have the gtkmm 3.0 or higher development libs installed 

yum install boost-devel
yum install gtkmm30-devel  

or

sudo apt install libboost-dev
sudo apt-get install libgtkmm-3.0-dev

Set up Instruction:
git clone https://github.com/Duke-Ly/PokerPlusPlus.git
-- or -- download the project from the link above

Compile Instruction:
Nagivate to the src directory and run make or make all
	./PokerPlusPlus $ cd src
./PokerPlusPlus/src $ make

To run the server application, in the src directory call the dealer application with parameter 9000
./PokerPlusPlus/src $ ./dealer 9000

To run the client application, in the src directory call the player application with parameter 127.0.0.1 9000
./PokerPlusPlus/src $ ./player 127.0.0.1 9000

Boost Testing Instruction:
To compile and run the shuffle_test first you need to comment out the main function in dealer.cpp
This can be done by going to line 144 in dealer.cpp and set the "#if 1" to "#if 0"
You will have to manually uncomment it for the game to run properly
Then in the src directory run make shuffle_test and then run the shuffle_test program
./PokerPlusPlus/src $ make shuffle_test
./PokerPlusPlus/src $ ./shuffle_test