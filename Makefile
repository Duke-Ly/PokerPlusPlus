# this is an example makefile for the first homework
# assignment this semester

all: asio-1.12.2 src/main src/Dealer.cpp

asio-1.12.2:
	tar xzf asio-1.12.2.tar.gz

CXXFLAGS+= -DASIO_STANDALONE -Wall -O0 -g -std=c++11
CPPFLAGS+= -I./include -I./asio-1.12.2/include
GTKFLAGS+= `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`
LDLIBS+= `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs` -lpthread

clean:
	-rm -rf asio-1.12.2
	-rm -f src/main

		


