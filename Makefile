
# all: output
# 	./output

# output: Assignment1.o
# 	g++ Assignment1.o -o output -std=c++11 `pkg-config --cflags --libs opencv`

# Assignment1.o: Assignment1.cpp
# 	g++ -c Assignment1.cpp -std=c++11 `pkg-config --cflags --libs opencv`

# clean: 
# 	rm -rf *.o Assignment1.o outputs/Grayscale_empty.jpg output

allpart2:
	g++ assignment1_2.cpp -o part2 -std=c++11 `pkg-config --cflags --libs opencv`

allpart1: 
	g++ Assignment1.cpp -o part1 -std=c++11 `pkg-config --cflags --libs opencv`

clean:
	rm -rf part1 part2