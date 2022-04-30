CPP     = g++ -std=c++17
CFLAGS  = -I./include -I../eigen -I../quickhull

all:
	$(CPP) example.cpp ./src/*.cpp ../quickhull/*.cpp $(CFLAGS) -o example -O3
	-mkdir -p bin
	-mv example ./bin

clean:
	-rm -f -r bin
