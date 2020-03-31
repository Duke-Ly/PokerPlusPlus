# CSE3310-003 Group 4 Project makefile

GTKLINKFLAGS    = $(shell pkg-config --libs gtkmm-3.0)
GTKCOMPILEFLAGS = $(shell pkg-config --cflags gtkmm-3.0)
CXXFLAGS += -DASIO_STANDALONE -Wall -O0 -g -std=c++11
CPPFLAGS += ${GTKCOMPILEFLAGS} -I../include -I../asio-1.12.2/include
LDLIBS   += -lpthread ${GTKLINKFLAGS}
#GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

TARGETS = ../asio-1.12.2 chat_server chat_client json test_code \
			main

all: ${TARGETS}

../asio-1.12.2:
	tar xzf asio-1.12.2.tar.gz
	
Player_GUI.o: Player_GUI.cpp ../include/Player_GUI.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ -c $< ${LDLIBS}
	
main: main.cpp Player_GUI.o
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< Player_GUI.o ${LDLIBS}

clean:
	-rm -rf ../asio-1.12.2
	-rm -f main
	-rm -f chat_client
	-rm -f chat_server
	-rm -f json
	-rm -f test_code
	-rm -f Player_GUI.o