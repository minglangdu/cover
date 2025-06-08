# covered := input/atsol.mp3
# used := input/dryout.mp3

covered := input/stalemate.mp3
used := input/atsol.mp3

all: main main.o sdl.o run clean

main.o sdl.o: main.cpp sdl.cpp
	-g++ sdl.cpp -c -o sdl.o -std=c++11 \
	-I /opt/homebrew/include
	-g++ main.cpp -c -o main.o -std=c++11 \
	-I /opt/homebrew/include

main: main.o sdl.o
	-g++ main.o sdl.o -o main -std=c++11 \
	-I /opt/homebrew/include -L /opt/homebrew/lib \
	-lSDL2

run: main
	echo "Covering song $(covered)"
	echo "Using song $(used)"
	echo "-------------------"
	-./main $(covered) $(used)

clean: 
	rm main
	rm sdl.o
	rm main.o
	rm input/covered.pcm
	rm input/used.pcm
	rm res.pcm

.SILENT: run clean main main.o sdl.o
.PHONY: clean run