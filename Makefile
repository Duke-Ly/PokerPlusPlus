# CSE3310-003 Group 4 Project makefile

GTKLINKFLAGS    = $(shell pkg-config --libs gtkmm-3.0)
GTKCOMPILEFLAGS = $(shell pkg-config --cflags gtkmm-3.0)
CXXFLAGS += -DASIO_STANDALONE -Wall -O0 -g -std=c++11
CPPFLAGS += ${GTKCOMPILEFLAGS} -I./include -I./asio-1.12.2/include
LDLIBS   += -lpthread ${GTKLINKFLAGS}
#GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

TARGETS = asio-1.12.2 src/chat_server src/chat_client src/json src/test_code \
			src/player_gui

all: ${TARGETS}

asio-1.12.2:
	tar xzf asio-1.12.2.tar.gz

clean:
	-rm -rf asio-1.12.2
	-rm -f src/player_gui
	-rm -f src/chat_client
	-rm -f src/chat_server
	-rm -f src/json
	-rm -f src/test_code
