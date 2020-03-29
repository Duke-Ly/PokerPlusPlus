# CSE3310-003 Group 4 Project makefile

CXXFLAGS += -DASIO_STANDALONE -Wall -O0 -g -std=c++11
CPPFLAGS += -I./include -I./asio-1.12.2/include
LDLIBS   += -lpthread
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

TARGETS = asio-1.12.2 src/chat_server src/chat_client src/json

all: ${TARGETS}

asio-1.12.2:
	tar xzf asio-1.12.2.tar.gz

clean:
	-rm -rf asio-1.12.2
#	-rm -f src/main
	-rm -f src/chat_client
	-rm -f src/chat_server		
	-rm -f src/json