# CSE3310-003 Group 4 Project makefile

GTKLINKFLAGS    = $(shell pkg-config --libs gtkmm-3.0)
GTKCOMPILEFLAGS = $(shell pkg-config --cflags gtkmm-3.0)
CXXFLAGS += -DASIO_STANDALONE -Wall -O0 -g -std=c++11
CPPFLAGS += ${GTKCOMPILEFLAGS} -I../include -I../asio-1.12.2/include
LDLIBS   += -lpthread ${GTKLINKFLAGS}

ROOTDIR = ../
VPATH = $(ROOTDIR)

TARGETS = $(ROOTDIR)/asio-1.12.2 player dealer

all: ${TARGETS}

$(ROOTDIR)/asio-1.12.2:
	tar xzf ../asio-1.12.2.tar.gz -C ..

hand.o: hand.cpp ../include/hand.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}

poker_table.o: poker_table.cpp ../include/poker_table.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}

poker_player.o: poker_player.cpp ../include/poker_player.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}

poker_game.o: poker_game.cpp ../include/poker_game.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}

player_gui.o: player_gui.cpp ../include/player_gui.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}
	
game_client.o: game_client.cpp ../include/game_client.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}

dealer.o: dealer.cpp ../include/dealer.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}

dealer: dealer.cpp hand.o poker_table.o poker_player.o poker_game.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< hand.o poker_table.o poker_player.o poker_game.o ${LDLIBS}

player: player.cpp player_gui.o game_client.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< player_gui.o game_client.o ${LDLIBS}

json: json.cpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}
	
shuffle_tests: shuffle_tests.cpp ../include/card.hpp dealer.o hand.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< dealer.o hand.o ${LDLIBS}

winning_hand_tests: winning_hand_tests.cpp ../include/card.hpp hand.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< hand.o ${LDLIBS}

clean:
	-rm -rf ../asio-1.12.2
	-rm -f player
	-rm -f dealer
	-rm -f json
	-rm -f shuffle_tests
	-rm -f winning_hand_tests
	-rm -f *.o