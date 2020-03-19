CXXFLAGS += -std=c++11
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

all: app

app: main.o poker_player.o *.h
				$(CXX) $(CXXFLAGS) main.o poker_player.o $(GTKFLAGS) -o app

main.o: main.cpp
				$(CXX) $(CXXFLAGS) -c main.cpp $(GTKFLAGS) -o main.o

poker_player.o: poker_player.cpp *.h
				$$(CXX) $(CXXFLAGS) -c poker_player.cpp $(GTKFLAGS) -o poker_player.o

clean:
				rm -f *.o *.gch app
