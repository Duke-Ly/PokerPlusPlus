# CSE3310-003 Group 4 Project makefile

GTKLINKFLAGS    = $(shell pkg-config --libs gtkmm-3.0)
GTKCOMPILEFLAGS = $(shell pkg-config --cflags gtkmm-3.0)
CXXFLAGS += -DASIO_STANDALONE -Wall -O0 -g -std=c++11
CPPFLAGS += ${GTKCOMPILEFLAGS} -I../include -I../asio-1.12.2/include
LDLIBS   += -lpthread ${GTKLINKFLAGS}
#GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

TARGETS = asio-1.12.2 chat_server chat_client json test_code \
			main

all: ${TARGETS}

asio-1.12.2:
	tar xzf ../asio-1.12.2.tar.gz -C ..

card.o: card.cpp ../include/card.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}

hand.o: hand.cpp ../include/hand.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}
	
player.o: player.cpp ../include/player.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}

dealer.o: dealer.cpp ../include/dealer.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}

poker_table.o: poker_table.cpp ../include/poker_table.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}

poker_player.o: poker_player.cpp ../include/poker_player.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}
	
poker_game.o: poker_game.cpp ../include/poker_game.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}

chat_server: chat_client.cpp card.o hand.o player.o dealer.o poker_table.o poker_player.o poker_game.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< ${LDLIBS}

clean:
	-rm -rf ../asio-1.12.2
	-rm -f chat_client
	-rm -f chat_server
	-rm -f json
	-rm -f test_code
	-rm -f main
	-rm *.o