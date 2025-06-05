covered := input/atsol.mp3
used := input/dryout.mp3

all: main run clean

main: main.cpp
	-g++ main.cpp -o main -std=c++11 -Wall \
	-I /opt/homebrew/include -L /opt/homebrew/lib \
	-lfftw3 -lm

run: main
	echo "Covering song $(covered)"
	echo "Using song $(used)"
	echo "-------------------"
	-./main $(covered) $(used)

clean: 
	rm main
	rm input/covered.pcm
	rm input/used.pcm

.SILENT: run clean
.PHONY: clean run